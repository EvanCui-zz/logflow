#pragma once

#include "BinaryLogReader.h"

using namespace LogFlow::DataModel;
using namespace System::Collections::Generic;
using namespace System;

namespace LogFlow {
	namespace DataModel {

		// could add a DataItemBase derived class to extend the cosmos specific columns
		public ref class CosmosLogSource : public CosmosLogSourceBase<DataItemBase^>
		{
		public:
			CosmosLogSource(String^ initializationData);
			~CosmosLogSource();
			Tuple<DataItemBase^, int>^ LoadItem(IFilter^ filter) override;

        protected:
            !CosmosLogSource();

		private:
			DataItemBase^ ReadLogEntry();

			BinaryLogReader *reader;
			bool needsRefresh = true;
		};
	}
}