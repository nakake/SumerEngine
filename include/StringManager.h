#pragma once

#include "IFManager.h"

#include <filesystem>

std::wstring GetDirectoryPath(const std::wstring& origin);
std::string ToUTF8(const std::wstring& value);
std::wstring ToWideString(const std::string& str);
//拡張子を返す
std::wstring FileExtension(const std::wstring& path);

std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);