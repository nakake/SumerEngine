#pragma once

#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <cassert>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <vector>
#include <memory>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;


#include "D3DManager.h"
#include "GraphicManager.h"
#include "RenderManager.h"
#include "SumerEngine.h"
#include "WndManager.h"