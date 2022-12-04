#pragma once

#include "CoreMinimal.h"
#include "MirageUtility.generated.h"

const FString SLASH = FString(TEXT("/"));
const FString API_DEVELOPMENT_URL		= FString(TEXT("https://demo-ue-service.game.ankr.com/"));
const FString API_PRODUCTION_URL		= FString(TEXT("https://ue-service.game.ankr.com/"));

const FString API_AD_DEVELOPMENT_URL    = FString(TEXT("http://45.77.189.28:5001/"));
const FString API_AD_PRODUCTION_URL     = FString(TEXT("https://ads.game.ankr.com/"));

const FString API_STATISTICS_URL        = FString(TEXT("https://usage-stats.game.ankr.com/"));

const FString CONTENT_TYPE_KEY			= FString(TEXT("Content-Type"));
const FString CONTENT_TYPE_VALUE		= FString(TEXT("application/json"));

const FString ENDPOINT_PING				= FString(TEXT("ping"));
const FString ENDPOINT_CONNECT			= FString(TEXT("connect"));
const FString ENDPOINT_WALLET_INFO		= FString(TEXT("wallet/info"));
const FString ENDPOINT_ABI				= FString(TEXT("abi"));
const FString ENDPOINT_SEND_TRANSACTION = FString(TEXT("send/transaction"));
const FString ENDPOINT_RESULT			= FString(TEXT("result"));
const FString ENDPOINT_CALL_METHOD		= FString(TEXT("call/method"));
const FString ENDPOINT_SIGN_MESSAGE		= FString(TEXT("sign/message"));
const FString ENDPOINT_VERIFY_MESSAGE	= FString(TEXT("verify/message"));

const FString ENDPOINT_START_SESSION	= FString(TEXT("start"));
const FString ENDPOINT_AD				= FString(TEXT("ad"));

const FString ENDPOINT_STATS_COLLECT    = FString(TEXT("collect"));

static FString LastRequest;
static bool IsDevelopment;

static FString DeviceID;
static FString Session;
static FString WalletConnectDeepLink;
static bool NeedLogin;
static TArray<FString> Accounts;
static int ChainID;

static FString AdvertisementAppID;
static FString Language;

UCLASS()
class MIRAGESDK_API UMirageUtility : public UObject
{
	GENERATED_BODY()

public:
	
	static void SetDevelopment(bool _value);
	static FString GetUrl();
	static FString GetAdUrl();
	static FString GetStatUrl();
	static FString GetLastRequest();
	static void SetLastRequest(FString _lastRequest);

	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static FString GetDeviceID();
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static void SetDeviceID(FString _deviceId);
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static FString GetSession();
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static void SetSession(FString _session);
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static FString GetWalletConnectDeepLink();
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static void SetWalletConnectDeepLink(FString _walletConnectDeepLink);
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static bool GetNeedLogin();
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static void SetNeedLogin(bool _needLogin);
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static void AddWalletAddress(FString _address);
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static FString GetWalletAddress();
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static int GetChainID();
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static void SetChainID(int _chainId);	
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static FString GetAdvertisementAppID();
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static void SetAdvertisementAppID(FString _appId);
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static FString GetLanguage();
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static void SetLanguage(FString _language);
	UFUNCTION(BlueprintCallable, Category = "MirageUtility") static void Reset();

	static std::string FStringToStdString(FString fstring);
	static FString StdStringToFString(std::string stdstring);
};