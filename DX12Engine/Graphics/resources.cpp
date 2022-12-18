#include "resources.h"

namespace engine::gfx
{
#pragma region TEXTURE
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
		const D3D12_CLEAR_VALUE* const clearVal = (desc.resDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET || desc.resDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) ? desc.clearValue : NULL;
		d3ddev->CreatePlacedResource(heap, info.Offset, &desc.resDesc, desc.initialState, clearVal, IID_PPV_ARGS(&m_res));
		m_srv = SRVHeap.Allocate();
		d3ddev->CreateShaderResourceView(m_res, &desc.srvDesc, m_srv->CPU);
	}
	
	Texture::Texture(TEXTURE_DESC& desc)
	{
		assert(d3ddev);
		const D3D12_CLEAR_VALUE* const clearVal = (desc.resDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET || desc.resDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) ? desc.clearValue : NULL;
		D3D12_HEAP_PROPERTIES heapProps;
		ZeroMemory(&heapProps, sizeof(D3D12_HEAP_PROPERTIES));
		heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
		d3ddev->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc.resDesc, desc.initialState, clearVal, IID_PPV_ARGS(&m_res));
		m_srv = SRVHeap.Allocate();
		d3ddev->CreateShaderResourceView(m_res, &desc.srvDesc, m_srv->CPU);
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
		Release();
		m_res = MOVE(m_res);
		m_srv = MOVE(m_srv);
	}

	Texture& Texture::operator=(Texture&& o)
	{
		assert(&o != this);
		Release();
		m_res = MOVE(m_res);
		m_srv = MOVE(m_srv);
		return *this;
	}

	void Texture::Release()
	{
		SRVHeap.Free(m_srv);
		RELEASE(m_res);
	}
#pragma endregion
#pragma region RENDER TEXTURE

	RenderTexture::RenderTexture(TEXTURE_DESC& desc)
		: Texture(desc)
	{
		_Initialize();
	}
	RenderTexture::RenderTexture(TEXTURE_DESC& desc, D3D12_RESOURCE_ALLOCATION_INFO1& info, ID3D12Heap* heap)
		: Texture(desc, info, heap)
	{
		_Initialize();
	}
	RenderTexture::RenderTexture(D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D12Resource* resource)
		: Texture(srvDesc, resource)
	{
		_Initialize();
	}
	// COPY
	RenderTexture& RenderTexture::operator=(const RenderTexture& o)
	{
		Texture::operator=(o);
		m_mipCount = o.m_mipCount;
		for (uint16_t i = 0; i < m_mipCount; i++)
		{
			m_rtv[i] = o.m_rtv[i];
			o.m_rtv[i]->AddRef();
		}
		return *this;
	}
	// MOVE
	RenderTexture::RenderTexture(RenderTexture&& o)
		: Texture(std::move(o))
	{
		m_mipCount = MOVE(o.m_mipCount);
		for (uint16_t i = 0; i < m_mipCount; i++)
		{
			m_rtv[i] = MOVE(o.m_rtv[i]);
		}		
	}
	RenderTexture& RenderTexture::operator=(RenderTexture&& o)
	{
		Texture::operator=(std::move(o));
		m_mipCount = MOVE(o.m_mipCount);
		for (uint16_t i = 0; i < m_mipCount; i++)
		{
			m_rtv[i] = MOVE(o.m_rtv[i]);
		}
		return *this;
	}

	void RenderTexture::Release()
	{
		Texture::Release();
		for (uint16_t i = 0; i < m_mipCount; i++)
		{
			RTVHeap.Free(m_rtv[i]);
		}
		m_mipCount = 0;
	}

	void RenderTexture::_Initialize()
	{
		assert(d3ddev);
		D3D12_RESOURCE_DESC desc = m_res->GetDesc();
		m_mipCount = desc.MipLevels;
		assert(m_mipCount && m_mipCount < Texture::maxMips);

		D3D12_RENDER_TARGET_VIEW_DESC rtvdesc{};
		rtvdesc.Format = desc.Format;
		rtvdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvdesc.Texture2D.MipSlice = 0;
		for (uint16_t i = 0; i < m_mipCount; i++)
		{
			m_rtv[i] = RTVHeap.Allocate();
			d3ddev->CreateRenderTargetView(m_res, &rtvdesc, m_rtv[i]->CPU);
			rtvdesc.Texture2D.MipSlice++;
		}
	}
#pragma endregion
#pragma region DEPTH TEXTURE
	DepthTexture::DepthTexture(TEXTURE_DESC& desc)
	{
		assert(d3ddev);
		const DXGI_FORMAT dsvFormat = desc.resDesc.Format;
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
		if (desc.resDesc.Format == DXGI_FORMAT_D32_FLOAT)
		{
			desc.resDesc.Format = DXGI_FORMAT_R32_TYPELESS;
			srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.PlaneSlice = 0;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		
		desc.srvDesc = srvDesc;
		Texture::Texture(desc);

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.Format = dsvFormat;
		dsvDesc.Texture2D.MipSlice = 0;

		m_dsv = DSVHeap.Allocate();
		d3ddev->CreateDepthStencilView(m_res, &dsvDesc, m_dsv->CPU);
	}
	DepthTexture::DepthTexture(DepthTexture&& o)
		: Texture(std::move(o))
	{
		m_dsv = MOVE(o.m_dsv);
	}
	DepthTexture& DepthTexture::operator=(DepthTexture&& o)
	{
		Texture::operator=(std::move(o));
		m_dsv = MOVE(o.m_dsv);
		return *this;
	}

	void DepthTexture::Release()
	{
		DSVHeap.Free(m_dsv);
		Texture::Release();
	}
#pragma endregion
}