#pragma once

#if PLATFORM_ANDROID

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
	FJavaClassMethod JCM_CallMethod;
	FJavaClassMethod JCM_SignMessage;
	FJavaClassMethod JCM_GetResult;
	FJavaClassMethod JCM_VerifyMessage;

	jobject GetJString(FString string);
	TArray<FString> GetFStringArrayFromJava(TSharedPtr<FJavaClassObject> javaObject, FJavaClassMethod javaMethod, const char* seperator);

	void Load();
	void DumpMethod(FString _dump);
	void Unload();

	int GlobalCallIndex;
	std::unordered_map<std::string, FAnkrCallStruct> CallList;
	std::queue<FAnkrCallStruct> CallQueue;

	int GetGlobalCallIndex();
	bool AddCall(const char* _sender, const FAnkrCallCompleteDynamicMulticastDelegate _callComplete);
	void FlushCall(const char* _sender, bool _success, const char* _data);
};

#endif