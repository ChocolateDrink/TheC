#include <Windows.h>
#include <shlobj.h>
#include <iostream>
#include <ctime>
#include <thread>

#include "io.h"
#include "time.h"
#include "recorder.h"

#pragma region Config

CONST WCHAR* MAIN_FOLDER_NAME = L"VidLogger"; // name name of the main folder
CONST WCHAR* RECORDED_VIDEOS_FOLDER_NAME = L"Videos"; // the name of the folder where the recorded videos will be stored
CONST WCHAR* LOGS_FOLDER_NAME = L"Logs"; // the name of the folder where logs will be stored

CONST INT VIDEO_LENGTH = 60; // how long the video will be recorded for

#pragma endregion

int main() {
	PWSTR localAppData;
	HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, NULL, NULL, &localAppData);
	if (result != S_OK) {
		std::cout << "failed to get base dir" << '\n';
		return EXIT_FAILURE;
	}

	std::wstring baseDir(localAppData);
	std::wstring mainFolder = baseDir + L"\\" + MAIN_FOLDER_NAME;
	io::makeFolder(mainFolder);

	io::vids = mainFolder + L"\\" + RECORDED_VIDEOS_FOLDER_NAME + L"\\";
	io::logs = mainFolder + L"\\" + LOGS_FOLDER_NAME + L"\\";

	io::makeFolder(io::logs);
	io::makeFolder(io::vids);

	CoTaskMemFree(localAppData);

	std::wstring filename = timeUtil::getDate();
	io::makeFile(io::logs + filename);
	io::appendFile(io::logs + filename, timeUtil::getTime() + L"first boot\n");

	while (true) {
		io::appendFile(io::logs + filename, timeUtil::getTime() + L"recording start\n");
		if (recorder::startRecording(VIDEO_LENGTH, io::vids + timeUtil::getTime(false) + L".mp4"))
			io::appendFile(io::logs + filename, timeUtil::getTime() + L"video successful\n");
		else
			io::appendFile(io::logs + filename, timeUtil::getTime() + L"ERROR failed to record\n");

		io::appendFile(io::logs + filename, timeUtil::getTime() + L"recording end\n");

		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	io::appendFile(io::logs + filename, timeUtil::getTime() + L"WARN loop killed\n");

	return EXIT_SUCCESS;
}