#include "types.h"
#include "BinaryLogReaderWrapper.h"
#include <vcclr.h>

using namespace System::Diagnostics;
using namespace System::IO;
using namespace LogFlow::DataModel;
using namespace LogFlow::DataModel::Algorithm;

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
    LogFlow::DataModel::LogLevels::Verbose,  // 0 debug
    LogFlow::DataModel::LogLevels::Info,     // 1 info
    LogFlow::DataModel::LogLevels::Detail,   // 2 status
    LogFlow::DataModel::LogLevels::Warning,
    LogFlow::DataModel::LogLevels::Error,    // 4
    LogFlow::DataModel::LogLevels::Critical, // 5 alert
    LogFlow::DataModel::LogLevels::Critical, // 6 assert
    LogFlow::DataModel::LogLevels::Verbose,  // 7 event
    LogFlow::DataModel::LogLevels::Verbose,  // 8 reserved
    LogFlow::DataModel::LogLevels::Verbose,  // 9 reserved
    LogFlow::DataModel::LogLevels::Verbose,  // 10 Perf
    LogFlow::DataModel::LogLevels::Verbose,  // 11 max
};

FullCosmosDataItem BinaryLogReaderWrapper::ReadItem()
{
    bool cstyle = 0;
    DWORD err;
    if ((err = this->reader->MoveNext()) == NO_ERROR)
    {
        auto node = gcnew CosmosDataItem();

        node->Level = Levels[this->reader->GetLoggingLevelI()];
        node->Time = DateTime::FromFileTime(this->reader->getEntryTime()).ToUniversalTime();
        node->ThreadId = this->reader->getEntryTid();
        node->ProcessId = this->reader->getEntryPid();

        if (!cstyle)
        {
            CHAR formattedEntry[MAX_LOG_ENTRY_SIZE];
            CHAR parameters[MAX_LOG_ENTRY_SIZE];
            size_t indexWidthLength[MAX_PARAMETER_COUNT * 3];
            // this method is the second fast one, it takes 5.4s for a sample 4 file data.
            int count = (int)this->reader->GetFormatDataCSharpStyle(formattedEntry, MAX_LOG_ENTRY_SIZE, parameters, MAX_LOG_ENTRY_SIZE, indexWidthLength, MAX_PARAMETER_COUNT);

            node->Parameters = gcnew array<String^>(count);
            size_t currentPosition = 0;
            for (int i = 0; i < count; i++)
            {
                // plus new string it takes 6.5s, 572M
                // plus string.Intern it takes 8.1s, 142M
                // plus the LocalStringPool, it takes 9.7s, 155M mem
//                node->Parameters[i] = LocalStringPool::Intern(gcnew String(parameters, currentPosition, indexWidthLength[i * 3 + 2]));
                node->Parameters[i] = gcnew String(parameters, (int)currentPosition, (int)indexWidthLength[i * 3 + 2]);
                currentPosition += indexWidthLength[i * 3 + 2];
            }

            auto guid = this->reader->GetEntryActivityId();
            Guid g = Guid(guid.Data1, guid.Data2, guid.Data3,
                guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
            
            return FullCosmosDataItem(node, gcnew String(formattedEntry), g, (int)this->reader->getPersentage());
        }
        else
        {
            // c style
            CHAR buffer[MAX_LOG_ENTRY_SIZE];
            StringToken tokens[MAX_PARAMETER_COUNT];

            // This is the fastest way, for a sample data, this takes about 4.7s for 4 files.
            int count = (int)this->reader->GetFormatDataCStyle(buffer, MAX_LOG_ENTRY_SIZE, tokens, MAX_PARAMETER_COUNT);

            node->Parameters = gcnew array<String^>(count);

            for (int i = 0; i < count; i++)
            {
                bool isInBuffer = (tokens[i].Pointer.Single >= buffer && tokens[i].Pointer.Single < buffer + MAX_LOG_ENTRY_SIZE);

                // plus new strings, it takes 6.6s, why? maybe the string count is larger.
                // plus string.Intern, it takes 7.8s.
                // plus LocalStringPool, it takes 25s.
                String^ str = tokens[i].IsUnicode ?
                    gcnew String(tokens[i].Pointer.Wide, 0, tokens[i].Length) :
                    gcnew String(tokens[i].Pointer.Single, 0, tokens[i].Length);

                node->Parameters[i] = str;
            }

            auto guid = this->reader->GetEntryActivityId();
            Guid g = Guid(guid.Data1, guid.Data2, guid.Data3,
                guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
            
            // this implementation doesn't have templates, but just concated strings.
            return FullCosmosDataItem(node, (String^)node->Parameters[0], g, (int)this->reader->getPersentage());
        }

        //node->SrcFile = LocalStringPool::Intern(gcnew String(this->reader->getEntryFileName()));


        //CHAR TS[21];
        //this->reader->getEntryTS(TS);
        //node->TimeStap=gcnew String(TS);
    }
    else
    {
        return FullCosmosDataItem(nullptr, nullptr, Guid::Empty, 100);
    }
}

void BinaryLogReaderWrapper::Refresh()
{
    this->reader->ReFresh();
}

