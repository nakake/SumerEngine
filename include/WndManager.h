#pragma once

#include "IFManager.h"

class WndManager
{
public:
	WndManager(uint32_t _width, uint32_t _height, const TCHAR* _appName) :
		hInst(nullptr), hWnd(nullptr), Width(_width), Height(_height), appName(_appName)
	{}

	WndManager(const WndManager& wnd) :
		hInst(wnd.hInst), hWnd(wnd.hWnd), Width(wnd.Width), Height(wnd.Height)
	{}

	WndManager(){}

	~WndManager(){
		TermWndManager();
	}

	HWND getHandleWnd() {
		return this->hWnd;
	}

	uint32_t GetWidth() {
		return this->Width;
	}

	uint32_t GetHeight() {
		return this->Height;
	}

	void Copy(const WndManager* wnd);
	bool Init();
	

private:
	HINSTANCE		hInst;
	HWND			hWnd;
	uint32_t		Width;
	uint32_t		Height;
	const TCHAR*	appName;
	
	void TermWndManager();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};

