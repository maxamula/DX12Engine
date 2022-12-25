#include "shaders.h"

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
			ID3DBlob* g_engineShaders[SHADER::numShaders]{};
			SHADER_RESOURCE g_shaderResources[] =
			{
				{ MAKEINTRESOURCEW(IDR_VS_DEFAULT), "vs_5_0" },
			};

			static_assert(std::size(g_shaderResources) == SHADER::numShaders);

			class ShaderCompiler
			{
			public:
				ShaderCompiler()
				{
					assert(SUCCEEDED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_compiler))));
					assert(SUCCEEDED(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_utils))));
					assert(SUCCEEDED(m_utils->CreateDefaultIncludeHandler(&m_includeHandler)));

				}

				ID3DBlob* Compile(SHADER_INFO& info, std::filesystem::path filepath)
				{
					ComPtr<IDxcBlobEncoding> sourceBlob = NULL;
					assert(SUCCEEDED(m_utils->LoadFile(filepath.c_str(), NULL, &sourceBlob)));
					assert(sourceBlob->GetBufferSize());
					LPCWSTR args[]
					{
						filepath.c_str(),
						L"-E", info.entryPoint.c_str(),
						L"-T", m_target[info.type],
						DXC_ARG_ALL_RESOURCES_BOUND,
						DXC_ARG_OPTIMIZATION_LEVEL3,
#ifdef _DEBUG
						DXC_ARG_DEBUG,
#endif
						DXC_ARG_WARNINGS_ARE_ERRORS,
						L"-Qstrip_reflect",
						L"-Qstrip_debug"
					};
				}

			private:
				const wchar_t* m_target[SHADER_TYPE::numTypes]{ L"vs_6_5", L"hs_6_5", L"ds_6_5", L"gs_6_5", L"ps_6_5", L"cs_6_5" };
				ComPtr<IDxcCompiler3> m_compiler = NULL;
				ComPtr<IDxcUtils> m_utils = NULL;
				ComPtr<IDxcIncludeHandler> m_includeHandler = NULL;
			};
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
			for (uint32_t i = 0; i < SHADER::numShaders; i++)
				g_engineShaders[i]->Release();
		}

		D3D12_SHADER_BYTECODE GetEngineShader(SHADER id)
		{
			assert(id < SHADER::numShaders);
			D3D12_SHADER_BYTECODE shaderBytecode;
			shaderBytecode.pShaderBytecode = g_engineShaders[id]->GetBufferPointer();
			shaderBytecode.BytecodeLength = g_engineShaders[id]->GetBufferSize();
			return shaderBytecode;
		}
	}
}