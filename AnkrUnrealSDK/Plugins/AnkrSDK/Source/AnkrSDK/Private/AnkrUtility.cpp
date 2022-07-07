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