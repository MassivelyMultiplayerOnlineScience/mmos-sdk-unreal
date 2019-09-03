#ifndef CLASSIFICATIONS_H_
#define CLASSIFICATIONS_H_

#include "Runtime/Json/Public/Dom/JsonObject.h"

namespace MMOS {
	namespace SDK {
		class Api; //forward declaration
	}
}

namespace MMOS {
	namespace SDK {
		namespace V2 {
			class Classifications {
				private:
					Api* api;
				public:
					Classifications(Api* api);
					~Classifications();
					TSharedPtr<FJsonObject> Create(TSharedPtr<FJsonObject> body);
			};
		}
	}
}

#endif