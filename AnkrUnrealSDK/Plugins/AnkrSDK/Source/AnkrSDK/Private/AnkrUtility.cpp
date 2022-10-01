#include "AnkrUtility.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Serialization/JsonSerializer.h"

void UAnkrUtility::SetDevelopment(bool _value)
{
	IsDevelopment = _value;
}

FString UAnkrUtility::GetUrl()
{
	return IsDevelopment ? API_DEVELOPMENT_URL : API_PRODUCTION_URL;
}
FString UAnkrUtility::GetAdsAppID()
{
	return ADS_APP_ID;
}
FString UAnkrUtility::GetAdUrl()
{
	return IsDevelopment ? API_AD_DEVELOPMENT_URL : API_AD_PRODUCTION_URL;
}
FString UAnkrUtility::GetStatUrl()
{
	return API_STATISTICS_URL;
}

FString UAnkrUtility::GetLastRequest()
{
	return LastRequest;
}

void UAnkrUtility::SetLastRequest(FString _lastRequest)
{
	LastRequest = _lastRequest;
}

FString UAnkrUtility::GetDefaultERC20()
{
	return DefaultERC20;
}
FString UAnkrUtility::GetDefaultERC721()
{
	return DefaultERC721;
}
FString UAnkrUtility::GetDefaultERC1155()
{
	return DefaultERC1155;
}

FString UAnkrUtility::GetDeviceID()
{
	return DeviceID;
}
void UAnkrUtility::SetDeviceID(FString _deviceId)
{
	DeviceID = _deviceId;
}

FString UAnkrUtility::GetSession()
{
	return Session;
}
void UAnkrUtility::SetSession(FString _session)
{
	Session = _session;
}

FString UAnkrUtility::GetWalletConnectDeepLink()
{
	return WalletConnectDeepLink;
}
void UAnkrUtility::SetWalletConnectDeepLink(FString _walletConnectDeepLink)
{
	WalletConnectDeepLink = _walletConnectDeepLink;
}

bool UAnkrUtility::GetNeedLogin()
{
	return NeedLogin;
}
void UAnkrUtility::SetNeedLogin(bool _needLogin)
{
	NeedLogin = _needLogin;
}

void UAnkrUtility::AddWalletAddress(FString _address)
{
	if (!Accounts.Contains(_address))
	{
		Accounts.Add(_address);
	}
}
FString UAnkrUtility::GetWalletAddress()
{
	if (Accounts.Num() > 0)
	{
		return Accounts[0];
	}
	
	return FString("");
}

int UAnkrUtility::GetChainID()
{
	return ChainID;
}

void UAnkrUtility::SetChainID(int _chainId)
{
	ChainID = _chainId;
}