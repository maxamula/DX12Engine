#include "cmdqueue.h"

namespace engine::gfx
{
	CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type)
	{
		assert(d3ddev);
		D3D12_COMMAND_QUEUE_DESC qd{};
		qd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		qd.NodeMask = 0;
		qd.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		qd.Type = type;
		d3ddev->CreateCommandQueue(&qd, __uuidof(ID3D12CommandQueue), (void**)&m_cmdQueue);
		for (uint8_t i = 0; i < FRAMEBUFFERS_COUNT; i++)
		{
			auto m = d3ddev->CreateCommandAllocator(type, __uuidof(ID3D12CommandAllocator), (void**)&m_cmdAlloc[i]);
		}
		d3ddev->CreateCommandList(0, type, m_cmdAlloc[0], NULL, __uuidof(ID3D12GraphicsCommandList6), (void**)&m_cmdList);
		m_cmdList->Close();
		d3ddev->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence1), (void**)&m_fence);
		m_fenceEvent = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
	}

	void CommandQueue::BeginFrame()
	{
		ID3D12CommandAllocator* allocator = m_cmdAlloc[m_iFrame];
		WaitGPU(m_fenceEvent, m_fence);
		allocator->Reset();
		m_cmdList->Reset(allocator, NULL);
	}

	void CommandQueue::EndFrame()
	{
		m_cmdList->Close();
		ID3D12CommandList* const cmdLists[] = { m_cmdList };
		m_cmdQueue->ExecuteCommandLists(_countof(cmdLists), &cmdLists[0]);
		m_fenceValue++;
		m_cmdQueue->Signal(m_fence, m_fenceValue);
		m_iFrame = (m_iFrame + 1) % FRAMEBUFFERS_COUNT;
	}

	void CommandQueue::Flush()
	{
		for (uint32_t i = 0; i < FRAMEBUFFERS_COUNT; i++)
		{
			WaitGPU(m_fenceEvent, m_fence);
			m_iFrame = (m_iFrame + 1) % FRAMEBUFFERS_COUNT;
		}
	}

	void CommandQueue::Release()
	{
		Flush();
		RELEASE(m_fence);
		m_fenceValue = 0;
		if (m_fenceEvent)
			CloseHandle(m_fenceEvent);
		m_fenceEvent = NULL;
		RELEASE(m_cmdQueue);
		RELEASE(m_cmdList);
		for (uint32_t i = 0; i < FRAMEBUFFERS_COUNT; i++)
			RELEASE(m_cmdAlloc[i]);
	}

	void CommandQueue::WaitGPU(HANDLE event, ID3D12Fence1* pFence)
	{
		if (pFence->GetCompletedValue() < m_fenceValue) // if else then framebuffer command queue is empty
		{
			m_fence->SetEventOnCompletion(m_fenceValue, event);
			WaitForSingleObject(event, INFINITE);
		}
	}
}