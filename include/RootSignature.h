#pragma once

#include "IFManager.h"
#include "GraphicManager.h"
#include "ErrorMassage.h"
#include "ComPtr.h"

class RootSignature
{
public:
	RootSignature(const GraphicManager* gEngine);
	
	ID3D12RootSignature* Get() {
		return pRootSignature.Get();
	}
	
	bool IsValid() {
		return isValid;
	}

private:
	bool isValid = false;
	ComPtr<ID3D12RootSignature> pRootSignature = nullptr;
};

