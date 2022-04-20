#include "WndManager.h"

bool WndManager::Init() {
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
	wc.lpszClassName	= appName;
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
		appName,
		appName,
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


void WndManager::TermWndManager() {
	if (hInst != nullptr) {
		UnregisterClass(appName, hInst);
	}
	hInst = nullptr;
	hWnd  = nullptr;
}

void WndManager::Copy(const WndManager* wnd) {
	hInst	= wnd->hInst;
	hWnd	= wnd->hWnd;
	Width	= wnd->Width;
	Height	= wnd->Height;
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