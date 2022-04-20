#include "PipelineState.h"

PipelineState::PipelineState(const GraphicManager* _gEngine) {
	gEngine							= _gEngine;
	desc.RasterizerState			= CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.RasterizerState.CullMode	= D3D12_CULL_MODE_NONE;
	desc.BlendState					= CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.DepthStencilState			= CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	desc.SampleMask					= UINT_MAX;
	desc.PrimitiveTopologyType		= D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets			= 1;
	desc.RTVFormats[0]				= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.DSVFormat					= DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count			= 1;
	desc.SampleDesc.Quality			= 0;
}

void PipelineState::SetVS(std::wstring filePath) {
	auto hr = D3DReadFileToBlob(filePath.c_str(), pVSBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("頂点シェーダーの読み込みに失敗"));
		return;
	}

	desc.VS = CD3DX12_SHADER_BYTECODE(pVSBlob.Get());
}

void PipelineState::SetPS(std::wstring filePath) {
	auto hr = D3DReadFileToBlob(filePath.c_str(), pPSBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("ピクセルシェーダーの読み込みに失敗"));
		return;
	}

	desc.PS = CD3DX12_SHADER_BYTECODE(pPSBlob.Get());
}

void PipelineState::Create() {
	auto hr = gEngine->GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pPipelineState.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) {
		ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("パイプラインステートの生成に失敗"));
		return;
	}

	isValid = false;
}