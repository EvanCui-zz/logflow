#include "Stdafx.h"
#include "types.h"
#include "CosmosLogSource.h"
#include "CosmosLogFile.h"
#include <vcclr.h>

using namespace LogFlow::DataModel;
using namespace System::IO;
using namespace System::Collections::Generic;

CosmosLogSource::CosmosLogSource(String^ initializationData)
{
    auto filePaths = initializationData->Split(gcnew array<wchar_t>(1) { ',' }, StringSplitOptions::RemoveEmptyEntries);
    int fileCount = filePaths->Length;
    this->logFiles = gcnew List<CosmosLogFileBase^>(fileCount);

    for (int i = 0; i < fileCount; i++)
    {
        this->logFiles->Add(gcnew CosmosLogFile(filePaths[i]));
    }

    if (fileCount > 1)
    {
        //    this->InnerGroupData = gcnew List<KeyValuePair<String^, LogFlow::DataModel::InnerGroupData^>>(fileCount);
        this->InnerGroupFilters = gcnew List<IFilter^>(fileCount);
        for (int i = 0; i < fileCount; i++)
        {
            this->InnerGroupFilters->Add(gcnew FileFilter(this->logFiles[i]->FileName, i));
            //   this->InnerGroupData->Add(KeyValuePair<String^, LogFlow::DataModel::InnerGroupData^>(filePaths[i], gcnew LogFlow::DataModel::InnerGroupData()));
        }
    }
}
