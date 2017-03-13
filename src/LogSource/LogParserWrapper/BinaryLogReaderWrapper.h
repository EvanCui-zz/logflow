#pragma once

#include <map>
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

            FullCosmosDataItem ReadItem() override;
            void Refresh() override;
            void Reset() override {}
            double GetPercent() override { return this->reader->getPersentage(); }

        protected:
            !BinaryLogReaderWrapper();

        private:
            BinaryLogReader *reader;
            bool needsRefresh = true;
        };
    }
}
