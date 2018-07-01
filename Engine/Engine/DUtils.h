#pragma once
#include <Windows.h>

wchar_t* CharToWCHAR(char* word)
{
	int num = MultiByteToWideChar(0, 0, word, -1, NULL, 0);
	wchar_t* result = new wchar_t[num];
	MultiByteToWideChar(0, 0, word, -1, result, num);
	return result;
}