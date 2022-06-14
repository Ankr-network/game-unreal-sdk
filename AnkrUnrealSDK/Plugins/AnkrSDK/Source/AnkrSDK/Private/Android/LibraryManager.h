#pragma once

#if PLATFORM_ANDROID

#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "Android/AndroidJava.h"

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

	jobject GetJString(FString string);
	TArray<FString> GetFStringArrayFromJava(FJavaClassObject javaObject, FJavaClassMethod javaMethod, const char* seperator);

	void Load();
	void DumpMethod(FString _dump);
	void Unload();
};

#endif