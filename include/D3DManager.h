#pragma once

#include "IFManager.h"
#include "WndManager.h"
#include "GraphicManager.h"

class Wndmanager;
class GraphicManager;
class RenderManager;

class D3DManager
{
public:
	D3DManager(){}
	~D3DManager(){}
	bool Init(const WndManager* wnd, uint32_t frameCount);
	void StartRender();
	void EndRender();
	void SetClearColor(float red, float green, float blue, float alpha);
	GraphicManager* GetGraphicManager() {
		return gManager.get();
	}
private:
	std::unique_ptr<GraphicManager>		gManager;
	std::unique_ptr<WndManager>			wManager;

	uint32_t		frameCount;
};

