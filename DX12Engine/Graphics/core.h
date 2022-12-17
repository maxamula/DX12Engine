#pragma once
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d12.h>

#include <assert.h>
#include <stdint.h>
#include <string>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

#define MOVE(a) a; a = {}; 

#define RELEASE(res) if(res){res->Release(); res = NULL;}
#define FRAMEBUFFERS_COUNT 3

using namespace Microsoft::WRL;

namespace engine::gfx
{
	extern IDXGIFactory7* dxgiFactory;
	extern IDXGIAdapter4* dxgiAdapter;
	extern ID3D12Device8* d3ddev;

	void Initialize();
	void Shutdown();
}

