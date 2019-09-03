#ifndef PLAYERS_H_
#define PLAYERS_H_

#include "Runtime/Json/Public/Dom/JsonObject.h"

namespace MMOS {
	namespace SDK {
		class Api; //forward declaration
	}
}

namespace MMOS {
	namespace SDK {
		namespace V2 {
			class Players {
				private:
					Api* api;
				public:
					Players(Api* api);
					~Players();
					TSharedPtr<FJsonObject> Get(FString playerCode, FString project);
					TSharedPtr<FJsonObject> CreateTask(FString playerCode, TSharedPtr<FJsonObject> body);
			};
		}
	}
}

#endif