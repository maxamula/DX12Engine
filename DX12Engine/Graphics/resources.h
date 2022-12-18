#pragma once
#include "core.h"

namespace engine::gfx
{
	class Texture
	{
	public:
		Texture() = default;
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

	class RenderTexture : Texture
	{
		
	};

	class DepthTexture : Texture
	{

	};
}

