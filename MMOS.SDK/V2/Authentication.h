#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

#include "../IAuthentication.h"
#include <string>

namespace MMOS {
	namespace SDK {
		namespace V2 {
			class Authentication : public MMOS::SDK::IAuthentication{
			private:
				const FString CONTENT_SEPARATOR = "|";
				const FString MMOS_SIGNING_ALGORITHM = "MMOS1-HMAC-SHA256";
				uint64_t timeSinceEpochMillisec();
				FString CreateHmac(std::string  key, std::string  msg);
			public:
				Authentication();
				TMap<FString, FString> prepareHeaders(FString key, FString secret, FString method, FString path, FString data);
			};
		}
	}
}

#endif