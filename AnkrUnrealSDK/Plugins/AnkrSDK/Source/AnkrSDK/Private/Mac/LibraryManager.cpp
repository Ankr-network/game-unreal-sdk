#include "LibraryManager.h"
#include <codecvt>

void LibraryManager::Load()
{
    if(ankrClient == nullptr)
    {
        ankrClient = [[AnkrClient alloc] init];
    }
}

void LibraryManager::Unload()
{
}

void LibraryManager::Initialize(bool _isDevelopment, FString _device_id)
{
    [ankrClient InitializeWith_isDevelopment:_isDevelopment _device_id:FStringToNSString(*_device_id)];
}
void LibraryManager::Ping()
{
    [ankrClient PingWithFunction:^(BOOL _success, NSString* _sender, NSString* _data)
     {
        UE_LOG(LogTemp, Warning, TEXT("ObjC - LibraryManager - %s - _success: %d | _data: %s"), *NSStringToFString(_sender), _success, *NSStringToFString(_data));
        
        FlushCall([_sender UTF8String], _success, [_data UTF8String]);
     }];
}
void LibraryManager::ConnectWallet(FString _content)
{
    [ankrClient ConnectWalletWith_content:FStringToNSString(_content) function:^(BOOL _success, NSString* _sender, NSString* _data)
     {
        UE_LOG(LogTemp, Warning, TEXT("ObjC - LibraryManager - %s - _success: %d | _data: %s"), *NSStringToFString(_sender), _success, *NSStringToFString(_data));
        
        FlushCall([_sender UTF8String], _success, [_data UTF8String]);
     }];
}
void LibraryManager::GetWallet(FString _content)
{
    [ankrClient GetWalletWith_content : FStringToNSString(_content) function : ^ (BOOL _success, NSString * _sender, NSString * _data)
    {
        UE_LOG(LogTemp, Warning, TEXT("ObjC - LibraryManager - %s - _success: %d | _data: %s"), *NSStringToFString(_sender), _success, *NSStringToFString(_data));

        FlushCall([_sender UTF8String], _success, [_data UTF8String]);
    }] ;
}
void LibraryManager::SendABI(FString _content)
{
    [ankrClient SendABIWith_content:FStringToNSString(_content) function:^(BOOL _success, NSString* _sender, NSString* _data)
     {
        UE_LOG(LogTemp, Warning, TEXT("ObjC - LibraryManager - %s - _success: %d | _data: %s"), *NSStringToFString(_sender), _success, *NSStringToFString(_data));
        
        FlushCall([_sender UTF8String], _success, [_data UTF8String]);
     }];
}
void LibraryManager::SendTransaction(FString _content)
{
    [ankrClient SendTransactionWith_content:FStringToNSString(_content) function:^(BOOL _success, NSString* _sender, NSString* _data)
     {
        UE_LOG(LogTemp, Warning, TEXT("ObjC - LibraryManager - %s - _success: %d | _data: %s"), *NSStringToFString(_sender), _success, *NSStringToFString(_data));
        
        FlushCall([_sender UTF8String], _success, [_data UTF8String]);
     }];
}
void LibraryManager::CallMethod(FString _content)
{
    [ankrClient CallMethodWith_content:FStringToNSString(_content) function:^(BOOL _success, NSString* _sender, NSString* _data)
     {
        UE_LOG(LogTemp, Warning, TEXT("ObjC - LibraryManager - %s - _success: %d | _data: %s"), *NSStringToFString(_sender), _success, *NSStringToFString(_data));
        
        FlushCall([_sender UTF8String], _success, [_data UTF8String]);
     }];
}
void LibraryManager::SignMessage(FString _content)
{
    [ankrClient SignMessageWith_content:FStringToNSString(_content) function:^(BOOL _success, NSString* _sender, NSString* _data)
     {
        UE_LOG(LogTemp, Warning, TEXT("ObjC - LibraryManager - %s - _success: %d | _data: %s"), *NSStringToFString(_sender), _success, *NSStringToFString(_data));
        
        FlushCall([_sender UTF8String], _success, [_data UTF8String]);
     }];
}
void LibraryManager::GetResult(FString _content)
{
    [ankrClient GetResultWith_content:FStringToNSString(_content) function:^(BOOL _success, NSString* _sender, NSString* _data)
     {
        UE_LOG(LogTemp, Warning, TEXT("ObjC - LibraryManager - %s - _success: %d | _data: %s"), *NSStringToFString(_sender), _success, *NSStringToFString(_data));
        
        FlushCall([_sender UTF8String], _success, [_data UTF8String]);
     }];
}
void LibraryManager::VerifyMessage(FString _content)
{
    [ankrClient VerifyMessageWith_content:FStringToNSString(_content) function:^(BOOL _success, NSString* _sender, NSString* _data)
     {
        UE_LOG(LogTemp, Warning, TEXT("ObjC - LibraryManager - %s - _success: %d | _data: %s"), *NSStringToFString(_sender), _success, *NSStringToFString(_data));
        
        FlushCall([_sender UTF8String], _success, [_data UTF8String]);
     }];
}

std::wstring LibraryManager::GetWString(FString input)
{
    std::string raw = std::string(TCHAR_TO_UTF8(*input));
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring conversion = converter.from_bytes(raw);

    return conversion;
}

std::string LibraryManager::GetString(std::wstring _wstring)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string conversion = converter.to_bytes(_wstring);
    return conversion;
}

FString LibraryManager::GetFString(std::wstring _wstring)
{
    FString conversion = FString(GetString(_wstring).c_str());
    return conversion;
}

NSString* LibraryManager::FStringToNSString(FString _input)
{
    std::string source = std::string(TCHAR_TO_UTF8(*_input));
    NSString* conversion = [[NSString alloc] initWithUTF8String:source.c_str()];
    return conversion;
}
FString LibraryManager::NSStringToFString(NSString* _input)
{
    FString conversion([_input UTF8String]);
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
bool LibraryManager::AddCall(const char* _sender, const FAnkrCallCompleteDynamicDelegate& _callComplete)
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
