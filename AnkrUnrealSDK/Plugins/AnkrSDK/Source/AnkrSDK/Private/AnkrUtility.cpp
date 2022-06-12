#include "AnkrUtility.h"
#include "Runtime/Online/HTTP/Public/Http.h"

FString AnkrUtility::GetLastRequest()
{
	return LastRequest;
}

void AnkrUtility::SetLastRequest(FString _lastRequest)
{
	LastRequest = _lastRequest;
}

void AnkrUtility::Process(FString _url, FString _verb, FString _body, TFunctionRef<void(const FString)> response)
{
	FHttpModule* httpModule = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = httpModule->CreateRequest();
#endif

	Request->SetURL(_url);
	Request->SetVerb(_verb);
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString(_body);
	Request->ProcessRequest();
	Request->OnProcessRequestComplete().BindLambda([response](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			response(Response->GetContentAsString());
		});
}