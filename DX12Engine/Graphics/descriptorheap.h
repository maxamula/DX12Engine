#pragma once
#include "core.h"
#include <mutex>
#include <DescriptorHeap.h>
#include <boost/dynamic_bitset.hpp>

namespace engine::gfx
{
	class DescriptorHeap;
	struct ALLOCATION_INFO
	{
		friend class DescriptorHeap;
	public:
		void AddRef() { InterlockedIncrement(&refs); };
		D3D12_CPU_DESCRIPTOR_HANDLE CPU;
		D3D12_GPU_DESCRIPTOR_HANDLE GPU;
	private:
		size_t index;
		ULONG64 refs;
	};
	typedef ALLOCATION_INFO* HEAP_ALLOCATION;

	class DescriptorHeap
	{
	public:
		DescriptorHeap() = default;
		DescriptorHeap(const DescriptorHeap&) = delete;
		DescriptorHeap& operator=(const DescriptorHeap&) = delete;
		DescriptorHeap(DescriptorHeap&&) = delete;
		DescriptorHeap& operator=(DescriptorHeap&&) = delete;
		DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, size_t capacity);
		~DescriptorHeap();

		HEAP_ALLOCATION Allocate();
		void Free(HEAP_ALLOCATION allocation);
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
