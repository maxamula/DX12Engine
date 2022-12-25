#include "window.h"

namespace engine
{
	LRESULT CALLBACK WndProcBase(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		Window* This = (Window*)GetWindowLongPtr(hwnd, 0);
		switch (msg)
		{
		case WM_CLOSE:
			This->CloseWnd();
			break;
		case WM_DESTROY:
			This->m_surface.Release();
			delete This;
			return DefWindowProc(hwnd, msg, wparam, lparam);
			break;
		case WM_NCDESTROY:
			return DefWindowProc(hwnd, msg, wparam, lparam);
			break;
		case WM_EXITSIZEMOVE:
			GetClientRect(hwnd, &This->m_clientRect);
			break;
		case WM_SIZE:	// TODO resize surface
			if (wparam == SIZE_MAXIMIZED)
				GetClientRect(hwnd, &This->m_clientRect);
			break;
		case WM_SYSCOMMAND:
			if (wparam == SC_RESTORE)
				GetClientRect(hwnd, &This->m_clientRect);
			break;
		case WM_QUIT:
			This->Destroy();
			break;
		}
		return (This && This->m_callback) ? This->m_callback(This, hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
	}

	Window::Window(HINSTANCE hInst, GFX_WND_DESC& desc)
		: m_hInst(hInst), m_parent(desc.hParent), m_callback(desc.callback), m_caption(desc.szCaption), m_width(desc.width), m_height(desc.height)
	{
		RECT rect{ desc.left, desc.top, desc.left + desc.width, desc.top + desc.height };
		m_dwStyle = desc.hParent != NULL ? WS_CHILD : WS_OVERLAPPEDWINDOW;
		AdjustWindowRect(&rect, m_dwStyle, FALSE);
		m_hWnd = CreateWindow(WND_CLASS, m_caption.c_str(), m_dwStyle, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, m_parent, NULL, hInst, NULL);
		GetWindowRect(m_hWnd, &m_wndRect);
		GetClientRect(m_hWnd, &m_clientRect);
		auto d = SetWindowLongPtr(m_hWnd, 0, (LONG_PTR)this);

		// create rtv && swapchain
		new (&m_surface) gfx::RenderSurface(m_hWnd, m_width, m_height);
	}

	void Window::Initialize()
	{
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProcBase;
		wc.cbClsExtra = 0;
		wc.lpszClassName = WND_CLASS;
		wc.cbWndExtra = sizeof(void*);	// 8 bytes
		wc.hInstance = GetModuleHandle(NULL);
		wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);

		RegisterClassEx(&wc);
	}

	void Window::ShowWnd()
	{
		if (m_bFullscreen)
		{
			GetWindowRect(m_hWnd, &m_wndRect);
			m_dwStyle = 0;
			SetWindowLongPtr(m_hWnd, GWL_STYLE, m_dwStyle);
			ShowWindow(m_hWnd, SW_MAXIMIZE);
		}
		else
		{
			m_dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
			SetWindowLongPtr(m_hWnd, GWL_STYLE, m_dwStyle);
			MoveWindow(m_hWnd, m_wndRect.left, m_wndRect.top, m_wndRect.right, m_wndRect.bottom, true);
			ShowWindow(m_hWnd, SW_SHOWNORMAL);
		}
	}

	void Window::CloseWnd()
	{
		m_bClosed = true;
		::CloseWindow(m_hWnd);
	}

	void Window::SetWindowSize(uint16_t width, uint16_t height)
	{
		if (m_dwStyle & WS_CHILD)
		{
			GetClientRect(m_hWnd, &m_clientRect);
			return;
		}
		m_width = width;
		m_height = height;
		m_wndRect.right += width - m_wndRect.right;
		m_wndRect.bottom += height - m_wndRect.bottom;
		AdjustWindowRect(&m_wndRect, m_dwStyle, FALSE);
		MoveWindow(m_hWnd, m_wndRect.left, m_wndRect.top, m_wndRect.right, m_wndRect.bottom, true);

		GetClientRect(m_hWnd, &m_clientRect);
	}

	void Window::SetWindowCaption(const wchar_t* szCaption)
	{
		SetWindowText(m_hWnd, szCaption);
		m_caption = std::wstring(szCaption);
	}

	void Window::Fullscreen(bool bState)
	{

	}
}