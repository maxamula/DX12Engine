#include "engine.h"
#include <Windows.h>

BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        engine::hModule = hinstDLL;
        break;
    case DLL_THREAD_ATTACH:

        break;
    case DLL_THREAD_DETACH:

        break;
    case DLL_PROCESS_DETACH:

        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}