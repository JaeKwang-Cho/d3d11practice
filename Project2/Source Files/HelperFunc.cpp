#include "pch.h"
#include "HelperFunc.h"

wstring StringHelper::StrToWstr(string _str)
{
	wstring wstr(_str.begin(), _str.end());
	return wstr;
}

string StringHelper::WstrToStr(wstring _wstr)
{
	string str;
	str.assign(_wstr.begin(), _wstr.end());
	return str;
}

wstring StringHelper::GetDirectoryFromPath(const wstring& _filePath)
{
	size_t off = _filePath.find_last_of(L'\\');
	if (off == wstring::npos)
	{
		return wstring();
	}
	return _filePath.substr(0, off);
}

wstring StringHelper::GetFileExtension(const wstring& _fileName)
{
	size_t off = _fileName.find_last_of(L'.');
	if (off == wstring::npos)
	{
		return wstring();
	}
	return wstring(_fileName.substr(off + 1));
}
