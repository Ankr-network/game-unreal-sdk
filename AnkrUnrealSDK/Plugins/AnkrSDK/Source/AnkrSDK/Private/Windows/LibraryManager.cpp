//#if PLATFORM_WINDOWS

#include "LibraryManager.h"
#include <codecvt>



void LibraryManager::Load()
{
	FString dllPath = *FPaths::ProjectPluginsDir() + FString("AnkrSDK/Source/AnkrSDK/Private/Windows/Libraries/AnkrSDKLibraryWindows.dll");
	bool exists = FPaths::FileExists(dllPath);

	/*std::string path = std::string(TCHAR_TO_UTF8(*dllPath));
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring conversion = converter.from_bytes(path);*/
	
	/*HNDL_DLL = LoadLibrary(GetWString(dllPath).c_str());
	if (!HNDL_DLL)
	{
		UE_LOG(LogTemp, Warning, TEXT("LibraryManager - HNDL_DLL can not be loaded."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("LibraryManager - HNDL_DLL loaded."));*/

	/*dumpMethod = reinterpret_cast<dll_DumpMethod>(reinterpret_cast<void*>(GetProcAddress(HNDL_DLL, "dumpMethod")));
	if (!dumpMethod)
	{
		UE_LOG(LogTemp, Warning, TEXT("dumpMethod can not be loaded."));
		return;
	}*/

	//PingFunctionCplusplus		   = reinterpret_cast<PingCplusplus>(reinterpret_cast<void*>(GetProcAddress(HNDL_DLL, "Ping")));
	//ConnectWalletFunctionCplusplus = reinterpret_cast<ConnectWalletCplusplus>(reinterpret_cast<void*>(GetProcAddress(HNDL_DLL, "ConnectWallet")));

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

void LibraryManager::DumpMethod(FString _dump)
{
	/*dumpMethod(GetWString(_dump).c_str(), [](bool success, const wchar_t* data)
		{

		});*/
	
	/*UE_LOG(LogTemp, Warning, TEXT("DumpMethod"));
	PingFunction([](bool success, const char* data)
		{
			UE_LOG(LogTemp, Warning, TEXT("C++ - LibraryManager - DumpMethod - data: %s"), *FString(data));
		});*/
}

void LibraryManager::Unload()
{
	/*if (HNDL_DLL)
	{
		dumpMethod = NULL;

		bool success = FreeLibrary(HNDL_DLL);
		if (success)
		{
			UE_LOG(LogTemp, Warning, TEXT("LibraryManager - UnloadLibrary - Library unloaded successfully."));
		}
	}*/

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

std::wstring LibraryManager::GetWString(FString input)
{
	std::string raw = std::string(TCHAR_TO_UTF8(*input));
	/*std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring conversion = converter.from_bytes(raw);

	return conversion;*/

	int count = MultiByteToWideChar(CP_UTF8, 0, raw.c_str(), raw.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, raw.c_str(), raw.length(), &wstr[0], count);
	return wstr;
}

std::string LibraryManager::GetString(std::wstring _wstring)
{
	/*std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string conversion = converter.to_bytes(_wstring);
	return conversion;*/

	int count = WideCharToMultiByte(CP_UTF8, 0, _wstring.c_str(), _wstring.length(), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_UTF8, 0, _wstring.c_str(), -1, &str[0], count, NULL, NULL);
	return str;
}

FString LibraryManager::GetFString(std::wstring _wstring)
{
	FString conversion = FString(GetString(_wstring).c_str());
	return conversion;
}

void LibraryManager::Log(FString _message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *_message);
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
		//UE_LOG(LogTemp, Warning, TEXT("LibraryManager - AddCall - %s call is already in the call list, can not be added again."), *FString(caller.c_str()));
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
//#endif