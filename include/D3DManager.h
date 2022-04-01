#pragma once

#include "IFManager.h"
#include "WndManager.h"
#include "GraphicManager.h"
#include "RenderManager.h"

class Wndmanager;
class GraphicManager;
class RenderManager;

class D3DManager
{
public:
	D3DManager(){}
	~D3DManager(){}
	bool InitD3DManager(const WndManager* wnd, uint32_t frameCount);
	void StartRender();
	void EndRender();
	void SetClearColor(float red, float green, float blue, float alpha);
private:
	std::unique_ptr<GraphicManager>		gManager;
	//std::unique_ptr<RenderManager>	rManager;
	std::unique_ptr<WndManager>			wManager;

	uint32_t		frameCount;
};

