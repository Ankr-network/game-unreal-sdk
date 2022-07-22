#pragma once

#include "CoreMinimal.h"

const FString SLASH = FString(TEXT("/"));
const FString API_PRODUCTION_URL		= FString(TEXT("https://ue-service.game.ankr.com/"));
const FString API_DEVELOPMENT_BASE_URL  = FString(TEXT("https://demo-ue-service.game.ankr.com/"));
const FString API_AD_URL				= FString(TEXT("http://45.77.189.28:5001/"));
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

static FString LastRequest;
static bool IsDevelopment;

class ANKRSDK_API AnkrUtility
{

public:
	
	static void SetDevelopment(bool _value);
	static FString GetUrl();
	static FString GetLastRequest();
	static void SetLastRequest(FString _lastRequest);
};