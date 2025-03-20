#include <Windows.h>
#include <iostream>
#include <vector>

using std::vector;

#pragma region offsets
uintptr_t game = (uintptr_t)GetModuleHandle(L"GameAssembly.dll");

vector<unsigned int> coin_offsets = { 0x20, 0xB8, 0x0, 0x20 };
uintptr_t coin_address = 0x01E8AF18;
#pragma endregion

uintptr_t findAddress(uintptr_t pointer, vector<unsigned int> offsets) {
	uintptr_t address = pointer;

	for (unsigned int i = 0; i < offsets.size(); ++i) {
		address = *(uintptr_t*)address;
		address += offsets[i];
	}

	return address;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason != DLL_PROCESS_ATTACH)
		return FALSE;

	// infinite coins (kinda)
	*(double*)findAddress(game + coin_address, coin_offsets) += 9e200;

	return TRUE;
}