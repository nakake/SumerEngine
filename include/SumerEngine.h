#pragma once

#include "IFManager.h"
#include "WndManager.h"
#include "D3DManager.h"
#include "Scene.h"
#include "ComPtr.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif//defined(DEBUG) || defined(_DEBUG)

class WndManager;
class D3DManager;
class Scene;

class SumerEngine
{
public:
	SumerEngine(uint32_t _width, uint32_t _height, const TCHAR* _appName):
		width(_width), height(_height), frameCount(2), appName(_appName)
	{ Init(); }

	SumerEngine(uint32_t _width, uint32_t _height, SME_FRAMEBUFFER _frameCount) :
		width(_width), height(_height), frameCount(_frameCount)
	{ Init(); }

	~SumerEngine(){}
	
	void StartEngine();

private:
	bool Init();
	void MainLoop();
	void StartRender();
	void EndRender();
	void SetClearColor(float red, float green, float blue, float alpha);
	
	std::unique_ptr<WndManager>			wndManager;
	std::unique_ptr<D3DManager>			d3dManager;
	std::unique_ptr<Scene>				m_Scene;

	uint32_t			frameCount;
	uint32_t			width;
	uint32_t			height;
	const TCHAR*		appName;
	
};

