#pragma once
#include "WndManager.h"

class SumerEngine
{
public:
	SumerEngine(uint32_t width, uint32_t height):
		wndManager(width, height)
	{}
	~SumerEngine(){}
	bool InitEngine();
	void TermEngine();
	bool LoopSetUp();
private:
	WndManager wndManager;
};

