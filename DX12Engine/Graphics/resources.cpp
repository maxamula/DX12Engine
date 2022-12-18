#include "resources.h"

namespace engine::gfx
{
	/////// TEXTURE /////////////////////

	Texture::Texture(D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D12Resource* resource)
		: m_res(resource)
	{
		assert(d3ddev);
		m_srv = SRVHeap.Allocate();
		d3ddev->CreateShaderResourceView(m_res, srvDesc, m_srv->CPU);
	}
	
	Texture::Texture(TEXTURE_DESC& desc, D3D12_RESOURCE_ALLOCATION_INFO1& info, ID3D12Heap* heap)
	{
		assert(d3ddev);
		const D3D12_CLEAR_VALUE* const clearVal = (desc.resDesc->Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET || desc.resDesc->Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) ? desc.clearValue : NULL;
		d3ddev->CreatePlacedResource(heap, info.Offset, desc.resDesc, desc.initialState, clearVal, IID_PPV_ARGS(&m_res));
		m_srv = SRVHeap.Allocate();
		d3ddev->CreateShaderResourceView(m_res, desc.srvDesc, m_srv->CPU);
	}
	
	Texture::Texture(TEXTURE_DESC& desc)
	{
		assert(d3ddev);
		const D3D12_CLEAR_VALUE* const clearVal = (desc.resDesc->Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET || desc.resDesc->Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) ? desc.clearValue : NULL;
		D3D12_HEAP_PROPERTIES heapProps;
		ZeroMemory(&heapProps, sizeof(D3D12_HEAP_PROPERTIES));
		heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
		d3ddev->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, desc.resDesc, desc.initialState, clearVal, IID_PPV_ARGS(&m_res));
		m_srv = SRVHeap.Allocate();
		d3ddev->CreateShaderResourceView(m_res, desc.srvDesc, m_srv->CPU);
	}

	Texture& Texture::operator=(const Texture& o)
	{
		assert(&o != this);
		m_res = o.m_res;
		o.m_res->AddRef();
		m_srv = o.m_srv;
		o.m_srv->AddRef();
		return *this;
	}

	Texture::Texture(Texture&& o)
	{
		assert(&o != this);
		m_res = MOVE(m_res);
		m_srv = MOVE(m_srv);
	}

	Texture& Texture::operator=(Texture&& o)
	{
		assert(&o != this);
		m_res = MOVE(m_res);
		m_srv = MOVE(m_srv);
		return *this;
	}

	void Texture::Release()
	{
		SRVHeap.Free(m_srv);
		RELEASE(m_res);
	}

	////// RENDER TEXTURE ///////////////////
}