#pragma once

#include "IFManager.h"

inline void ErrorMessage(const HWND wndHandle, const TCHAR* errorMas) {
	MessageBox(wndHandle, errorMas, TEXT("�G���["), MB_OK);
}