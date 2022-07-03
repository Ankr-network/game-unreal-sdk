#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include <queue>
#include "../../Public/AnkrDelegates.h"
#include "RequestBodyStructure.h"

#import <Foundation/Foundation.h>
#import "AnkrSDKUnrealMac-Swift.h"

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

    AnkrClient* ankrClient = nullptr;
    void Load();
    void Unload();
    
    void Initialize(bool, FString);
    void Ping();
    void ConnectWallet(FString);
    void SendABI(FString);
    void SendTransaction(FString);
    void CallMethod(FString);
    void SignMessage(FString);
    void GetResult(FString);
    void VerifyMessage(FString);

    static std::wstring GetWString(FString input);
    static std::string GetString(std::wstring _wstring);
    static FString GetFString(std::wstring _wstring);
    NSString* FStringToNSString(FString _input);
    FString NSStringToFString(NSString* _input);
    
    void Log(FString _message);
    int GlobalCallIndex;
    std::unordered_map<std::string, FAnkrCallStruct> CallList;
    std::queue<FAnkrCallStruct> CallQueue;

    int GetGlobalCallIndex();
    bool AddCall(const char* _sender, const FAnkrCallCompleteDynamicDelegate& _callComplete);
    void FlushCall(const char* _sender, bool _success, const char* _data);
};
