#pragma once
#include <stdint.h>
#include <windows.h>
#include <string>


#ifdef _ENGINE
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif