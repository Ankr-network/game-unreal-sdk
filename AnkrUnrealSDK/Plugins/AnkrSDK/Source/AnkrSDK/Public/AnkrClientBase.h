#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnkrDelegates.h"
#include "AnkrClientBase.generated.h"

/// UAnkrClientBase is an ANKR class that provides helper functions to send GET or POST requests asynchronously.
UCLASS(Blueprintable, BlueprintType)
class ANKRSDK_API UAnkrClientBase : public UObject
{
	GENERATED_BODY()

public:

	/// SendRequest function is used to send GET or POST request and the response will be received as a JsonObject. 
	///
	/// The function requires parameters described below and returns nothing.\n
	/// Inside the function, A Get or POST request is sent to the specified URL. 
	///
	/// @param _url The URL to which the request will be sent.
	/// @param _verb Whether the request is a GET or POST request.
	/// @param _payload The payload data associated with the request
	/// @param _function The function to be executed when a response is received.
	/// @param _backgroundTask Determines if the request to be processed as a background task.
	
	void SendRequest(FString _url, FString _verb, FString _payload, TFunctionRef<void(const TArray<uint8>, const FString, TSharedPtr<FJsonObject>)> _function, bool _backgroundTask);

	/// SendRequest function is used to send GET or POST request and the response will be received as a JsonObject. 
	///
	/// The function requires parameters described below and returns nothing.\n
	/// Inside the function, A Get or POST request is sent to the specified URL. 
	///
	/// @param _url The URL to which the request will be sent.
	/// @param _verb Whether the request is a GET or POST request.
	/// @param _payload The payload data associated with the request
	/// @param _function The function to be executed when a response is received.
	/// @param _callback The callback delegate to be executed when a response is received.
	/// @param _backgroundTask Determines if the request to be processed as a background task.
	void SendRequest(FString _url, FString _verb, FString _payload, TFunctionRef<void(const TArray<uint8>, const FString, const FAnkrCallCompleteDynamicDelegate&, TSharedPtr<FJsonObject>)> _function, const FAnkrCallCompleteDynamicDelegate& _callback, bool _backgroundTask);

	/// SendRequest function is used to send GET or POST request and the response will be received as a JsonObject. 
	///
	/// The function requires parameters described below and returns nothing.\n
	/// Inside the function, A Get or POST request is sent to the specified URL. 
	///
	/// @param _url The URL to which the request will be sent.
	/// @param _verb Whether the request is a GET or POST request.
	/// @param _payload The payload data associated with the request
	/// @param _function The function to be executed when a response is received.
	/// @param _callback The callback delegate to be executed when an advertisement is received.
	/// @param _backgroundTask Determines if the request to be processed as a background task.
	void SendRequest(FString _url, FString _verb, FString _payload, TFunctionRef<void(const TArray<uint8>, const FString, const FAdvertisementReceivedDelegate&, TSharedPtr<FJsonObject>)> _function, const FAdvertisementReceivedDelegate& _callback, bool _backgroundTask);

	/// SendRequest function is used to send GET or POST request and the response will be received as a JsonObject. 
	///
	/// The function requires parameters described below and returns nothing.\n
	/// Inside the function, A Get or POST request is sent to the specified URL. 
	///
	/// @param _url The URL to which the request will be sent.
	/// @param _verb Whether the request is a GET or POST request.
	/// @param _payload The payload data associated with the request
	/// @param _function The function to be executed when a response is received.
	/// @param _callback The callback delegate to be executed when the advertisement content is downloaded.
	/// @param _backgroundTask Determines if the request to be processed as a background task.
	void SendRequest(FString _url, TFunctionRef<void(const TArray<uint8>, const FString, FAdvertisementDataStructure data, const FAdvertisementVideoAdDownloadDelegate&)> _function, FAdvertisementDataStructure data, const FAdvertisementVideoAdDownloadDelegate& _callback, bool _backgroundTask);

};
