#include "core.h"

namespace engine::gfx
{
	/////// Global vars //////////////////
	IDXGIFactory7* dxgiFactory = NULL;
	IDXGIAdapter4* dxgiAdapter = NULL;

	ID3D12Device8* d3ddev = NULL;

	D3DDescriptorHeap RTVHeap;
	D3DDescriptorHeap DSVHeap;
	D3DDescriptorHeap SRVHeap;
	D3DDescriptorHeap UAVHeap;

	void Initialize()
	{
#ifdef _DEBUG
		ComPtr<ID3D12Debug3> dbg;
		D3D12GetDebugInterface(IID_PPV_ARGS(&dbg));
		dbg->EnableDebugLayer();
#endif
		if (d3ddev != NULL)
			Shutdown();
		ComPtr<IDXGIFactory2> factory;
		assert(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, __uuidof(IDXGIFactory), (void**)&factory) == S_OK);
		factory->QueryInterface(IID_PPV_ARGS(&dxgiFactory));
		assert(dxgiFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, __uuidof(IDXGIAdapter4), (void**)&dxgiAdapter) == S_OK);
		assert(D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device8), (void**)&d3ddev) == S_OK);
		d3ddev->SetName(L"MAIN_DEVICE");
		new (&RTVHeap) D3DDescriptorHeap(d3ddev, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 512);
		new (&DSVHeap) D3DDescriptorHeap(d3ddev, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 512);
		new (&SRVHeap) D3DDescriptorHeap(d3ddev, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 4096);
		new (&UAVHeap) D3DDescriptorHeap(d3ddev, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 512);
	}

	void Shutdown()
	{
		RELEASE(dxgiFactory);
		RELEASE(dxgiAdapter);
		RELEASE(d3ddev);
		RTVHeap.Release();
		DSVHeap.Release();
		SRVHeap.Release();
		UAVHeap.Release();
	}
}