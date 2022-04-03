#pragma once

#include "CoreMinimal.h"

#define BASE_URL								"http://45.77.189.28:5000/"

#define ENDPOINT_PING							"ping"
#define ENDPOINT_CONNECT						"connect"
#define ENDPOINT_WALLET_INFO					"wallet/info"
#define ENDPOINT_ABI							"abi"
#define ENDPOINT_SEND_TRANSACTION				"send/transaction"
#define ENDPOINT_RESULT							"result"
#define ENDPOINT_CALL_METHOD					"call/method"
#define ENDPOINT_SIGN_MESSAGE					"sign/message"
#define ENDPOINT_VERIFY_MESSAGE					"verify/message"

#define METHOD_GET_TOKEN_DETAILS				"getTokenDetails"
#define METHOD_UPDATE_TOKEN_WITH_SIGNED_MESSAGE "updateTokenWithSignedMessage"

#define METHOD_MINT_BATCH						"mintBatch"
#define METHOD_SAFE_MINT						"safeMint"
#define METHOD_MINT								"mint"
#define METHOD_SET_APPROVAL_FOR_ALL				"setApprovalForAll"
#define METHOD_BALANCE_OF						"balanceOf"
#define METHOD_BALANCE_OF_BATCH					"balanceOfBatch"
#define METHOD_TOKEN_OF_OWNER_BY_INDEX			"tokenOfOwnerByIndex"
#define METHOD_CHANGE_HAT						"changeHat"
#define METHOD_GET_HAT							"getHat"