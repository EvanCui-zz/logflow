#include "Stdafx.h"
#include "types.h"
#include "CosmosLogSource.h"
#include <vcclr.h>

using namespace LogFlow::DataModel;

CosmosLogSource::CosmosLogSource(String^ initializationData)
{
	this->reader = new BinaryLogReader();

	pin_ptr<const WCHAR> wch = PtrToStringChars(initializationData);
	WCHAR wcstring[MAX_PATH];
	wcscpy_s(wcstring, wch);
	DWORD err = this->reader->OpenReader(wcstring);
	if (err != NO_ERROR)
	{
		return;
	}
}

CosmosLogSource::~CosmosLogSource()
{
	this->reader->CloseReader();
	delete this->reader;
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

DataItemBase^ CosmosLogSource::ReadLogEntry()
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
		//this->reader->getFormatedText(formattedEntry);
		//node->Content=gcnew String(formattedEntry);
        node->TemplateId = this->AddTemplate(gcnew String(formattedEntry));
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
