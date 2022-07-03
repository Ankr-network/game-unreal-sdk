#pragma once

#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "Android/AndroidJava.h"

#include <string>
#include <queue>
#include <unordered_map>
#include "RequestBodyStructure.h"
#include "../../Public/AnkrDelegates.h"

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

	TSharedPtr<FJavaClassObject> FJavaClassObject_DumpClass;
	FJavaClassMethod FJavaClassMethod_DumpMethod;

	TSharedPtr<FJavaClassObject> JCO_AnkrClient;
	FJavaClassMethod JCM_LaunchWalletForLogin;
	FJavaClassMethod JCM_LaunchWalletForTransaction;
	FJavaClassMethod JCM_Initialize;
	FJavaClassMethod JCM_Ping;
	FJavaClassMethod JCM_ConnectWallet;
	FJavaClassMethod JCM_GetWallet;
	FJavaClassMethod JCM_SendABI;
	FJavaClassMethod JCM_SendTransaction;
	FJavaClassMethod JCM_GetResult;
	FJavaClassMethod JCM_CallMethod;
	FJavaClassMethod JCM_SignMessage;
	FJavaClassMethod JCM_GetSignature;
	FJavaClassMethod JCM_VerifyMessage;

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

	jobject GetJString(FString string);
	TArray<FString> GetFStringArrayFromJava(TSharedPtr<FJavaClassObject> javaObject, FJavaClassMethod javaMethod, const char* seperator);

	void Load();
	void Unload();

	int GlobalCallIndex;
	std::unordered_map<std::string, FAnkrCallStruct> CallList;
	std::queue<FAnkrCallStruct> CallQueue;

	int GetGlobalCallIndex();
	bool AddCall(const char* _sender, const FAnkrCallCompleteDynamicDelegate _callComplete);
	void FlushCall(const char* _sender, bool _success, const char* _data);
};