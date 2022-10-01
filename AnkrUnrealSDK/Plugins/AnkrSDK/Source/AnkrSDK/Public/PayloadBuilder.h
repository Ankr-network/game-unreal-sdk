#pragma once

#include "CoreMinimal.h"

/// PayloadBuilder class provides customizable and easy functionalities to generate payload with complex structures.
class ANKRSDK_API UPayloadBuilder : public UObject
{
public:

	/// GetBuilder function is used to get a new FJsonObject builder object.
	///
	/// The function does not require a parameter and returns FJsonObject builder.\n
	/// The returned builder will be used to populate the payload with key value pairs. An example is shown in the body section below.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	/// builder->SetStringField("YOUR_KEY", "YOUR_STRING_VALUE");
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	static TSharedPtr<FJsonObject> GetBuilder();

	/// Build function is used to convert the populated FJsonObject to FString.
	///
	/// The function requires a parameter and returns a FString.\n
	/// The returned FString will be the json version of the builder. An example is shown in the body section below.
	/// 
	/// @param builder A populated FJsonObject with key value pairs.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// const FString payload = UPayloadBuilder::Build(builder);
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	static FString Build(TSharedPtr<FJsonObject> builder);

	/// BoolToJsonValue function is used to convert bool to a FJsonValueBoolean.
	///
	/// The function requires a parameter and returns FJsonValueBoolean.\n
	/// The returned FJsonValueBoolean will be used as a value for a key.
	/// 
	/// @param _value A bool value to be converted to FJsonValueBoolean.
	static TSharedPtr<FJsonValueBoolean> BoolToJsonValue(bool _value);

	/// IntToJsonValue function is used to convert int to a FJsonValueNumber.
	///
	/// The function requires a parameter and returns FJsonValueNumber.\n
	/// The returned FJsonValueNumber will be used as a value for a key.
	/// 
	/// @param _value An int value to be converted to FJsonValueNumber.
	static TSharedPtr<FJsonValueNumber> IntToJsonValue(int _value);

	/// LongToJsonValue function is used to convert long to a FJsonValueNumber.
	///
	/// The function requires a parameter and returns FJsonValueNumber.\n
	/// The returned FJsonValueNumber will be used as a value for a key.
	/// 
	/// @param _value A long value to be converted to FJsonValueNumber.
	static TSharedPtr<FJsonValueNumber> LongToJsonValue(long _value);

	/// FloatToJsonValue function is used to convert float to a FJsonValueNumber.
	///
	/// The function requires a parameter and returns FJsonValueNumber.\n
	/// The returned FJsonValueNumber will be used as a value for a key.
	/// 
	/// @param _value A float value to be converted to FJsonValueNumber.
	static TSharedPtr<FJsonValueNumber> FloatToJsonValue(float _value);

	/// DoubleToJsonValue function is used to convert double to a FJsonValueNumber.
	///
	/// The function requires a parameter and returns FJsonValueNumber.\n
	/// The returned FJsonValueNumber will be used as a value for a key.
	/// 
	/// @param _value A double value to be converted to FJsonValueNumber.
	static TSharedPtr<FJsonValueNumber> DoubleToJsonValue(double _value);

	/// FStringToJsonValue function is used to convert FString to a FJsonValueString.
	///
	/// The function requires a parameter and returns FJsonValueString.\n
	/// The returned FJsonValueString will be used as a value for a key.
	/// 
	/// @param _value A FString value to be converted to FJsonValueString.
	static TSharedPtr<FJsonValueString> FStringToJsonValue(FString _value);

	/// AddArrayItem function is used to add an item to the specified FJsonValue array.
	///
	/// The function requires parameters and returns nothing.\n
	/// 
	/// @param _array The address of the source FJsonValue array.
	/// @param value A bool value to be added to the source array.
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, bool value);

	/// AddArrayItem function is used to add an item to the specified FJsonValue array.
	///
	/// The function requires parameters and returns nothing.\n
	/// 
	/// @param _array The address of the source FJsonValue array.
	/// @param value An int value to be added to the source array.
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, int value);

	/// AddArrayItem function is used to add an item to the specified FJsonValue array.
	///
	/// The function requires parameters and returns nothing.\n
	/// 
	/// @param _array The address of the source FJsonValue array.
	/// @param value A long value to be added to the source array.
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, long value);

	/// AddArrayItem function is used to add an item to the specified FJsonValue array.
	///
	/// The function requires parameters and returns nothing.\n
	/// 
	/// @param _array The address of the source FJsonValue array.
	/// @param value A float value to be added to the source array.
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, float value);

	/// AddArrayItem function is used to add an item to the specified FJsonValue array.
	///
	/// The function requires parameters and returns nothing.\n
	/// 
	/// @param _array The address of the source FJsonValue array.
	/// @param value A double value to be added to the source array.
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, double value);

	/// AddArrayItem function is used to add an item to the specified FJsonValue array.
	///
	/// The function requires parameters and returns nothing.\n
	/// 
	/// @param _array The address of the source FJsonValue array.
	/// @param value A FString value to be added to the source array.
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, FString value);

	/// AddNestedArray function is used to add a nested FJsonValue array to the specified FJsonValue array.
	///
	/// The function requires parameters and returns nothing.\n
	/// 
	/// @param _array The address of the source FJsonValue array.
	/// @param _nestedArray A FJsonValue array to be added to the source array as nested.
	static void AddNestedArray(TArray<TSharedPtr<FJsonValue>>& _array, TArray<TSharedPtr<FJsonValue>> _nestedArray);

	/// AddNestedObject function is used to add a nested FJsonObject array to the specified FJsonValue array.
	///
	/// The function requires parameters and returns nothing.\n
	/// 
	/// @param _array The address of the source FJsonValue array.
	/// @param _nestedObject A FJsonObject object to be added to the source array as nested.
	static void AddNestedObject(TArray<TSharedPtr<FJsonValue>>& _array, TSharedPtr<FJsonObject> _nestedObject);

	/// BuildPayload function is used to build a quick payload having a key and a bool value.
	///
	/// @param _key A key to be added to the payload.
	/// @param _value A bool value for the key.
	static FString BuildPayload(const FString _key, const bool _value);

	/// BuildPayload function is used to build a quick payload having a key and an int value.
	///
	/// @param _key A key to be added to the payload.
	/// @param _value An int value for the key.
	static FString BuildPayload(const FString _key, const int _value);

	/// BuildPayload function is used to build a quick payload having a key and a long value.
	///
	/// @param _key A key to be added to the payload.
	/// @param _value A long value for the key.
	static FString BuildPayload(const FString _key, const long _value);

	/// BuildPayload function is used to build a quick payload having a key and a float value.
	///
	/// @param _key A key to be added to the payload.
	/// @param _value A float value for the key.
	static FString BuildPayload(const FString _key, const float _value);

	/// BuildPayload function is used to build a quick payload having a key and a double value.
	///
	/// @param _key A key to be added to the payload.
	/// @param _value A double value for the key.
	static FString BuildPayload(const FString _key, const double _value);

	/// BuildPayload function is used to build a quick payload having a key and a FString value.
	///
	/// @param _key A key to be added to the payload.
	/// @param _value A FString value for the key.
	static FString BuildPayload(const FString _key, const FString _value);

	/// BuildPayload function is used to build a quick payload having a key and a convertible FJsonValue.
	///
	/// @param _key A key to be added to the payload.
	/// @param _value A convertible FJsonValue for the key.
	static FString BuildPayload(const FString _key, const TSharedPtr<FJsonValue> _value);

	/// BuildPayload function is used to build a quick payload having two keys with convertible FJsonValue.
	/// The function requires two key value pairs to be added to the payload.
	static FString BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
								const FString _key2, const TSharedPtr<FJsonValue> _value2);
	
	/// BuildPayload function is used to build a quick payload having three keys with convertible FJsonValue.
	/// The function requires three key value pairs to be added to the payload.
	static FString BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
								const FString _key2, const TSharedPtr<FJsonValue> _value2,
								const FString _key3, const TSharedPtr<FJsonValue> _value3);
	
	/// BuildPayload function is used to build a quick payload having four keys with convertible FJsonValue.
	/// The function requires four key value pairs to be added to the payload.
	static FString BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
								const FString _key2, const TSharedPtr<FJsonValue> _value2,
								const FString _key3, const TSharedPtr<FJsonValue> _value3,
								const FString _key4, const TSharedPtr<FJsonValue> _value4);
	
	/// BuildPayload function is used to build a quick payload having five keys with convertible FJsonValue.
	/// The function requires five key value pairs to be added to the payload.
	static FString BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
								const FString _key2, const TSharedPtr<FJsonValue> _value2,
								const FString _key3, const TSharedPtr<FJsonValue> _value3,
								const FString _key4, const TSharedPtr<FJsonValue> _value4,
								const FString _key5, const TSharedPtr<FJsonValue> _value5);
	
	/// BuildPayload function is used to build a quick payload having six keys with convertible FJsonValue.
	/// The function requires six key value pairs to be added to the payload.
	static FString BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
								const FString _key2, const TSharedPtr<FJsonValue> _value2,
								const FString _key3, const TSharedPtr<FJsonValue> _value3,
								const FString _key4, const TSharedPtr<FJsonValue> _value4,
								const FString _key5, const TSharedPtr<FJsonValue> _value5,
								const FString _key6, const TSharedPtr<FJsonValue> _value6);
	
	/// BuildPayload function is used to build a quick payload having seven keys with convertible FJsonValue.
	/// The function requires seven key value pairs to be added to the payload.
	static FString BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
								const FString _key2, const TSharedPtr<FJsonValue> _value2,
								const FString _key3, const TSharedPtr<FJsonValue> _value3,
								const FString _key4, const TSharedPtr<FJsonValue> _value4,
								const FString _key5, const TSharedPtr<FJsonValue> _value5,
								const FString _key6, const TSharedPtr<FJsonValue> _value6,
								const FString _key7, const TSharedPtr<FJsonValue> _value7);
	
	/// BuildPayload function is used to build a quick payload having eight keys with convertible FJsonValue.
	/// The function requires eight key value pairs to be added to the payload.
	static FString BuildPayload(const FString _key1, const TSharedPtr<FJsonValue> _value1,
								const FString _key2, const TSharedPtr<FJsonValue> _value2,
								const FString _key3, const TSharedPtr<FJsonValue> _value3,
								const FString _key4, const TSharedPtr<FJsonValue> _value4,
								const FString _key5, const TSharedPtr<FJsonValue> _value5,
								const FString _key6, const TSharedPtr<FJsonValue> _value6,
								const FString _key7, const TSharedPtr<FJsonValue> _value7,
								const FString _key8, const TSharedPtr<FJsonValue> _value8);
	
};