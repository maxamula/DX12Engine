#pragma once
#include "core.h"
#include <mutex>
#include <DescriptorHeap.h>
#include <boost/dynamic_bitset.hpp>

namespace engine::gfx
{
	struct HEAP_ALLOCATION
	{
		size_t index;
		D3D12_CPU_DESCRIPTOR_HANDLE CPU;
		D3D12_GPU_DESCRIPTOR_HANDLE GPU;
	};

	class D3DDescriptorHeap
	{
	public:
		D3DDescriptorHeap() = default;
		D3DDescriptorHeap(const D3DDescriptorHeap&) = delete;
		D3DDescriptorHeap& operator=(const D3DDescriptorHeap&) = delete;
		D3DDescriptorHeap(D3DDescriptorHeap&&) = delete;
		D3DDescriptorHeap& operator=(D3DDescriptorHeap&&) = delete;
		D3DDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, size_t capacity);
		~D3DDescriptorHeap();

		HEAP_ALLOCATION Allocate();
		void Free(HEAP_ALLOCATION& allocation);
		inline DirectX::DescriptorHeap* Heap() const { return m_heap.get(); }
		inline void Release() { m_heap.reset(); }

	private:
		size_t GetFreeIndex() const;

		std::unique_ptr<DirectX::DescriptorHeap> m_heap{};
		boost::dynamic_bitset<unsigned int> m_available;
		std::mutex m_mutex;

		size_t m_size = 0;
		size_t m_cap = 0;
		bool m_shaderVisible = false;
	};
}
