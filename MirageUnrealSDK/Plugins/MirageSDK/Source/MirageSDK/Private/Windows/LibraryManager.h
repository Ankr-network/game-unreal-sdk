#pragma once

// __stdcall
typedef void(*SampleDelegate)(bool success, const char* data);
using DumpMethodHandle = void(__stdcall*) (bool, const char*, SampleDelegate);
//

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

		// __stdcall
		void* HNDL = NULL;
		DumpMethodHandle dumpMethod = NULL;
		//

		bool isInitialized;

		void Load();
		void Unload();
		void DumpMethod(bool, FString);

		std::string GetString(std::wstring _wstring);
		std::wstring GetWString(FString input);
};