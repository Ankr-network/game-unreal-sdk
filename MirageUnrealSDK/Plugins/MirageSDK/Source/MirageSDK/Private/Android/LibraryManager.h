#pragma once

#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "Android/AndroidJava.h"

class MIRAGESDK_API LibraryManager
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

	void LoadLibrary();
	void UnloadLibrary();
	void Initialize(FString appId, FString deviceId, FString publicAddress, FString language);
	void LoadAd(FString _unitId);
	void ShowView(FString _unitId);

	jobject GetJString(FString string);
	TArray<FString> GetFStringArrayFromJava(TSharedPtr<FJavaClassObject> javaObject, FJavaClassMethod javaMethod, const char* seperator);
};
