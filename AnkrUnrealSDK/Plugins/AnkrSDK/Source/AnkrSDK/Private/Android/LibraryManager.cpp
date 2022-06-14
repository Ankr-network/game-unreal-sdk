#if PLATFORM_ANDROID

#include "LibraryManager.h"
#include "AnkrSaveGame.h"

void LibraryManager::Load()
{
	FJavaClassObject_DumpClass = MakeShareable(new FJavaClassObject("com/organisation/product/DumpClassName", "(Landroid/app/Activity;Ljava/lang/String;)V", FJavaWrapper::GameActivityThis, GetJString("Dump")));
	
	FJavaClassMethod_DumpMethod = FJavaClassObject_DumpClass->GetClassMethod("DumpMethodName", "(Ljava/lang/String;)V");
}
	

void LibraryManager::DumpMethod(FString _dump)
{
	UE_LOG(LogTemp, Error, TEXT("LibraryManager.cpp - DumpMethod"));

	// If the method is not a static method, use this
	FJavaClassObject_DumpClass->CallMethod<void>(FJavaClassMethod_DumpMethod, GetJString(_dump));

	// If the method is a static method, use this
	JNIEnv* env = AndroidJavaEnv::GetJavaEnv();
	jclass jclass_DumpClass = env->GetObjectClass(FJavaClassObject_DumpClass->GetJObject());
	jmethodID jmethodID_DumpMethod = env->GetStaticMethodID(jclass_DumpClass, "DumpMethodName", "(Ljava/lang/String;)V");
	env->CallStaticVoidMethod(jclass_AnkrAds, jmethodID_DumpMethod, GetJString(_appId));
}

void LibraryManager::Unload()
{
	// Nothing to do here for now.
}

jobject LibraryManager::GetJString(FString string)
{
	JNIEnv* JEnv = AndroidJavaEnv::GetJavaEnv();
	jstring local = JEnv->NewStringUTF(TCHAR_TO_UTF8(*string));
	jstring result = (jstring)JEnv->NewGlobalRef(local);
	JEnv->DeleteLocalRef(local);
	return result;
}

TArray<FString> LibraryManager::GetFStringArrayFromJava(FJavaClassObject javaObject, FJavaClassMethod javaMethod, const char* seperator)
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

#endif