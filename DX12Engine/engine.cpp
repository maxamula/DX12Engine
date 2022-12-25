#include "engine.h"
#include "Graphics/core.h"

namespace engine
{
	// Global vars
	HMODULE hModule = NULL;

	void Initialize()
	{
		gfx::Initialize();
	}

	void Shutdown()
	{
		gfx::Shutdown();
	}
}