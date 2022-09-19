#include "AnkrClientBase.h"
#include "AnkrUtility.h"

void UAnkrClientBase::SendRequest(FString _url, FString _verb, FString _payload, TFunctionRef<void(const TArray<uint8>, const FString, TSharedPtr<FJsonObject>)> _function, bool _backgroundTask)
{
	FHttpModule* httpModule = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = httpModule->CreateRequest();
#endif

	if (_backgroundTask)
	{
		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, _url, _verb, _payload]()
			{
				Request->SetURL(_url);
				Request->SetVerb(_verb);
				Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
				if (!_payload.IsEmpty()) Request->SetContentAsString(_payload);
				Request->ProcessRequest();
			});
	}
	else
	{
		Request->SetURL(_url);
		Request->SetVerb(_verb);
		Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
		if (!_payload.IsEmpty()) Request->SetContentAsString(_payload);
		Request->ProcessRequest();
	}
	Request->OnProcessRequestComplete().BindWeakLambda(this, [this, _url, _function](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const TArray<uint8> bytes = Response->GetContent();
			const FString content = Response->GetContentAsString();

			if (!bWasSuccessful || !Response.IsValid() || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				UE_LOG(LogTemp, Error, TEXT("AnkrClientBase - SendRequest - Request Failed -_url: %s | content: %s"), *_url, *content);
				return;
			}

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			if (!FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				UE_LOG(LogTemp, Error, TEXT("AnkrClientBase - SendRequest - Deserialize Failed -_url: %s | content: %s"), *_url, *content);
				return;
			}

			_function(bytes, content, JsonObject);
		});
}

void UAnkrClientBase::SendRequest(FString _url, FString _verb, FString _payload, TFunctionRef<void(const TArray<uint8>, const FString, const FAnkrCallCompleteDynamicDelegate&, TSharedPtr<FJsonObject>)> _function, const FAnkrCallCompleteDynamicDelegate& _callback, bool _backgroundTask)
{
	FHttpModule* httpModule = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = httpModule->CreateRequest();
#endif

	if (_backgroundTask)
	{
		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, _url, _verb, _payload]()
			{
				Request->SetURL(_url);
				Request->SetVerb(_verb);
				Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
				if (!_payload.IsEmpty()) Request->SetContentAsString(_payload);
				Request->ProcessRequest();
			});
	}
	else
	{
		Request->SetURL(_url);
		Request->SetVerb(_verb);
		Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
		if (!_payload.IsEmpty()) Request->SetContentAsString(_payload);
		Request->ProcessRequest();
	}
	Request->OnProcessRequestComplete().BindWeakLambda(this, [this, _url, _function, _callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const TArray<uint8> bytes = Response->GetContent();
			const FString content = Response->GetContentAsString();

			if (!bWasSuccessful || !Response.IsValid() || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				UE_LOG(LogTemp, Error, TEXT("AnkrClientBase - SendRequest - Request Failed -_url: %s | content: %s"), *_url, *content);
				return;
			}

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			if (!FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				UE_LOG(LogTemp, Error, TEXT("AnkrClientBase - SendRequest - Deserialize Failed -_url: %s | content: %s"), *_url, *content);
				return;
			}
			
			_function(bytes, content, _callback, JsonObject);
		});
}