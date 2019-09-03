[Site](http://mmos.ch/) |
[Developer Portal](https://devportal.mmos.ch/) |
[Twitter](https://twitter.com/the_mmos) |

# MMOS SDK - Unreal Engine C++ edition

The MMOS SDK gives easy access to the MMOS API by providing an abstraction layer and encapsulating the authentication modules.

## Installation

* Open your unreal engine 4 project folder ("myproject" for the sake of this guide)
* copy the MMOS folder into myproject/source/myproject
* Add project dependencies to myproject/source/myproject/myproject.build.cs as follows 
```c++
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Http", "Json", "JsonUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] { "OpenSSL" });
```
* double check that your Unreal Engine installation has the OpenSSL binaries included


## Using the MMOS SDK inside unreal

In a standard Visual Studio C++ project you can use the syntax below.

Make sure your includes point to the correct file location.

```c++
#include "MMOS.SDK/Api.h"

MMOS::SDK::Api* api;

Api::ApiConfig Config = {
	"myKey",
	"mySecret",
	"myGame",
	"https",
	"api.depo.mmos.blue",
	"443",
	Api::ApiVersion::V2
};

api = new Api(Config);

FString Project = "unige-exoplanet";
FString Player = "testPlayer";
FString PlayerAccountCode = "testAccount";

TSharedPtr<FJsonObject> bodyTask = MakeShareable(new FJsonObject);

TArray< TSharedPtr<FJsonValue> > projects;
projects.Add(MakeShareable(new FJsonValueString(Project)));
bodyTask->SetArrayField("projects", projects);

TSharedPtr<FJsonObject> playerObj = MakeShareable(new FJsonObject);
playerObj->SetStringField(("accountCode"), PlayerAccountCode);
bodyTask->SetObjectField("player", playerObj);

api->V2->players->CreateTask(Player, bodyTask);


```

Please note the the MMOS.SDK is designed to be a server side component as the API Key it uses is a per game API Key, therefore it needs to be managed on the server side.


## Documentation


API blueprint is available in compiled [html](doc/blueprint/mmos-api-public.html) and [apib](doc/blueprint/mmos-api-public.apib) format.


## Authentication

Please see the [authentication docs](api-hmac-authentication.md) for details.

## Acknowledgments

The GAPARS project has received funding from the European Union’s Horizon 2020 research and innovation programme under grant agreement Nr 732703

![EU flag](https://github.com/MassivelyMultiplayerOnlineScience/mmos-sdk-csharp/raw/master/MMOS.SDK/doc/logo/eu.jpg)