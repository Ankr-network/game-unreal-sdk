#pragma once

typedef void(*SampleDelegate)(bool success, const char* data);

using DumpMethodHandle = void(__stdcall*) (bool, const char*, SampleDelegate);

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

		void* HNDL = NULL;
		DumpMethodHandle dumpMethod = NULL;
		bool isInitialized;

		void LoadLibrary();
		void UnloadLibrary();
		void DumpMethod(bool, FString);
};