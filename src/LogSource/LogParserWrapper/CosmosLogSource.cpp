#include "types.h"
#include "CosmosLogSource.h"
#include "CosmosLogFile.h"
#include <vcclr.h>

using namespace LogFlow::DataModel;
using namespace System::IO;
using namespace System::Collections::Generic;

CosmosLogSource::CosmosLogSource(String^ initializationData, LogSourceProperties^ properties) : CosmosLogSourceBase(properties)
{
    auto filePaths = initializationData->Split(gcnew array<wchar_t>(1) { ',' }, StringSplitOptions::RemoveEmptyEntries);
    int fileCount = filePaths->Length;
    this->LogFiles = gcnew List<CosmosLogFileBase^>(fileCount);

    for (int i = 0; i < fileCount; i++)
    {
        this->LogFiles->Add(gcnew CosmosLogFile(filePaths[i]));
    }

    if (fileCount > 1)
    {
        this->InnerGroupFilters = gcnew List<IFilter^>(fileCount);
        for (int i = 0; i < fileCount; i++)
        {
            this->InnerGroupFilters->Add(gcnew FileFilter(this->LogFiles[i]->FileName, i));
        }
    }
}
