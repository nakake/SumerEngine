#include "StringManager.h"

namespace fs = std::filesystem;

std::wstring GetDirectoryPath(const std::wstring& origin) {
	fs::path p = origin.c_str();
	return p.remove_filename().c_str();
}

std::string ToUTF8(const std::wstring& value) {
	auto length = WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
	auto buffer = new char[length];

	WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

	std::string result(buffer);
	delete[] buffer;
	buffer = nullptr;

	return result;
}

std::wstring ToWideString(const std::string& str) {
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
	return wstr;
}

std::wstring FileExtension(const std::wstring& path) {
	auto idx = path.rfind(L".");
	return path.substr(idx + 1, path.length() - idx - 1);
}

std::wstring ReplaceExtension(const std::wstring& origin, const char* ext) {
	fs::path p = origin.c_str();
	return p.replace_extension(ext).c_str();
}