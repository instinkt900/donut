#pragma once

#include <codecvt>

namespace donut {
	std::wstring StringToWString(const std::string& str)
	{
		auto wchars_num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		std::wstring wstr(wchars_num, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr.data(), wstr.size());
		return wstr;
	}

	std::string WStringToString(const std::wstring& wstr)
	{
		auto chars_num = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string str(chars_num, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str.data(), str.size(), nullptr, nullptr);
		return str;
	}
}
