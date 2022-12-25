#include "engine.h"
#include "../Graphics/core.h"
#include "window.h"

namespace engine
{
	// Global vars
	HMODULE hModule = NULL;

	void Initialize()
	{
		Window::Initialize();
		gfx::Initialize();
	}

	void Shutdown()
	{
		gfx::Shutdown();
		Window::Shutdown();	
	}
}