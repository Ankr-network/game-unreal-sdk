#include "LibraryManager.h"
#include "AnkrSaveGame.h"
#include "AnkrUtility.h"

void LibraryManager::LoadLibrary()
{
	//FJavaClassObject_DumpClass  = MakeShareable(new FJavaClassObject("com/company/product/class", "(Landroid/app/Activity;)V", FJavaWrapper::GameActivityThis));
	//FJavaClassMethod_DumpMethod = FJavaClassObject_DumpClass->GetClassMethod("Dump", "()V");
	//FJavaClassObject_DumpClass->CallMethod<void>(FJavaClassMethod_DumpMethod);

	JCO_AnkrAds = MakeShareable(new FJavaClassObject("com/ankr/nativeads/AnkrAds", "()V"));

	UE_LOG(LogTemp, Error, TEXT("LibraryManager.cpp - Load"));
}

void LibraryManager::UnloadLibrary()
{
	// Nothing to do here for now.
}

void LibraryManager::Initialize(FString appId, FString deviceId, FString publicAddress, FString language)
{
	JNIEnv* env = AndroidJavaEnv::GetJavaEnv();
	
	jclass localRef = env->GetObjectClass(JCO_AnkrAds->GetJObject());
	jclass globalRef = reinterpret_cast<jclass>(env->NewGlobalRef(localRef));

	jmethodID method = env->GetStaticMethodID(globalRef, "Initialize", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	env->CallStaticVoidMethod(globalRef, method, FJavaWrapper::GameActivityThis, GetJString(appId), GetJString(deviceId), GetJString(publicAddress), GetJString(language));
}

void LibraryManager::LoadAd(FString _unitId)
{
	JNIEnv* env = AndroidJavaEnv::GetJavaEnv();

	jclass localRef = env->GetObjectClass(JCO_AnkrAds->GetJObject());
	jclass globalRef = reinterpret_cast<jclass>(env->NewGlobalRef(localRef));

	jmethodID method = env->GetStaticMethodID(globalRef, "LoadAd", "(Ljava/lang/String;)V");
	env->CallStaticVoidMethod(globalRef, method, GetJString(_unitId));
}

void LibraryManager::ShowView(FString _unitId)
{
	JNIEnv* env = AndroidJavaEnv::GetJavaEnv();

	jclass localRef = env->GetObjectClass(JCO_AnkrAds->GetJObject());
	jclass globalRef = reinterpret_cast<jclass>(env->NewGlobalRef(localRef));

	jmethodID method = env->GetStaticMethodID(globalRef, "ShowView", "(Ljava/lang/String;)V");
	env->CallStaticVoidMethod(globalRef, method, GetJString(_unitId));
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
