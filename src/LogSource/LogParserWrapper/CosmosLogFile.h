#pragma once

#include "BinaryLogReaderWrapper.h"

using namespace System;
using namespace System::Collections::Generic;

namespace LogFlow {
    namespace DataModel {
        public ref class CosmosLogFile : public CosmosLogFileBase
        {
        public:
            CosmosLogFile(String^ filename)
            {
				this->filePath = filename;
                this->reader = gcnew BinaryLogReaderWrapper(filename);
            }
        };
    }
}