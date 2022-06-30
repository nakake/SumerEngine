#pragma once

#include "IFManager.h"
#include "ComPtr.h"
#include "GraphicManager.h"
#include "StringManager.h"
#include "GraphicManager.h"

class DescriptorHandle;
class DescriptorHeap;
class GraphicManager;

class Texture2D
{
public:
	static Texture2D* Get(std::wstring path, const GraphicManager* _gEngine);
	static Texture2D* Get(std::string path, const GraphicManager* _gEngine);
	static Texture2D* GetWhite(const GraphicManager* _gEngine);
	D3D12_SHADER_RESOURCE_VIEW_DESC ViewDesc();
	
	ID3D12Resource* Resouce() {
		return pResouce.Get();
	}

	bool IsValid() {
		return isValid;
	}

	Texture2D(std::wstring path, const GraphicManager* _gEngine) {
		gEngine = _gEngine;
		isValid = Load(path);
	}

	Texture2D(std::string path, const GraphicManager* _gEngine) {
		gEngine = _gEngine;
		isValid = Load(path);
	}

	Texture2D(ID3D12Resource* buffer, const GraphicManager* _gEngine) {
		gEngine = _gEngine;
		pResouce = buffer;
		isValid = pResouce != nullptr;
	}

private:
	bool isValid = false;
	ComPtr<ID3D12Resource> pResouce;
	const GraphicManager* gEngine;
	

	
	bool Load(std::wstring& path);
	bool Load(std::string& path);

	static ID3D12Resource* GetDefaultResouce(size_t width, size_t height, const GraphicManager* _gEngine);


};

