#include "d3dUtil.h"

LPCWSTR ConvertToLPCWSTR(const std::string& str)
{
	LPCWSTR result;
	size_t size = strlen(str.c_str()) + 1;
	size_t outSize;
	mbstowcs_s(&outSize, NULL, 0, str.c_str(), size);
	wchar_t* buf = new wchar_t[outSize];
	mbstowcs_s(&outSize, buf, outSize, str.c_str(), size);
	result = buf;
	return result;
}