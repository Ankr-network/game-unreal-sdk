#include "AnkrUtility.h"
#include "Runtime/Online/HTTP/Public/Http.h"

void AnkrUtility::SetDevelopment(bool _value)
{
	IsDevelopment = _value;
}

FString AnkrUtility::GetUrl()
{
	return IsDevelopment ? API_DEVELOPMENT_BASE_URL : API_PRODUCTION_URL;
}

FString AnkrUtility::GetLastRequest()
{
	return LastRequest;
}

void AnkrUtility::SetLastRequest(FString _lastRequest)
{
	LastRequest = _lastRequest;
}