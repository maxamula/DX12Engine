#pragma once
#include "core.h"
#include "../Content/ContentManager.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

namespace engine::gfx
{
	enum SHADER_TYPE : uint32_t
	{
		SHADER_TYPE_VERTEX,
		SHADER_TYPE_HULL,
		SHADER_TYPE_DOMAIN,
		SHADER_TYPE_GEOMETRY,
		SHADER_TYPE_PIXEL,
		SHADER_TYPE_COMPUTE,
		SHADER_TYPE_AMPLIFICATION,
		SHADER_TYPE_MESH
	};

	enum SHADER : uint32_t
	{
		VS_FULSCREEN_TRIANGLE,
		Count
	};


	D3D12_SHADER_BYTECODE GetEngineShader(SHADER id);

	bool InitializeShaders();
	void ShutdownShaders();
}

