#pragma once

namespace StringHelper
{
	wstring StrToWstr(string _str);
	string WstrToStr(wstring _wstr);
	wstring GetDirectoryFromPath(const wstring& _filePath);
	wstring GetFileExtension(const wstring& _fileName);
}