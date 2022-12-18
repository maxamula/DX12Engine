#pragma once
#include "core.h"

namespace engine::gfx
{
	struct TEXTURE_DESC
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
		D3D12_RESOURCE_DESC resDesc;
		D3D12_CLEAR_VALUE* clearValue;
		D3D12_RESOURCE_STATES initialState{};
	};

	class Texture
	{
	public:
		Texture() = default;
		// From existing resource
		Texture(D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D12Resource* resource);
		// Place resource
		Texture(TEXTURE_DESC& desc, D3D12_RESOURCE_ALLOCATION_INFO1& info, ID3D12Heap* heap);
		// Create texture
		Texture(TEXTURE_DESC& desc);
		// COPY
		Texture(const Texture&) = delete; // = operator only
		Texture& operator=(const Texture&);
		// MOVE
		Texture(Texture&&);
		Texture& operator=(Texture&&);
		// DESTRUCTOR
		~Texture() { Release(); }
		
		inline ID3D12Resource* Resource() const { return m_res; }
		inline HEAP_ALLOCATION SRVAllocation() const { return m_srv; }
		virtual void Release();

	protected:
		static constexpr uint16_t maxMips{ 10 };
		ID3D12Resource* m_res;
		HEAP_ALLOCATION m_srv;
	};

	class RenderTexture : public Texture
	{
	public:
		RenderTexture() = default;
		RenderTexture(TEXTURE_DESC& desc);
		RenderTexture(TEXTURE_DESC& desc, D3D12_RESOURCE_ALLOCATION_INFO1& info, ID3D12Heap* heap);
		RenderTexture(D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D12Resource* resource);
		// COPY
		RenderTexture(const RenderTexture&) = delete;
		RenderTexture& operator=(const RenderTexture&);
		// MOVE
		RenderTexture(RenderTexture&&);
		RenderTexture& operator=(RenderTexture&&);
		// DESTRUCTOR
		~RenderTexture() { Release(); }

		void Release() override;
	private:
		void _Initialize();
		HEAP_ALLOCATION m_rtv[Texture::maxMips]{};
		uint32_t m_mipCount = 0;

	};

	class DepthTexture : Texture
	{
	public:
		DepthTexture() = default;
		DepthTexture(TEXTURE_DESC& desc);
		// DISABLE COPY
		DepthTexture(const DepthTexture&) = delete;
		DepthTexture& operator=(const DepthTexture&) = delete;
		// MOVE
		DepthTexture(DepthTexture&&);
		DepthTexture& operator=(DepthTexture&&);
		// DESTRUCTOR
		~DepthTexture() { Release(); }

		void Release() override;
	private:
		HEAP_ALLOCATION m_dsv;
	};
}

