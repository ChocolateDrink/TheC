#pragma once
#include <string>
#include <time.h>

namespace timeUtil {
	std::wstring getDate() {
		time_t now = time(0);
		tm ltm;
		localtime_s(&ltm, &now);

		wchar_t dateStr[11];
		swprintf_s(dateStr, L"%04d-%02d-%02d", 1900 + ltm.tm_year, 1 + ltm.tm_mon, ltm.tm_mday);

		return std::wstring(dateStr) + L".txt";
	}

	std::wstring getTime(bool logPrefix = true) {
		SYSTEMTIME st;
		GetLocalTime(&st);

		wchar_t timeStr[13];
		swprintf_s(timeStr, L"%02d:%02d:%02d:%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		return logPrefix ? L"[" + std::wstring(timeStr) + L"] " : std::wstring(timeStr);
	}
}