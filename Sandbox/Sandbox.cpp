#include <Windows.h>
#include "engine.h"
#include <dxcapi.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <d3d12.h>
#include <D3DCompiler.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    engine::Initialize();
    engine::Shutdown();
    while(true){}
}