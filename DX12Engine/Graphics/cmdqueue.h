#pragma once
#include "core.h"

namespace engine::gfx
{
	class CommandQueue
	{
	public:
		CommandQueue() = default;
		CommandQueue(const CommandQueue&) = delete;
		CommandQueue& operator=(const CommandQueue&) = delete;
		CommandQueue(CommandQueue&&) = delete;
		CommandQueue& operator=(CommandQueue&&) = delete;
		CommandQueue(D3D12_COMMAND_LIST_TYPE type);

		void BeginFrame();
		void EndFrame();
		void Flush();
		void Release();
		inline ID3D12CommandQueue* CmdQueue() const { return m_cmdQueue; }
		inline ID3D12GraphicsCommandList6* CmdList() const { return m_cmdList; }
		inline uint8_t CurrentFrame() const { return m_iFrame; }

	private:
		void WaitGPU(HANDLE event, ID3D12Fence1* pFence);	// wait if gpu is busy while executing commands

		ID3D12CommandQueue* m_cmdQueue = NULL;
		ID3D12GraphicsCommandList6* m_cmdList = NULL;
		ID3D12CommandAllocator* m_cmdAlloc[3];
		ID3D12Fence1* m_fence = NULL;
		HANDLE m_fenceEvent = NULL;
		uint64_t m_fenceValue = 0;
		uint8_t m_iFrame = 0;
	};
}
