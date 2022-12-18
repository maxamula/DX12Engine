#include <Windows.h>
#include "Sandbox.h"
#include <string>
int gg = 0; // 14


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    int a = 4;
    int b = 12;
    b = std::move(a);
    int p = 2 + 2;
}