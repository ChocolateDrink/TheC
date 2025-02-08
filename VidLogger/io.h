#pragma once
#include <Windows.h>

using namespace std;

namespace io {
	wstring logs;
	wstring vids;

	bool makeFolder(const wstring& path) {
		if (CreateDirectoryW(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
			return TRUE;

		return FALSE;
	}

	bool makeFile(const wstring& path) {
		HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE) {
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				return TRUE;

			return FALSE;
		}

		CloseHandle(file);
		return true;
	}

	bool writeFile(const wstring path, const wstring content) {
		HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
			return FALSE;

		DWORD bytes;
		BOOL success = WriteFile(file, content.c_str(), content.size() * sizeof(WCHAR), &bytes, NULL);

		CloseHandle(file);
		return success;
	}

	bool appendFile(const wstring path, const wstring content) {
		HANDLE file = CreateFileW(path.c_str(), FILE_APPEND_DATA, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
			return FALSE;

		SetFilePointer(file, NULL, NULL, FILE_END);

		DWORD bytes;
		BOOL success = WriteFile(file, content.c_str(), content.size() * sizeof(WCHAR), &bytes, NULL);

		CloseHandle(file);
		return success;
	}
}