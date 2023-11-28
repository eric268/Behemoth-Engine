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
	int result = NextAPI_wWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	return 0;
}