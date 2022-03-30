#pragma once

#include <Windows.h>
#include <cstdint>

class WndManager
{
public:
	WndManager(uint32_t _width, uint32_t _height) :
		hInst(nullptr), hWnd(nullptr), Width(_width), Height(_height)
	{}
	~WndManager(){}

	HWND getHandleWnd() {
		return this->hWnd;
	}


	bool InitWnd();
	void TermWnd();

private:
	HINSTANCE	hInst;
	HWND		hWnd;
	uint32_t	Width;
	uint32_t	Height;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};

