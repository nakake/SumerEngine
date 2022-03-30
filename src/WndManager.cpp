#include "WndManager.h"

namespace {
	const auto ClassName = TEXT("SumerEngineWindowClass");
}

bool WndManager::InitWnd() {
	auto _hInst = GetModuleHandle(nullptr);
	if (_hInst == nullptr) {
		return  false;
	}

	hInst = _hInst;

	WNDCLASSEX wc = {};
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.hIcon			= LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground	= GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName		= nullptr;
	wc.lpszClassName	= ClassName;
	wc.hIconSm			= LoadIcon(hInst, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		return false;
	}

	RECT rc = {};
	rc.right	= static_cast<LONG>(Width);
	rc.bottom	= static_cast<LONG>(Height);

	auto style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&rc, style, FALSE);

	hWnd = CreateWindowEx(
		0, 
		ClassName,
		TEXT("SumerEngine"),
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInst,
		nullptr
	);

	if (hWnd == nullptr) {
		return false;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	return true;
}

void WndManager::TermWnd() {
	if (hInst != nullptr) {
		UnregisterClass(ClassName, hInst);
	}
	hInst = nullptr;
	hWnd  = nullptr;
}

LRESULT CALLBACK WndManager::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
		case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			break;
		default:
			{}
			break;
	}

	return DefWindowProc(hWnd, msg, wp, lp);
}