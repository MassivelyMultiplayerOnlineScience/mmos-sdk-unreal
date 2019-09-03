#include "Classifications.h"
#include "../Api.h"

namespace MMOS {
	namespace SDK {
		namespace V2 {
			Classifications::Classifications(Api* api) : api(api) {
			}
			Classifications::~Classifications() {
				api = NULL;
			}
			TSharedPtr<FJsonObject> Classifications::Create(TSharedPtr<FJsonObject> body) {
				FString path = "/classifications";
				FString method = "POST";
				TArray<int> expectedStatusCodes { 201 };

				body->SetStringField("game", api->GetGame());

				return api->Call(path, method, body, expectedStatusCodes);
			}
		}
	}
}