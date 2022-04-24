#pragma once

#include "CoreMinimal.h"

const FString API_BASE_URL				= FString(TEXT("http://45.77.189.28:5000/"));
const FString API_AD_URL				= FString(TEXT("http://45.77.189.28:5001/"));
const FString USER_AGENT_KEY			= FString(TEXT("User-Agent"));
const FString USER_AGENT_VALUE			= FString(TEXT("X-MirageSDK-Agent"));
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

class ANKRSDK_API AnkrUtility
{

public:

	static FString GetLastRequest();
	static void SetLastRequest(FString _lastRequest);
};