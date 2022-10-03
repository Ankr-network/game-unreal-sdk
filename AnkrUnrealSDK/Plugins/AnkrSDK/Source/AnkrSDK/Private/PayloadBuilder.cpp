#include "PayloadBuilder.h"
#include "AnkrUtility.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Serialization/JsonSerializer.h"

TSharedPtr<FJsonObject> UPayloadBuilder::GetBuilder()
{
	TSharedPtr<FJsonObject> builder = MakeShareable(new FJsonObject);
	return builder;
}

FString UPayloadBuilder::Build(TSharedPtr<FJsonObject> builder)
{
	FString payload;
	TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&payload);
	FJsonSerializer::Serialize(builder.ToSharedRef(), writer);
	return payload;
}

TSharedPtr<FJsonValueBoolean> UPayloadBuilder::BoolToJsonValue(bool _value)
{
	return MakeShareable(new FJsonValueBoolean(_value));
}

TSharedPtr<FJsonValueNumber> UPayloadBuilder::IntToJsonValue(int _value)
{
	return MakeShareable(new FJsonValueNumber(_value));
}

TSharedPtr<FJsonValueNumber> UPayloadBuilder::LongToJsonValue(long _value)
{
	return MakeShareable(new FJsonValueNumber(_value));
}

TSharedPtr<FJsonValueNumber> UPayloadBuilder::FloatToJsonValue(float _value)
{
	return MakeShareable(new FJsonValueNumber(_value));
}

TSharedPtr<FJsonValueNumber> UPayloadBuilder::DoubleToJsonValue(double _value)
{
	return MakeShareable(new FJsonValueNumber(_value));
}

TSharedPtr<FJsonValueString> UPayloadBuilder::FStringToJsonValue(FString _value)
{
	return MakeShareable(new FJsonValueString(_value));
}

void UPayloadBuilder::AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, bool value)
{
	_array.Add(MakeShareable(new FJsonValueBoolean(value)));
}

void UPayloadBuilder::AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, int value)
{
	_array.Add(MakeShareable(new FJsonValueNumber(value)));
}

void UPayloadBuilder::AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, long value)
{
	_array.Add(MakeShareable(new FJsonValueNumber(value)));
}

void UPayloadBuilder::AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, float value)
{
	_array.Add(MakeShareable(new FJsonValueNumber(value)));
}

void UPayloadBuilder::AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, double value)
{
	_array.Add(MakeShareable(new FJsonValueNumber(value)));
}

void UPayloadBuilder::AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, FString value)
{
	_array.Add(MakeShareable(new FJsonValueString(value)));
}

void UPayloadBuilder::AddNestedArray(TArray<TSharedPtr<FJsonValue>>& _array, TArray<TSharedPtr<FJsonValue>> _nestedArray)
{
	TSharedRef<FJsonValueArray> nestedArray = MakeShareable(new FJsonValueArray(_nestedArray));
	_array.Add(nestedArray);
}

void UPayloadBuilder::AddNestedObject(TArray<TSharedPtr<FJsonValue>>& _array, TSharedPtr<FJsonObject> _nestedObject)
{
	TSharedRef<FJsonValueObject> nestedObject = MakeShareable(new FJsonValueObject(_nestedObject));
	_array.Add(nestedObject);
}

FString UPayloadBuilder::BuildPayload(TMap<FString, TSharedPtr<FJsonValue>> _map)
{
	if (_map.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PayloadBuilder - BuildPayload - The parameter _map has no entries, returning an empty FString."));
		return FString("");
	}

	TSharedPtr<FJsonObject> builder = GetBuilder();
	for (auto& pair : _map)
	{
		builder->SetField(pair.Key, pair.Value);
	}
	return Build(builder);
}