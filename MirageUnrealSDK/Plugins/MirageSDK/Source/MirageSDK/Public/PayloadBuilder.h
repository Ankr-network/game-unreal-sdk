#pragma once

#include "CoreMinimal.h"

/// PayloadBuilder class provides customizable and easy functionalities to generate payload with complex structures.
class MIRAGESDK_API UPayloadBuilder : public UObject
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

	/// BuildPayload function is used to build a payload based on the specified TMap.
	/// 
	/// @param _map A TMap that contains the key value pair entries.
	static FString BuildPayload(TMap<FString, TSharedPtr<FJsonValue>> _map);
	
};