#pragma once
#include "API/engine.h"
#include <windows.h>

namespace engine
{
	extern HMODULE hModule;
	void Initialize();
	void Shutdown();
}

