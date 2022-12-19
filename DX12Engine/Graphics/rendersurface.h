#pragma once
#include "core.h"

namespace engine::gfx
{
	struct RENDER_TARGET
	{
		ID3D12Resource* resource = NULL;
		HEAP_ALLOCATION allocation = NULL;
	};

	class RenderSurface
	{
	public:
		RenderSurface() = default;
		RenderSurface(const RenderSurface&) = delete;
		RenderSurface& operator=(const RenderSurface&) = delete;
		RenderSurface(RenderSurface&&) = delete;
		RenderSurface& operator=(RenderSurface&&) = delete;
		RenderSurface(HWND hWnd, unsigned short width, unsigned short height);
		~RenderSurface() { assert(m_pSwap == NULL); };

		inline void Present()
		{
			m_pSwap->Present(1, 0);
			m_backBufferIndex = m_pSwap->GetCurrentBackBufferIndex();
		}
		inline D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle(unsigned int index)
		{
			assert(index < FRAMEBUFFERS_COUNT);
			return m_renderTargets[index].allocation->CPU;
		}

		//TODO: add resizing
		void Release();
	private:
		void CreateRendertargetViews();

		IDXGISwapChain4* m_pSwap = NULL;
		D3D12_VIEWPORT m_viewport{};
		RENDER_TARGET m_renderTargets[FRAMEBUFFERS_COUNT]{};
		byte m_backBufferIndex = 0;

		HWND m_hWnd = NULL;
		unsigned short m_width = 0;
		unsigned short m_height = 0;
	};
}

