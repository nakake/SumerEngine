#pragma once

#include "IFManager.h"
#include "WndManager.h"
#include "D3DManager.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif//defined(DEBUG) || defined(_DEBUG)

enum SME_FRAMEBUFFER{
	SME_FRAMEBUFFER_SINGLE = 1,
	SME_FRAMEBUFFER_DOUBLE,
	SME_FRAMEBUFFER_TRIPLE
};

class WndManager;
class D3DManager;

class SumerEngine
{
public:
	SumerEngine(uint32_t _width, uint32_t _height):
		width(_width), height(_height), frameCount(2)
	{}

	SumerEngine(uint32_t _width, uint32_t _height, SME_FRAMEBUFFER _frameCount) :
		width(_width), height(_height), frameCount(_frameCount)
	{}

	~SumerEngine(){}
	bool InitEngine();
	bool LoopSetUp();
	void StartRender();
	void EndRender();
	void SetClearColor(float red, float green, float blue, float alpha);
private:
	std::unique_ptr<WndManager>			wndManager;
	std::unique_ptr<D3DManager>			d3dManager;

	uint32_t			frameCount;
	uint32_t			width;
	uint32_t			height;
};

