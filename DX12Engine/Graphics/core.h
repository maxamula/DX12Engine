#pragma once
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d12.h>

#include <assert.h>
#include <stdint.h>
#include <string>

#include "cmdqueue.h"
#include "descriptorheap.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

#define MOVE(a) a; a = {}; 

#define RELEASE(res) if(res){res->Release(); res = NULL;}

using namespace Microsoft::WRL;

constexpr int FRAMEBUFFERS_COUNT = 3;

namespace engine::gfx
{
	class CommandQueue;
	class DescriptorHeap;

	extern IDXGIFactory7* dxgiFactory;
	extern IDXGIAdapter4* dxgiAdapter;
	extern ID3D12Device8* d3ddev;
	extern CommandQueue d3dcmd;
	extern DescriptorHeap RTVHeap;
	extern DescriptorHeap DSVHeap;
	extern DescriptorHeap SRVHeap;
	extern DescriptorHeap UAVHeap;

	void Initialize();
	void Shutdown();
}

