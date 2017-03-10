#pragma once

using namespace System;
using namespace System::Collections::Generic;

using namespace LogFlow::DataModel;

namespace LogFlow {
    namespace DataModel {

        // could add a DataItemStruct derived class to extend the cosmos specific columns
        public ref class CosmosLogSource : CosmosLogSourceBase
        {
        public:
            CosmosLogSource(String^ initializationData);
        };
    }
}