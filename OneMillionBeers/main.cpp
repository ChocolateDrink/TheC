#include <Windows.h>
#include "./texts.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow) {
	CHAR path[MAX_PATH] = {};
	GetModuleFileNameA(NULL, path, MAX_PATH);

	HKEY handle = NULL;
	LONG result = RegOpenKeyExA(
		HKEY_CURRENT_USER,
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		NULL, KEY_SET_VALUE, &handle
	);

	if (result == ERROR_SUCCESS)
		RegSetValueExA(handle, "Beer Alarm", NULL, REG_SZ, (CONST BYTE*)path, (DWORD)(strlen(path) + 1));

	RegCloseKey(handle);

	bool waited = false;

	while (true) {
		if (!waited) {
			int sleepTime = texts::getRandomInt();
			Sleep(sleepTime);

			waited = true;
		}

		MBText textData = texts::getRandom();
		MessageBox(NULL, textData.text, textData.title, MB_OK | MB_OKCANCEL);

		int sleepTime = texts::getRandomInt();
		Sleep(sleepTime);
	}
}
