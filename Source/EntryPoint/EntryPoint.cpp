#include <cstdio>

#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>

int APIENTRY NextAPI_wWinMain(
	_In_ HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR        lpCmdLine,
	_In_ int           nShowCmd);

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd)
{
	AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);

	std::filesystem::current_path("../../Resources/");

	std::cout << std::filesystem::current_path();

	int result = NextAPI_wWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	// Cleanup
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

	FreeConsole();

	return 0;
}