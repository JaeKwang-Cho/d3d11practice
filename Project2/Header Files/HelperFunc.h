#pragma once

namespace StringHelper
{
	wstring StrToWstr(string _str);
	string WstrToStr(wstring _wstr);
	wstring GetDirectoryFromPath(const wstring& _filePath);
	string GetDirectoryFromPath(const string& _filePath);
	wstring GetFileExtension(const wstring& _fileName);
}