#include "Authentication.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#define UI UI_ST
THIRD_PARTY_INCLUDES_START
#if PLATFORM_WINDOWS
#include <openssl/hmac.h>
#elif PLATFORM_MAC
#elif PLATFORM_LINUX
#endif
THIRD_PARTY_INCLUDES_END
#undef UI


namespace MMOS {
	namespace SDK {
		namespace V2 {

			Authentication::Authentication() {};

			TMap<FString, FString> Authentication::prepareHeaders(FString key, FString secret, FString method, FString path, FString data) {
				TMap<FString, FString> headers;

				uint64_t timestamp_uint = timeSinceEpochMillisec();
				FString timestamp = FString(std::to_string(timestamp_uint).c_str());
				uint64_t nonceLong = floor(rand() * timestamp_uint) + 1;
				FString nonce = FString(std::to_string(nonceLong).c_str());
				FString signature;
				FString contentData = (data.IsEmpty()) ? "{}" : data;
				TArray<FString> contentParts;

				contentParts.Add(MMOS_SIGNING_ALGORITHM);
				contentParts.Add(key);
				contentParts.Add(timestamp);
				contentParts.Add(nonce);
				contentParts.Add(method);
				contentParts.Add(path);
				contentParts.Add(contentData);

				FString contents = FString::Join(contentParts, *CONTENT_SEPARATOR);

				FString keyHash = CreateHmac(TCHAR_TO_UTF8(*timestamp), TCHAR_TO_UTF8(*secret));
				signature = CreateHmac(TCHAR_TO_UTF8(*keyHash), TCHAR_TO_UTF8(*contents));

				headers.Add("X-MMOS-Algorithm", MMOS_SIGNING_ALGORITHM);
				headers.Add("X-MMOS-Credential", key);
				headers.Add("X-MMOS-Timestamp", timestamp);
				headers.Add("X-MMOS-Nonce", nonce);
				headers.Add("X-MMOS-Signature", signature);

				return headers;
			}
			uint64_t Authentication::timeSinceEpochMillisec() {
				using namespace std::chrono;
				return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
			};
			FString Authentication::CreateHmac(std::string key, std::string msg) {
				unsigned char hash[32];

				HMAC_CTX *hmac = HMAC_CTX_new();
				HMAC_Init_ex(hmac, &key[0], key.length(), EVP_sha256(), NULL);
				HMAC_Update(hmac, (unsigned char*)&msg[0], msg.length());
				unsigned int len = 32;
				HMAC_Final(hmac, hash, &len);
				HMAC_CTX_free(hmac);

				std::stringstream ss;
				ss << std::hex << std::setfill('0');
				for (unsigned int i = 0; i < len; i++)
				{
					ss << std::hex << std::setw(2) << (unsigned int)hash[i];
				}

				return FString(ss.str().c_str());
			};
		}
	}
}