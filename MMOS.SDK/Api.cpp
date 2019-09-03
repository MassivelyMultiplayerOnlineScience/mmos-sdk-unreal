#include "Api.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"

#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "Engine.h"

namespace MMOS {
	namespace SDK {

		Api::ApiV2::ApiV2(Api* api) {
			players = new V2::Players(api);
			classifications = new V2::Classifications(api);
			authentication = new V2::Authentication();
		};

		Api::ApiV2::~ApiV2() {
			delete classifications;
			delete players;
			delete authentication;
			classifications = NULL;
			players = NULL;
			authentication = NULL;
		};

		Api::Api(ApiConfig config) : Config(config), Game(config.Game)
		{
			UE_LOG(LogTemp, Warning, TEXT("MMOS API CONSTRUCTOR"), );
			HostUrl = config.Protocol + "://" + config.Host + ":" + config.Port;
				client = &FHttpModule::Get();
				switch (Config.Version) {
				case ApiVersion::V2:
					V2 = new ApiV2(this);
					Authentication = V2->authentication;
					break;
				}
		};

		Api::~Api()
		{
			UE_LOG(LogTemp, Warning, TEXT("MMOS API DESTRUCTOR"), );
			//delete client; No need, Unreal handles this
			delete V2;
			client = NULL;
			V2 = NULL;
			Authentication = NULL;

		}

		TSharedPtr<FJsonObject> Api::Info() {
			FString path = "/";
			FString method = "GET";
			TArray<int> expectedStatusCodes{ 200 };

			TSharedPtr<FJsonObject> JsonObject;
			return Call(path, method, JsonObject, expectedStatusCodes);
		};
		TSharedPtr<FJsonObject> Api::Call(FString path, FString method, TSharedPtr<FJsonObject> body, TArray<int> expectedStatusCodes) {
			//validate state in MMOSResponse?
			FString data = "{}";
			
			if (body.IsValid()) {
				json_toString(body, &data);
			}
			TMap<FString, FString> headers = Authentication->prepareHeaders(Config.Key, Config.Secret, method, path, data);

			this->LastResponse = { 0, expectedStatusCodes, FString(), MakeShareable(new FJsonObject()), false, false };

			TSharedRef<IHttpRequest> Request = this->client->CreateRequest();
			Request->OnProcessRequestComplete().BindRaw(this, &Api::OnProcessRequestComplete);
			Request->SetURL(HostUrl + path);

			for (TMap<FString, FString>::TConstIterator It(headers); It; ++It) {
				Request->SetHeader(It.Key(), It.Value());
			}

			if (method.Equals(TEXT("GET"))) {
				Request->SetVerb("GET");
			} else if (method.Equals(TEXT("POST"))) {
				Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
				Request->SetVerb("POST");
				Request->SetContentAsString(data);
			}

			Request->ProcessRequest();

			//response = HandleResponse(client->request(req), expectedStatusCodes);

			return NULL;
		};

		FString Api::GetGame() {
			return Game;
		};

		void Api::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {

			if (!bWasSuccessful)
			{
				UE_LOG(LogTemp, Error, TEXT("Request failed: %s"), *Request->GetURL());

				// Broadcast the result event
				//OnRequestFail.Broadcast();

				return;
			}

			LastResponse.ResponseString = Response->GetContentAsString();
			LastResponse.StatusCode = Response->GetResponseCode();
			LastResponse.ResponseJson->SetNumberField("statusCode", LastResponse.StatusCode);

			// Log response state

			TSharedPtr<FJsonObject> JsonResult = MakeShareable(new FJsonObject);
			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(LastResponse.ResponseString);
			FJsonSerializer::Deserialize(JsonReader, JsonResult);

			if (JsonResult.IsValid()) {
				LastResponse.ResponseJson->SetObjectField(TEXT("body"), JsonResult);
			}

			if (!LastResponse.expectedStatusCodes.IsValidIndex(0) || LastResponse.expectedStatusCodes.Find(LastResponse.StatusCode)) {
				LastResponse.IsSuccess = true;
			}

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, 
				FString::Printf(TEXT("MMOS Request completed. Response (%d): %s"), LastResponse.StatusCode, *Response->GetContentAsString()));

			UE_LOG(LogTemp, Warning, TEXT("MMOS Request completed. Response (%d): %s"), LastResponse.StatusCode, *Response->GetContentAsString());

			LastResponse.IsCompleted = true;

			//OnRequestComplete.Broadcast();

		};

		TSharedPtr<FJsonObject> Api::HandleResponse(FHttpResponsePtr Response, TArray<int> expectedStatusCodes) {

			//response.wait();
			//web::http::http_response result = response.get();

			//int statusCode = result.status_code();
			TSharedPtr<FJsonObject> responseJson;
			//responseJson[U("statusCode")] = TSharedPtr<FJsonObject>(statusCode);
			//responseJson[U("body")] = result.extract_json().get();

			////LOG
			////std::cout << "Received response status code: " << statusCode << std::endl;
			////utility::stringstream_t logStream;
			////responseJson.serialize(logStream);
			//
			//if (expectedStatusCodes.empty() || std::find(expectedStatusCodes.begin(), expectedStatusCodes.end(), statusCode) != expectedStatusCodes.end()) {
				return responseJson;
			//}
			//else {
			//	utility::stringstream_t stream;
			//	responseJson.serialize(stream);
			//	throw stream.str();
			//}

		};

		void Api::json_toString(TSharedPtr<FJsonObject> json, FString* OutputString) {
			TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(OutputString);
			FJsonSerializer::Serialize(json.ToSharedRef(), Writer);
		}

		void Api::string_toJSON(FString JsonString, TSharedPtr<FJsonObject> JsonObject) {
			TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(JsonString);
			FJsonSerializer::Deserialize(Reader, JsonObject);
		}
	}
}