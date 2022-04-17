#include "AnkrUtility.h"

FString AnkrUtility::GetLastRequest()
{
	return LastRequest;
}

void AnkrUtility::SetLastRequest(FString _lastRequest)
{
	LastRequest = _lastRequest;
}