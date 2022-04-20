#pragma once

#include "IFManager.h"
#include "SumerEngine.h"
#include "ComPtr.h"
#include "ErrorMassage.h"

#include <string>

class PipelineState
{
public:
	PipelineState(const GraphicManager* _gEngine);

	void Create();
	void SetVS(std::wstring filePath);
	void SetPS(std::wstring filePath);
	
	void SetInputLayout(D3D12_INPUT_LAYOUT_DESC layout) {
		desc.InputLayout = layout;
	}

	void SetRootSignature(ID3D12RootSignature* rootSignature) {
		desc.pRootSignature = rootSignature;
	}

	bool IsValid() {
		return isValid;
	}

	ID3D12PipelineState* Get() {
		return pPipelineState.Get();
	}

private:
	bool isValid = false;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	ComPtr<ID3D12PipelineState> pPipelineState = nullptr;
	ComPtr<ID3D10Blob> pVSBlob;
	ComPtr<ID3D10Blob> pPSBlob;

	const GraphicManager* gEngine;

};

