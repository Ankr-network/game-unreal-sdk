#include "AnkrUtility.h"
#include "Runtime/Online/HTTP/Public/Http.h"

void AnkrUtility::SetDevelopment(bool _value)
{
	IsDevelopment = _value;
}

FString AnkrUtility::GetUrl()
{
	return IsDevelopment ? API_DEVELOPMENT_URL : API_PRODUCTION_URL;
}
FString AnkrUtility::GetAdUrl()
{
	return IsDevelopment ? API_AD_DEVELOPMENT_URL : API_AD_PRODUCTION_URL;
}
FString AnkrUtility::GetStatUrl()
{
	return API_STATISTICS_URL;
}

FString AnkrUtility::GetLastRequest()
{
	return LastRequest;
}

void AnkrUtility::SetLastRequest(FString _lastRequest)
{
	LastRequest = _lastRequest;
}

FString AnkrUtility::GetDefaultERC20()
{
	return DefaultERC20;
}
FString AnkrUtility::GetDefaultERC721()
{
	return DefaultERC721;
}
FString AnkrUtility::GetDefaultERC1155()
{
	return DefaultERC1155;
}