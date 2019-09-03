#include "Players.h"
#include "../Api.h"

namespace MMOS {
	namespace SDK {
		namespace V2 {
			Players::Players(Api* api) : api(api) {
			};
			Players::~Players() {
				api = NULL;
			}
			TSharedPtr<FJsonObject> Players::Get(FString playerCode, FString project) {
				FString path = "/games/" + api->GetGame() + "/players/" + playerCode;
				
				FString method = "GET";
				TArray<int> expectedStatusCodes{ 200, 404 };

				//if (!project.empty()) {
				//	path += "?project=" + project;
				//}

				TSharedPtr<FJsonObject> JsonObject;
				return api->Call(path, method, JsonObject, expectedStatusCodes);
				
			};
			TSharedPtr<FJsonObject> Players::CreateTask(FString playerCode, TSharedPtr<FJsonObject> body) {
				FString path = "/games/" + api->GetGame() + "/players/" + playerCode + "/tasks";

				FString method = "POST";
				TArray<int> expectedStatusCodes{ 201 };

				return api->Call(path, method, body, expectedStatusCodes);
			};
		}
	}
}