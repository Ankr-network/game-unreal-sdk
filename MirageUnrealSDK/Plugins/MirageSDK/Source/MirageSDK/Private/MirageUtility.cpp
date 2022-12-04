#include "MirageUtility.h"

void UMirageUtility::SetDevelopment(bool _value)
{
	IsDevelopment = _value;
}

FString UMirageUtility::GetUrl()
{
	return IsDevelopment ? API_DEVELOPMENT_URL : API_PRODUCTION_URL;
}

FString UMirageUtility::GetAdUrl()
{
	return IsDevelopment ? API_AD_DEVELOPMENT_URL : API_AD_PRODUCTION_URL;
}
FString UMirageUtility::GetStatUrl()
{
	return API_STATISTICS_URL;
}

FString UMirageUtility::GetLastRequest()
{
	return LastRequest;
}

void UMirageUtility::SetLastRequest(FString _lastRequest)
{
	LastRequest = _lastRequest;
}

FString UMirageUtility::GetDeviceID()
{
	return DeviceID;
}
void UMirageUtility::SetDeviceID(FString _deviceId)
{
	DeviceID = _deviceId;
}

FString UMirageUtility::GetSession()
{
	return Session;
}
void UMirageUtility::SetSession(FString _session)
{
	Session = _session;
}

FString UMirageUtility::GetWalletConnectDeepLink()
{
	return WalletConnectDeepLink;
}
void UMirageUtility::SetWalletConnectDeepLink(FString _walletConnectDeepLink)
{
	WalletConnectDeepLink = _walletConnectDeepLink;
}

bool UMirageUtility::GetNeedLogin()
{
	return NeedLogin;
}
void UMirageUtility::SetNeedLogin(bool _needLogin)
{
	NeedLogin = _needLogin;
}

void UMirageUtility::AddWalletAddress(FString _address)
{
	if (!Accounts.Contains(_address))
	{
		Accounts.Add(_address);
	}
}
FString UMirageUtility::GetWalletAddress()
{
	if (Accounts.Num() > 0)
	{
		return Accounts[0];
	}
	
	return FString("");
}

int UMirageUtility::GetChainID()
{
	return ChainID;
}

void UMirageUtility::SetChainID(int _chainId)
{
	ChainID = _chainId;
}

FString UMirageUtility::GetAdvertisementAppID()
{
	return AdvertisementAppID;
}
void UMirageUtility::SetAdvertisementAppID(FString _appId)
{
	AdvertisementAppID = _appId;
}
FString UMirageUtility::GetLanguage()
{
	return Language;
}
void UMirageUtility::SetLanguage(FString _language)
{
	Language = _language;
}

void UMirageUtility::Reset()
{
	LastRequest = FString("");
	DeviceID = FString("");
	Session = FString("");
	WalletConnectDeepLink = FString("");
	NeedLogin = true;
	Accounts.Reset();
	ChainID = -1;
}

std::string UMirageUtility::FStringToStdString(FString fstring)
{
	std::string conversion = std::string(TCHAR_TO_UTF8(*fstring));

	return conversion;
}
FString UMirageUtility::StdStringToFString(std::string stdstring)
{
	FString conversion(stdstring.c_str());

	return conversion;
}