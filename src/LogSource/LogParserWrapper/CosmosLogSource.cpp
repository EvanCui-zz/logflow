#include "Stdafx.h"
#include "types.h"
#include "CosmosLogSource.h"
#include <vcclr.h>

using namespace LogFlow::DataModel;
using namespace System::IO;

CosmosLogSource::CosmosLogSource(String^ initializationData)
{
    auto filenames = initializationData->Split(gcnew array<wchar_t>(1) { ',' });
    this->fileCount = filenames->Length;
    this->readers = new BinaryLogReader*[fileCount];
    this->needsRefresh = new bool[fileCount];

    for (int i = 0; i < fileCount; i++)
    {
        pin_ptr<const WCHAR> wch = PtrToStringChars(filenames[i]);
        WCHAR wcstring[MAX_PATH];
        wcscpy_s(wcstring, wch);
        this->readers[i] = new BinaryLogReader();
        this->needsRefresh[i] = true;
        DWORD err = this->readers[i]->OpenReader(wcstring);
        if (err != NO_ERROR)
        {
            throw gcnew IOException(String::Format("Cannot open file {0}", filenames[i]), err);
        }
    }
}

CosmosLogSource::!CosmosLogSource()
{
    for (int i = 0; i < fileCount; i++)
    {
        this->readers[i]->CloseReader();
        delete this->readers[i];
    }

    delete[] this->readers;
    delete[] this->needsRefresh;
}

CosmosLogSource::~CosmosLogSource()
{
    this->!CosmosLogSource();
}

// Debug,Info,Status,Warning,Error,AppAlert,Assert
static LogFlow::DataModel::LogLevel	Levels[] =
{
	LogFlow::DataModel::LogLevel::Verbose,
	LogFlow::DataModel::LogLevel::Info,
	LogFlow::DataModel::LogLevel::Detail,
	LogFlow::DataModel::LogLevel::Warning,
	LogFlow::DataModel::LogLevel::Error,
	LogFlow::DataModel::LogLevel::Critical,
	LogFlow::DataModel::LogLevel::Critical,
};

DataItemBase^ CosmosLogSource::ReadLogEntry(int i)
{
	DWORD err;
	if ((err = this->reader->MoveNext()) == NO_ERROR)
	{
		auto node = gcnew DataItemBase();

		node->Level = Levels[this->reader->GetLoggingLevelI()];
		node->Time = DateTime::FromFileTime(this->reader->getEntryTime()).ToUniversalTime();
		node->ThreadId = this->reader->getEntryTid();

		CHAR formattedEntry[MAX_LOG_ENTRY_SIZE];
		CHAR parameters[MAX_LOG_ENTRY_SIZE];
        Size_t indexWidthLength[MAX_PARAMETER_COUNT * 3];
        int count = this->reader->GetFormatDataCSharpStyle(formattedEntry, MAX_LOG_ENTRY_SIZE, parameters, MAX_LOG_ENTRY_SIZE, indexWidthLength, MAX_PARAMETER_COUNT);

        // TODO: one possible optimization is to map the raw point
        node->TemplateId = this->AddTemplate(gcnew String(formattedEntry));

        
        //PCSTR format = NULL;
        //int count = this->reader->GetFormatDataCStyle(format, parameters, MAX_LOG_ENTRY_SIZE, indexWidthLength, MAX_PARAMETER_COUNT);

        //if (!this->templateMap->TryGetValue(IntPtr((void*)format), node->TemplateId))
        //{
        //    node->TemplateId = this->AddTemplate(gcnew String(format));
        //    this->templateMap->Add(IntPtr((void*)format), node->TemplateId);
        //}

        node->Parameters = gcnew array<Object^>(count);
        int currentPosition = 0;
        for (int i = 0; i < count; i++)
        {
            node->Parameters[i] = gcnew String(parameters, currentPosition, indexWidthLength[i * 3 + 2]);
            currentPosition += indexWidthLength[i * 3 + 2];
        }

		//node->SrcFile = LocalStringPool::Intern(gcnew String(this->reader->getEntryFileName()));

		node->ProcessId = this->reader->getEntryPid();

		//CHAR TS[21];
		//this->reader->getEntryTS(TS);
		//node->TimeStap=gcnew String(TS);

		return node;
	}
	else
	{
		return nullptr;
	}
}

Tuple<DataItemBase^, int>^ CosmosLogSource::LoadItem(IFilter^ filter)
{
	if (this->needsRefresh)
	{
		this->reader->ReFresh();
		this->needsRefresh = false;
	}

	DataItemBase^ node;

	do
	{
		node = this->ReadLogEntry();
		if (node == nullptr)
		{
			this->needsRefresh = true;
			break;
		}
	} while (filter != nullptr && !filter->Match(node, this->Templates[node->TemplateId]));

	return Tuple::Create(node, (int)this->reader->getPersentage());
}
