#ifndef IAUTHENTICATION_H_
#define IAUTHENTICATION_H_

#include "CoreMinimal.h"

namespace MMOS {
	namespace SDK {
		class IAuthentication {
		public:
			virtual TMap<FString, FString> prepareHeaders(FString key, FString secret, FString method, FString path, FString data) = 0;
			virtual ~IAuthentication() {};
		};
	}
}


#endif