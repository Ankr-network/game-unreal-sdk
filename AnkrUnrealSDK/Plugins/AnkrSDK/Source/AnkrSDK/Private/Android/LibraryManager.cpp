#include "LibraryManager.h"
#include "AnkrSaveGame.h"

void LibraryManager::Load()
{
	/*JCO_AnkrClient = MakeShareable(new FJavaClassObject("com/ankr/ankrsdkunreal/AnkrClient", "(Landroid/app/Activity;)V", FJavaWrapper::GameActivityThis));

	JCM_LaunchWalletForLogin		= JCO_AnkrClient->GetClassMethod("LaunchWalletForLogin", "()V");
	JCM_LaunchWalletForTransaction	= JCO_AnkrClient->GetClassMethod("LaunchWalletForTransaction", "()V");
	JCM_Initialize					= JCO_AnkrClient->GetClassMethod("Initialize", "(ZLjava/lang/String;)V");
	JCM_Ping						= JCO_AnkrClient->GetClassMethod("Ping", "()V");
	JCM_ConnectWallet				= JCO_AnkrClient->GetClassMethod("ConnectWallet", "()V");
	JCM_GetWallet					= JCO_AnkrClient->GetClassMethod("GetWallet", "()V");
	JCM_SendABI						= JCO_AnkrClient->GetClassMethod("SendABI", "(Ljava/lang/String;)V");
	JCM_SendTransaction				= JCO_AnkrClient->GetClassMethod("SendTransaction", "(Ljava/lang/String;)V");
	JCM_CallMethod					= JCO_AnkrClient->GetClassMethod("CallMethod", "(Ljava/lang/String;)V");
	JCM_SignMessage					= JCO_AnkrClient->GetClassMethod("SignMessage", "(Ljava/lang/String;)V");
	JCM_GetResult					= JCO_AnkrClient->GetClassMethod("GetResult", "(Ljava/lang/String;)V");
	JCM_GetSignature				= JCO_AnkrClient->GetClassMethod("GetSignature", "(Ljava/lang/String;)V");
	JCM_VerifyMessage				= JCO_AnkrClient->GetClassMethod("VerifyMessage", "(Ljava/lang/String;)V");*/

	JNIEnv* env = AndroidJavaEnv::GetJavaEnv();
	//jclass jclass_AnkrAds = FJavaWrapper::FindClass(env, "com/ankr/nativeads/AnkrAds", false);//MakeShareable(new FJavaClassObject("com/ankr/nativeads/AnkrAds", "()V"));
	UE_LOG(LogTemp, Error, TEXT("LibraryManager.cpp - Load - Initialize"));
	Native_AnkrClient = MakeShareable(new FJavaClassObject("com/ankr/nativeads/AnkrAds", "()V"));
	jclass jclass_AnkrAds = env->GetObjectClass(Native_AnkrClient->GetJObject());
	jmethodID jmethodID_Initialize = env->GetStaticMethodID(jclass_AnkrAds, "Initialize", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	env->CallStaticVoidMethod(jclass_AnkrAds, jmethodID_Initialize, FJavaWrapper::GameActivityThis, GetJString("86fc9bc7-3f7a-46cf-9cab-a12935d1e4ad"), GetJString("123456"), GetJString("0x0E9E2a366F0d82502483380093867335a58025bF"), GetJString("EN"));
}



void LibraryManager::Unload()
{
	// Nothing to do here for now.
}

void LibraryManager::Initialize(bool _isDevelopment, FString _device_id)
{
	//JCO_AnkrClient->CallMethod<void>(JCM_Initialize, _isDevelopment, GetJString(_device_id));
}
void LibraryManager::Ping()
{
	JCO_AnkrClient->CallMethod<void>(JCM_Ping);
}
void LibraryManager::ConnectWallet(FString _content)
{
	JCO_AnkrClient->CallMethod<void>(JCM_ConnectWallet, GetJString(_content));
}
void LibraryManager::GetWallet(FString _content)
{
	JCO_AnkrClient->CallMethod<void>(JCM_GetWallet, GetJString(_content));
}
void LibraryManager::SendABI(FString _content)
{
	JCO_AnkrClient->CallMethod<void>(JCM_SendABI, GetJString(_content));
}
void LibraryManager::SendTransaction(FString _content)
{
	JCO_AnkrClient->CallMethod<void>(JCM_SendTransaction, GetJString(_content));
}
void LibraryManager::GetResult(FString _content)
{
	JCO_AnkrClient->CallMethod<void>(JCM_GetResult, GetJString(_content));
}
void LibraryManager::CallMethod(FString _content)
{
	JCO_AnkrClient->CallMethod<void>(JCM_CallMethod, GetJString(_content));
}
void LibraryManager::SignMessage(FString _content)
{
	JCO_AnkrClient->CallMethod<void>(JCM_SignMessage, GetJString(_content));
}
void LibraryManager::GetSignature(FString _content)
{
	JCO_AnkrClient->CallMethod<void>(JCM_GetSignature, GetJString(_content));
}
void LibraryManager::VerifyMessage(FString _content)
{
	JCO_AnkrClient->CallMethod<void>(JCM_VerifyMessage, GetJString(_content));
}

jobject LibraryManager::GetJString(FString string)
{
	JNIEnv* JEnv = AndroidJavaEnv::GetJavaEnv();
	jstring local = JEnv->NewStringUTF(TCHAR_TO_UTF8(*string));
	jstring result = (jstring)JEnv->NewGlobalRef(local);
	JEnv->DeleteLocalRef(local);
	return result;
}

TArray<FString> LibraryManager::GetFStringArrayFromJava(TSharedPtr<FJavaClassObject> javaObject, FJavaClassMethod javaMethod, const char* seperator)
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jstring javaReturn = static_cast<jstring>(javaObject->CallMethod<jobject>(javaMethod));
	const char* rawConversion = Env->GetStringUTFChars(javaReturn, nullptr);
	FString conversion(UTF8_TO_TCHAR(rawConversion));
	Env->ReleaseStringUTFChars(javaReturn, rawConversion);
	Env->DeleteGlobalRef(javaReturn);

	TArray<FString> NamesArray;
	FString seperatorFString(seperator);
	conversion.ParseIntoArray(NamesArray, *seperatorFString, true);

	return NamesArray;
}

JNI_METHOD void Java_com_ankr_ankrsdkunreal_AnkrClient_OnCallback(JNIEnv* env, jclass clazz, bool _success, jstring _sender, jstring _data)
{
	const char* senderUTF8 = env->GetStringUTFChars(_sender, 0);
	const char* dataUTF8 = env->GetStringUTFChars(_data, 0);
	FString sender(UTF8_TO_TCHAR(senderUTF8));
	FString data(UTF8_TO_TCHAR(dataUTF8));
	env->ReleaseStringUTFChars(_sender, senderUTF8);
	env->ReleaseStringUTFChars(_data, dataUTF8);

	UE_LOG(LogAndroid, Log, TEXT("C++ - LibraryManager - Java_com_ankr_ankrsdkunreal_AnkrClient_OnCallback - _success: %d | _sender: %s | _data: %s"), _success, *sender, *data);
	LibraryManager::GetInstance().FlushCall(TCHAR_TO_UTF8(*sender), _success, TCHAR_TO_UTF8(*data));
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

	FAnkrCallStruct call = CallList[caller];
	call.success = _success;
	call.data = FString(_data);
	CallQueue.push(call);
	CallList.erase(caller);

	//UE_LOG(LogTemp, Warning, TEXT("LibraryManager - FlushCall - %s call is pushed to queue successfully."), *call.sender);
}