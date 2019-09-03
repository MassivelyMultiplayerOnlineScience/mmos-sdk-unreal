#ifndef API_H_
#define API_H_

#include "CoreMinimal.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "IAuthentication.h"
#include "V2/Authentication.h"
#include "V2/Classifications.h"
#include "V2/Players.h"

namespace MMOS {
	namespace SDK {
		class Api
		{
		public:
			enum class ApiVersion { V2 = 2 };
			struct ApiV2 {
				V2::Players* players;
				V2::Classifications* classifications;
				IAuthentication* authentication;
				ApiV2(Api* api);
				~ApiV2();
			};
			struct ApiConfig {
				FString Key;
				FString Secret;
				FString Game;
				FString Protocol;
				FString Host;
				FString Port;
				ApiVersion Version;
			};
			struct MMOSResponse {
				int StatusCode;
				TArray<int> expectedStatusCodes;
				FString ResponseString;
				TSharedPtr<FJsonObject> ResponseJson;
				bool IsCompleted;
				bool IsSuccess;
			};

			const ApiVersion CURRENT_VERSION = ApiVersion::V2;
			const FString DEFAULT_PROTOCOL = FString("https");
			const int DEFAULT_PORT = 443;

			Api(ApiConfig config);
			~Api();

			TSharedPtr<FJsonObject> Info();
			TSharedPtr<FJsonObject> Call(FString path, FString method, TSharedPtr<FJsonObject> body, TArray<int> expectedStatusCodes);
			FString GetGame();
			ApiV2* V2;
			void json_toString(TSharedPtr<FJsonObject> json, FString* OutputString);
			void string_toJSON(FString OutputString, TSharedPtr<FJsonObject> json);
		private:
			ApiConfig Config;
			FString Game;
			FString HostUrl;
			IAuthentication* Authentication;
			FHttpModule* client;
			MMOSResponse LastResponse;
			void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
			TSharedPtr<FJsonObject> HandleResponse(FHttpResponsePtr Response, TArray<int> expectedStatusCodes);
		};
	}
}

#endif