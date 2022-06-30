#include "Texture2D.h"

bool Texture2D::Load(std::string& path) {
	auto wpath = ToWideString(path);
	return Load(wpath);
}

bool Texture2D::Load(std::wstring& path) {
	DirectX::TexMetadata	meta = {};
	DirectX::ScratchImage	scratch = {};

	auto ext = FileExtension(path);

	HRESULT hr = S_FALSE;
	if (ext == L"png") {
		DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, &meta, scratch);
	}
	else if(ext == L"tga") {
		hr = DirectX::LoadFromTGAFile(path.c_str(), &meta, scratch);
	}

	if (FAILED(hr)) {
		return false;
	}

	auto img  = scratch.GetImage(0, 0, 0);
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(
		meta.format,
		meta.width,
		meta.height,
		static_cast<UINT16>(meta.arraySize),
		static_cast<UINT16>(meta.mipLevels)
	);

	hr = gEngine->GetDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(pResouce.ReleaseAndGetAddressOf())
	);

	if (FAILED(hr)) {
		return false;
	}

	hr = pResouce->WriteToSubresource(
		0, 
		nullptr,
		img->pixels,
		static_cast<UINT>(img->rowPitch),
		static_cast<UINT>(img->slicePitch)
	);

	if (FAILED(hr)) {
		return false;
	}

	return true;
}

Texture2D* Texture2D::Get(std::string path, const GraphicManager* _gEngine) {
	auto wpath = ToWideString(path);
	return Get(wpath, _gEngine);
}

Texture2D* Texture2D::Get(std::wstring path, const GraphicManager* _gEngine) {
	Texture2D* tex;
	tex = new Texture2D(path, _gEngine);
	if (!tex->IsValid()) {
		return GetWhite(_gEngine);
	}

	return tex;
}

Texture2D* Texture2D::GetWhite(const GraphicManager* _gEngine) {
	ID3D12Resource* buff = GetDefaultResouce(4, 4, _gEngine);
	
	std::vector<unsigned char> data(4 * 4 * 4);
	std::fill(data.begin(), data.end(), 0xff);

	auto hr = buff->WriteToSubresource(0, nullptr, data.data(), 4 * 4, data.size());
	if (FAILED(hr)) {
		return nullptr;
	}

	Texture2D* tex;
	tex = new Texture2D(buff, _gEngine);
	return tex;
}


ID3D12Resource* Texture2D::GetDefaultResouce(size_t width, size_t height, const GraphicManager* _gEngine) {
	auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	ID3D12Resource* buff = nullptr;

	auto result = _gEngine->GetDevice()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&buff)
	);
	if (FAILED(result)) {
		assert(SUCCEEDED(result));
		return nullptr;
	}

	return buff;
}

D3D12_SHADER_RESOURCE_VIEW_DESC Texture2D::ViewDesc() {
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format						= pResouce->GetDesc().Format;
	desc.Shader4ComponentMapping	= D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension				= D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels		= 1;
	return desc;
}