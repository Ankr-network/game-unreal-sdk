#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include "RequestBodyStructure.h"
#include "../../Public/AnkrDelegates.h"

typedef void(*LogCallbackDelegate)(const char* _message);
typedef void(*Callback)(bool success, const char* data);

using InitializeImport      = void(__stdcall*) (bool, const char*, LogCallbackDelegate);
using GetDeviceIdImport     = const char*(__stdcall*) ();
using GetQRCodeTextImport   = const char* (__stdcall*) ();
using GetSessionImport	    = const char* (__stdcall*) ();
using GetAccountImport	    = const char* (__stdcall*) ();
using GetChainIdImport      = int(__stdcall*) ();
using PingImport			= void(__stdcall*)(Callback);
using ConnectWalletImport   = void(__stdcall*)(const char*, Callback);
using GetWalletImport       = void(__stdcall*)(const char*, Callback);
using SendABIImport         = void(__stdcall*)(const char*, Callback);
using SendTransactionImport = void(__stdcall*)(const char*, Callback);
using CallMethodImport      = void(__stdcall*)(const char*, Callback);
using SignMessageImport     = void(__stdcall*)(const char*, Callback);
using GetResultImport       = void(__stdcall*)(const char*, Callback);
using VerifyMessageImport   = void(__stdcall*)(const char*, Callback);

class ANKRSDK_API LibraryManager
{
	public:
		static LibraryManager& GetInstance()
		{
			static LibraryManager INSTANCE;
			return INSTANCE;
		}

	private:
		LibraryManager() {}

	public:
		LibraryManager(LibraryManager const&) = delete;
		void operator = (LibraryManager const&) = delete;

	public:

		void Load();
		void Unload();

		void Log(FString _message);

		void* HNDL = NULL;
		InitializeImport      InitializeFunction      = NULL;
		GetDeviceIdImport     GetDeviceIdFunction     = NULL;
		GetQRCodeTextImport   GetQRCodeTextFunction   = NULL;
		GetSessionImport      GetSessionFunction      = NULL;
		GetAccountImport      GetAccountFunction      = NULL;
		GetChainIdImport      GetChainIdFunction      = NULL;
		PingImport            PingFunction            = NULL;
		ConnectWalletImport   ConnectWalletFunction   = NULL;
		GetWalletImport		  GetWalletFunction       = NULL;
		SendABIImport		  SendABIFunction		  = NULL;
		SendTransactionImport SendTransactionFunction = NULL;
		CallMethodImport	  CallMethodFunction      = NULL;
		SignMessageImport	  SignMessageFunction     = NULL;
		GetResultImport		  GetResultFunction       = NULL;
		VerifyMessageImport	  VerifyMessageFunction   = NULL;

		void Initialize(bool, FString);
		void Ping();
		void ConnectWallet(FString);
		void GetWallet(FString);
		void SendABI(FString);
		void SendTransaction(FString);
		void GetResult(FString);
		void CallMethod(FString);
		void SignMessage(FString);
		void GetSignature(FString _content);
		void VerifyMessage(FString);

		int GlobalCallIndex;
		std::unordered_map<std::string, FAnkrCallStruct> CallList;
		std::queue<FAnkrCallStruct> CallQueue;

		int GetGlobalCallIndex();
		bool AddCall(const char* _sender, const FAnkrCallCompleteDynamicDelegate _callComplete);
		void FlushCall(const char* _sender, bool _success, const char* _data);
};