#pragma once

#include "BinaryLogReader.h"

using namespace System;
using namespace System::Collections::Generic;

namespace LogFlow {
    namespace DataModel {
        public ref class BinaryLogReaderWrapper : BinaryLogReaderWrapperBase
        {
        public:
            BinaryLogReaderWrapper(String^ fileName);
            ~BinaryLogReaderWrapper();

            FullCosmosDataItem^ ReadItem() override;
            bool Refresh() override;
            void Reset() override {}
            double GetPercent() override { return this->reader->getPersentage(); }

        protected:
            !BinaryLogReaderWrapper();

        private:
            String^ Intern(String^ str) { return this->stringPool->Intern(str); }

            BinaryLogReader *reader;
            Algorithm::HashTablePool^ stringPool;
            bool needsRefresh = true;
        };
    }
}
