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

FString UPayloadBuilder::BuildPayload(const FString _key, const bool _value)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key, MakeShareable(new FJsonValueBoolean(_value)));
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key, const int _value)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key, MakeShareable(new FJsonValueNumber(_value)));
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key, const long _value)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key, MakeShareable(new FJsonValueNumber(_value)));
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key, const float _value)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key, MakeShareable(new FJsonValueNumber(_value)));
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key, const double _value)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key, MakeShareable(new FJsonValueNumber(_value)));
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key, const FString _value)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key, MakeShareable(new FJsonValueString(_value)));
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key, const TSharedPtr<FJsonValue> _value)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key, _value);
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
									  const FString _key2, const TSharedPtr<FJsonValue> _value2)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key1, _value1);
	builder->SetField(_key2, _value2);
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
									  const FString _key2, const TSharedPtr<FJsonValue> _value2,
									  const FString _key3, const TSharedPtr<FJsonValue> _value3)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key1, _value1);
	builder->SetField(_key2, _value2);
	builder->SetField(_key3, _value3);
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
									  const FString _key2, const TSharedPtr<FJsonValue> _value2,
									  const FString _key3, const TSharedPtr<FJsonValue> _value3,
									  const FString _key4, const TSharedPtr<FJsonValue> _value4)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key1, _value1);
	builder->SetField(_key2, _value2);
	builder->SetField(_key3, _value3);
	builder->SetField(_key4, _value4);
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
									  const FString _key2, const TSharedPtr<FJsonValue> _value2,
									  const FString _key3, const TSharedPtr<FJsonValue> _value3,
									  const FString _key4, const TSharedPtr<FJsonValue> _value4,
									  const FString _key5, const TSharedPtr<FJsonValue> _value5)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key1, _value1);
	builder->SetField(_key2, _value2);
	builder->SetField(_key3, _value3);
	builder->SetField(_key4, _value4);
	builder->SetField(_key5, _value5);
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
									  const FString _key2, const TSharedPtr<FJsonValue> _value2,
									  const FString _key3, const TSharedPtr<FJsonValue> _value3,
									  const FString _key4, const TSharedPtr<FJsonValue> _value4,
									  const FString _key5, const TSharedPtr<FJsonValue> _value5,
									  const FString _key6, const TSharedPtr<FJsonValue> _value6)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key1, _value1);
	builder->SetField(_key2, _value2);
	builder->SetField(_key3, _value3);
	builder->SetField(_key4, _value4);
	builder->SetField(_key5, _value5);
	builder->SetField(_key6, _value6);
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
									  const FString _key2, const TSharedPtr<FJsonValue> _value2,
									  const FString _key3, const TSharedPtr<FJsonValue> _value3,
									  const FString _key4, const TSharedPtr<FJsonValue> _value4,
									  const FString _key5, const TSharedPtr<FJsonValue> _value5,
									  const FString _key6, const TSharedPtr<FJsonValue> _value6,
									  const FString _key7, const TSharedPtr<FJsonValue> _value7)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key1, _value1);
	builder->SetField(_key2, _value2);
	builder->SetField(_key3, _value3);
	builder->SetField(_key4, _value4);
	builder->SetField(_key5, _value5);
	builder->SetField(_key6, _value6);
	builder->SetField(_key7, _value7);
	return Build(builder);
}

FString UPayloadBuilder::BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
									  const FString _key2, const TSharedPtr<FJsonValue> _value2,
									  const FString _key3, const TSharedPtr<FJsonValue> _value3,
									  const FString _key4, const TSharedPtr<FJsonValue> _value4,
									  const FString _key5, const TSharedPtr<FJsonValue> _value5,
									  const FString _key6, const TSharedPtr<FJsonValue> _value6,
									  const FString _key7, const TSharedPtr<FJsonValue> _value7,
									  const FString _key8, const TSharedPtr<FJsonValue> _value8)
{
	TSharedPtr<FJsonObject> builder = GetBuilder();
	builder->SetField(_key1, _value1);
	builder->SetField(_key2, _value2);
	builder->SetField(_key3, _value3);
	builder->SetField(_key4, _value4);
	builder->SetField(_key5, _value5);
	builder->SetField(_key6, _value6);
	builder->SetField(_key7, _value7);
	builder->SetField(_key8, _value8);
	return Build(builder);
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