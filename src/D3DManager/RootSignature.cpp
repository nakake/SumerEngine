#include "RootSignature.h"

RootSignature::RootSignature(const GraphicManager* gEngine) {
    auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; 
    flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS; 
    flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

    CD3DX12_ROOT_PARAMETER rootParam[2] = {};
    rootParam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

    CD3DX12_DESCRIPTOR_RANGE tableRange[1] = {};
    tableRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    rootParam[1].InitAsDescriptorTable(std::size(tableRange), tableRange, D3D12_SHADER_VISIBILITY_ALL);

    auto sampler = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

    D3D12_ROOT_SIGNATURE_DESC desc  = {};
    desc.NumParameters              = std::size(rootParam);
    desc.NumStaticSamplers          = 1;
    desc.pParameters                = rootParam;
    desc.pStaticSamplers            = &sampler;
    desc.Flags                      = flag;

    ComPtr<ID3DBlob> pBlob;
    ComPtr<ID3DBlob> pErrorBlob;

    auto hr = D3D12SerializeRootSignature(
        &desc,
        D3D_ROOT_SIGNATURE_VERSION_1_0,
        pBlob.GetAddressOf(),
        pErrorBlob.GetAddressOf()
    );
    
    if (FAILED(hr)) {
        ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("ルートシグネチャシリアライズに失敗"));
        return;
    }

    hr = gEngine->GetDevice()->CreateRootSignature(
        0,
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        IID_PPV_ARGS(pRootSignature.GetAddressOf())
    );

    if (FAILED(hr)) {
        ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("ルートシグネチャの生成に失敗"));
        return;
    }

    isValid = true;
}
