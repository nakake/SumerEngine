#pragma once

#include "IFManager.h"
#include "SumerEngine.h"
#include "GraphicManager.h"
#include "ShaderSturuct.h"

class Scene
{
public:
	Scene(GraphicManager* gManager, uint32_t _frameCount):frameCount(_frameCount)
	{
		gEngine = gManager;
	}
	bool Init();

	void Update();
	void Draw();
private:
	GraphicManager* gEngine;
	uint32_t		frameCount;
	float			rotateX;
	float			rotateY;
	float			rotateZ;
};

