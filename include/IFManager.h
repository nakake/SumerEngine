#pragma once

#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXTex.h>
#include <d3dx12.h>
#include <wrl/client.h>
#include <cassert>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <vector>
#include <memory>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

enum SME_FRAMEBUFFER {
	SME_FRAMEBUFFER_SINGLE = 1,
	SME_FRAMEBUFFER_DOUBLE,
	SME_FRAMEBUFFER_TRIPLE
};