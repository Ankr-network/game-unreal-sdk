#include "LibraryManager.h"
#include <codecvt>

void LibraryManager::Load()
{
	FString dllPath = *FPaths::ProjectPluginsDir() + FString("AnkrSDK/Source/AnkrSDK/Private/Windows/Libraries/AnkrSDKUnrealWindows.dll");
	bool exists = FPaths::FileExists(dllPath);

	HNDL = FPlatformProcess::GetDllHandle(*dllPath);
	if (!HNDL)
	{
		UE_LOG(LogTemp, Warning, TEXT("HNDL could not be loaded."));
		return;
	}

	InitializeFunction      = (InitializeImport)      FPlatformProcess::GetDllExport(HNDL, *FString("Initialize"));
	GetDeviceIdFunction     = (GetDeviceIdImport)     FPlatformProcess::GetDllExport(HNDL, *FString("GetDeviceId"));
	GetQRCodeTextFunction   = (GetQRCodeTextImport)   FPlatformProcess::GetDllExport(HNDL, *FString("GetQRCodeText"));
	GetSessionFunction      = (GetSessionImport)      FPlatformProcess::GetDllExport(HNDL, *FString("GetSession"));
	GetAccountFunction      = (GetAccountImport)      FPlatformProcess::GetDllExport(HNDL, *FString("GetAccount"));
	GetChainIdFunction      = (GetChainIdImport)      FPlatformProcess::GetDllExport(HNDL, *FString("GetChainId"));
	PingFunction		    = (PingImport)            FPlatformProcess::GetDllExport(HNDL, *FString("Ping"));
	ConnectWalletFunction   = (ConnectWalletImport)   FPlatformProcess::GetDllExport(HNDL, *FString("ConnectWallet"));
	GetWalletFunction       = (GetWalletImport)       FPlatformProcess::GetDllExport(HNDL, *FString("GetWallet"));
	SendABIFunction         = (SendABIImport)         FPlatformProcess::GetDllExport(HNDL, *FString("SendABI"));
	SendTransactionFunction = (SendTransactionImport) FPlatformProcess::GetDllExport(HNDL, *FString("SendTransaction"));
	CallMethodFunction      = (CallMethodImport)      FPlatformProcess::GetDllExport(HNDL, *FString("CallMethod"));
	SignMessageFunction     = (SignMessageImport)     FPlatformProcess::GetDllExport(HNDL, *FString("SignMessage"));
	GetResultFunction       = (GetResultImport)       FPlatformProcess::GetDllExport(HNDL, *FString("GetResult"));
	VerifyMessageFunction   = (VerifyMessageImport)   FPlatformProcess::GetDllExport(HNDL, *FString("VerifyMessage"));
}

void LibraryManager::Unload()
{
	if (HNDL)
	{
		InitializeFunction = NULL;
		GetDeviceIdFunction = NULL;
		GetQRCodeTextFunction = NULL;
		GetSessionFunction = NULL;
		GetAccountFunction = NULL;
		GetChainIdFunction = NULL;
		PingFunction = NULL;
		ConnectWalletFunction = NULL;
		GetWalletFunction = NULL;
		SendABIFunction = NULL;
		SendTransactionFunction = NULL;
		CallMethodFunction = NULL;
		SignMessageFunction = NULL;
		GetResultFunction = NULL;
		VerifyMessageFunction = NULL;

		FPlatformProcess::FreeDllHandle(HNDL);
		HNDL = NULL;

	}
}

void LibraryManager::Log(FString _message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *_message);
}

void LibraryManager::Initialize(bool _isDevelopment, FString _device_id)
{
	InitializeFunction(false, TCHAR_TO_UTF8(*_device_id), [](const char* _message) { UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(_message)); });
}
void LibraryManager::Ping()
{
	PingFunction([](bool _success, const char* _data)
		{
			LibraryManager::GetInstance().FlushCall("Ping", _success, _data);
		});
}
void LibraryManager::ConnectWallet(FString _content)
{
	ConnectWalletFunction(TCHAR_TO_UTF8(*_content), [](bool _success, const char* _data)
		{
			LibraryManager::GetInstance().FlushCall("ConnectWallet", _success, _data);
		});
}
void LibraryManager::GetWallet(FString _content)
{
	GetWalletFunction(TCHAR_TO_UTF8(*_content), [](bool _success, const char* _data)
		{
			LibraryManager::GetInstance().FlushCall("GetWallet", _success, _data);
		});
}
void LibraryManager::SendABI(FString _content)
{
	SendABIFunction(TCHAR_TO_UTF8(*_content), [](bool _success, const char* _data)
		{
			LibraryManager::GetInstance().FlushCall("SendABI", _success, _data);
		});
}
void LibraryManager::SendTransaction(FString _content)
{
	SendTransactionFunction(TCHAR_TO_UTF8(*_content), [](bool _success, const char* _data)
		{
			LibraryManager::GetInstance().FlushCall("SendTransaction", _success, _data);
		});
}
void LibraryManager::GetResult(FString _content)
{
	GetResultFunction(TCHAR_TO_UTF8(*_content), [](bool _success, const char* _data)
		{
			LibraryManager::GetInstance().FlushCall("GetResult", _success, _data);
		});
}
void LibraryManager::CallMethod(FString _content)
{
	CallMethodFunction(TCHAR_TO_UTF8(*_content), [](bool _success, const char* _data)
		{
			LibraryManager::GetInstance().FlushCall("CallMethod", _success, _data);
		});
}
void LibraryManager::SignMessage(FString _content)
{
	SignMessageFunction(TCHAR_TO_UTF8(*_content), [](bool _success, const char* _data)
		{
			LibraryManager::GetInstance().FlushCall("SignMessage", _success, _data);
		});
}
void LibraryManager::GetSignature(FString _content)
{
	GetResultFunction(TCHAR_TO_UTF8(*_content), [](bool _success, const char* _data)
		{
			LibraryManager::GetInstance().FlushCall("GetSignature", _success, _data);
		});
}
void LibraryManager::VerifyMessage(FString _content)
{
	VerifyMessageFunction(TCHAR_TO_UTF8(*_content), [](bool _success, const char* _data)
		{
			LibraryManager::GetInstance().FlushCall("VerifyMessage", _success, _data);
		});
}

int LibraryManager::GetGlobalCallIndex()
{
	GlobalCallIndex++; if (GlobalCallIndex > INT_MAX) GlobalCallIndex = 0;
	return GlobalCallIndex;
}
bool LibraryManager::AddCall(const char* _sender, const FAnkrCallCompleteDynamicDelegate _callComplete)
{
	std::string caller = std::string(_sender);
	
	if (CallList.count(caller) > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("LibraryManager - AddCall - %s call is already in the call list, can not add again."), *FString(caller.c_str()));
		return false;
	}

	FAnkrCallStruct call{};
	call.callIndex = LibraryManager::GetGlobalCallIndex();
	call.sender = FString(_sender);
	call.CallComplete = _callComplete;
	CallList[caller] = call;

	//UE_LOG(LogTemp, Warning, TEXT("LibraryManager - AddCall - %s call is added to the call list successfully."), *call.sender);
	return true;
}

void LibraryManager::FlushCall(const char* _sender, bool _success, const char* _data)
{
	std::string caller = std::string(_sender);

	if (CallList.count(caller) <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("LibraryManager - FlushCall - %s call doesn't exist in the call list."), *FString(caller.c_str()));
		return;
	}

	FAnkrCallStruct call =  CallList[caller];
	call.success = _success;
	call.data = FString(_data);
	CallQueue.push(call);
	CallList.erase(caller);

	//UE_LOG(LogTemp, Warning, TEXT("LibraryManager - FlushCall - %s call is pushed to queue successfully."), *call.sender);
}