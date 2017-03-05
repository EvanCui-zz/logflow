#include "Stdafx.h"
#include "BinaryLogReaderWrapper.h"
#include <vcclr.h>

using namespace System::IO;
using namespace LogFlow::DataModel;

BinaryLogReaderWrapper::BinaryLogReaderWrapper(String^ filename)
{
    pin_ptr<const WCHAR> wch = PtrToStringChars(filename);
    WCHAR wcstring[MAX_PATH];
    wcscpy_s(wcstring, wch);
    this->reader = new BinaryLogReader();
    DWORD err = this->reader->OpenReader(wcstring);
    if (err != NO_ERROR)
    {
        throw gcnew IOException(String::Format("Cannot open file {0}", filename), err);
    }
}

BinaryLogReaderWrapper::!BinaryLogReaderWrapper()
{
    this->reader->CloseReader();
    delete this->reader;
}

BinaryLogReaderWrapper::~BinaryLogReaderWrapper()
{
    this->!BinaryLogReaderWrapper();
}

// Debug,Info,Status,Warning,Error,AppAlert,Assert
static LogFlow::DataModel::LogLevels Levels[] =
{
    LogFlow::DataModel::LogLevels::Verbose,
    LogFlow::DataModel::LogLevels::Info,
    LogFlow::DataModel::LogLevels::Detail,
    LogFlow::DataModel::LogLevels::Warning,
    LogFlow::DataModel::LogLevels::Error,
    LogFlow::DataModel::LogLevels::Critical,
    LogFlow::DataModel::LogLevels::Critical,
};

FullCosmosDataItem BinaryLogReaderWrapper::ReadItem()
{
    DWORD err;
    if ((err = this->reader->MoveNext()) == NO_ERROR)
    {
        auto node = gcnew CosmosDataItem();

        node->Level = Levels[this->reader->GetLoggingLevelI()];
        node->Time = DateTime::FromFileTime(this->reader->getEntryTime()).ToUniversalTime();
        node->ThreadId = this->reader->getEntryTid();

        CHAR formattedEntry[MAX_LOG_ENTRY_SIZE];
        CHAR parameters[MAX_LOG_ENTRY_SIZE];
        size_t indexWidthLength[MAX_PARAMETER_COUNT * 3];
        int count = this->reader->GetFormatDataCSharpStyle(formattedEntry, MAX_LOG_ENTRY_SIZE, parameters, MAX_LOG_ENTRY_SIZE, indexWidthLength, MAX_PARAMETER_COUNT);

        // TODO: one possible optimization is to map the raw point

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

        return FullCosmosDataItem(node, String::Intern(gcnew String(formattedEntry)));
    }
    else
    {
        return FullCosmosDataItem(nullptr, nullptr);
    }
}

void BinaryLogReaderWrapper::Refresh()
{
    this->reader->ReFresh();
}

