#include "shaders.h"
#include "../resource.h"

namespace engine
{
	extern HMODULE hModule;
	namespace gfx
	{
		namespace	
		{
			struct SHADER_RESOURCE
			{
				LPWSTR name;
				std::string target;
			};
			// Global varriables
			ID3DBlob* g_engineShaders[SHADER::Count]{};
			SHADER_RESOURCE g_shaderResources[] =
			{
				{ MAKEINTRESOURCEW(IDR_VS_FSTRIANGLE), "vs_5_0" },
			};

			static_assert(std::size(g_shaderResources) == SHADER::Count);
		}

		bool InitializeShaders()
		{
			uint32_t index = 0;
			for (SHADER_RESOURCE& resource : g_shaderResources)
			{
				HRSRC hlslResource = FindResource(hModule, resource.name, L"HLSL");
				assert(hlslResource);
				HGLOBAL hlslData = LoadResource(hModule, hlslResource);
				assert(hlslData);
				LPVOID hlslPtr = LockResource(hlslData);
				assert(hlslPtr);
				DWORD hlslSize = SizeofResource(hModule, hlslResource);

				const UINT compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_WARNINGS_ARE_ERRORS;

				ID3DBlob* pCompiledShaderBlob = nullptr;
				ID3DBlob* pErrorBlob = nullptr;
				HRESULT hr = D3DCompile(hlslPtr, hlslSize, nullptr, nullptr, nullptr, "main", resource.target.c_str(), compileFlags, 0, &pCompiledShaderBlob, &pErrorBlob);
				assert(SUCCEEDED(hr));
				assert(!pErrorBlob);

				g_engineShaders[index] = pCompiledShaderBlob;

				UnlockResource(hlslData);
				FreeResource(hlslData);
				index++;
			}

			return true;
		}

		void ShutdownShaders()
		{
			for (uint32_t i = 0; i < SHADER::Count; i++)
				g_engineShaders[i]->Release();
		}

		D3D12_SHADER_BYTECODE GetEngineShader(SHADER id)
		{
			assert(id < SHADER::Count);
			D3D12_SHADER_BYTECODE shaderBytecode;
			shaderBytecode.pShaderBytecode = g_engineShaders[id]->GetBufferPointer();
			shaderBytecode.BytecodeLength = g_engineShaders[id]->GetBufferSize();
			return shaderBytecode;
		}
	}
}