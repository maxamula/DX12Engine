#pragma once
#include "common.h"

namespace engine { class Window; }
typedef LRESULT(__stdcall* WndProcFnPtr)(engine::Window* This, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace engine
{
	ENGINE_API struct GFX_WND_DESC
	{
		WndProcFnPtr callback = NULL;
		HWND hParent = NULL;
		wchar_t* szCaption = (wchar_t*)L"Game";
		uint16_t left = 0;
		uint16_t top = 0;
		uint16_t width = 1280;
		uint16_t height = 720;
	};

	ENGINE_API class IWindow
	{
	public:
		virtual void Destroy() = 0;
		virtual void ShowWnd() = 0;
		virtual void CloseWnd() = 0;
		virtual void SetWindowSize(uint16_t width, uint16_t height) = 0;
		virtual void SetWindowCaption(const wchar_t* szCaption) = 0;
		virtual void Fullscreen(bool bState) = 0;

		virtual bool IsFullscreen() const = 0;
		virtual bool IsClosed() const = 0;
		virtual uint16_t Width() const = 0;
		virtual uint16_t Height() const = 0;
		virtual HWND WinId() const = 0;
	};
}