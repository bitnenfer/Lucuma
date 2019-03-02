#include "../../../resources/shader.h"
#include <Windows.h>
#include <windowsx.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <malloc.h>
#include "../../../../debug.h"
#include "../../../../file.h"


extern DXGI_FORMAT kResourceFormat[];

lu::VertexLayout::VertexLayout() :
	attributeCount(0),
	vertexStride(0)
{}

void lu::VertexLayout::addAttribute(lu::ResourceFormat format, uint32_t byteOffset)
{
	LU_ASSERT(attributeCount < kMaxAttributeCount);
	attributes[attributeCount].format = format;
	attributes[attributeCount].byteOffset = byteOffset;
	attributeCount += 1;
}

bool lu::resources::CreateVertexShaderFromSource(const RendererDevice& device, const void* pShaderSource, uint64_t sourceByteSize, const char* pEntryPoint, const VertexLayout& vertexLayout, VertexShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	if (shader.shader != LU_INVALID_HANDLE) DestroyVertexShader(device, shader);
	ID3D11VertexShader* pVertexShader = NULL;
	ID3DBlob* pShaderBlob = NULL;
	ID3DBlob* pErrorBlob = NULL;
	D3D_SHADER_MACRO macros[] = { NULL };
	HRESULT result = D3DCompile(
		pShaderSource,
		(SIZE_T)sourceByteSize,
		NULL,
		macros,
		NULL,
		pEntryPoint,
		"vs_5_0",
#if _DEBUG
		D3DCOMPILE_DEBUG |
#endif
		D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&pShaderBlob,
		&pErrorBlob
	);
	if (result != S_OK && pErrorBlob != NULL)
	{
		OutputDebugStringA("Vertex Shader Error!\n");
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pShaderBlob != NULL) pShaderBlob->Release();
		return false;
	}
	bool createResult = CreateVertexShaderFromByteCode(device, pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), vertexLayout, shader);
	pShaderBlob->Release();
	return createResult;
}
bool lu::resources::CreatePixelShaderFromSource(const RendererDevice& device, const void* pShaderSource, uint64_t sourceByteSize, const char* pEntryPoint, PixelShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	if (shader.shader != LU_INVALID_HANDLE) DestroyPixelShader(device, shader);
	ID3DBlob* pShaderBlob = NULL;
	ID3DBlob* pErrorBlob = NULL;
	D3D_SHADER_MACRO macros[] = { NULL };
	HRESULT result = D3DCompile(
		pShaderSource,
		(SIZE_T)sourceByteSize,
		NULL,
		macros,
		NULL,
		pEntryPoint,
		"ps_5_0",
#if _DEBUG
		D3DCOMPILE_DEBUG |
#endif
		D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&pShaderBlob,
		&pErrorBlob
	);
	if (result != S_OK && pErrorBlob != NULL)
	{
		OutputDebugStringA("Pixel Shader Error!\n");
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pShaderBlob != NULL) pShaderBlob->Release();
		return false;
	}
	bool createResult = CreatePixelShaderFromByteCode(device, pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), shader);
	pShaderBlob->Release();
	return createResult;
}
bool lu::resources::CreateComputeShaderFromSource(const RendererDevice& device, const void* pShaderSource, uint64_t sourceByteSize, const char* pEntryPoint, ComputeShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	if (shader.shader != LU_INVALID_HANDLE) DestroyComputeShader(device, shader);
	ID3DBlob* pShaderBlob = NULL;
	ID3DBlob* pErrorBlob = NULL;
	D3D_SHADER_MACRO macros[] = { NULL };
	HRESULT result = D3DCompile(
		pShaderSource,
		(SIZE_T)sourceByteSize,
		NULL,
		macros,
		NULL,
		pEntryPoint,
		"cs_5_0",
#if _DEBUG
		D3DCOMPILE_DEBUG |
#endif
		D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&pShaderBlob,
		&pErrorBlob
	);
	if (result != S_OK && pErrorBlob != NULL)
	{
		OutputDebugStringA("Compute Shader Error!\n");
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pShaderBlob != NULL) pShaderBlob->Release();
		LU_ASSERT(0);
		ExitProcess(1);
		return false;
	}
	bool createResult = CreateComputeShaderFromByteCode(device, pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), shader);
	pShaderBlob->Release();
	return createResult;
}

bool lu::resources::CreateGeometryShaderFromSource(const RendererDevice& device, const void* pShaderSource, uint64_t sourceByteSize, const char* pEntryPoint, GeometryShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	if (shader.shader != LU_INVALID_HANDLE) DestroyGeometryShader(device, shader);
	ID3DBlob* pShaderBlob = NULL;
	ID3DBlob* pErrorBlob = NULL;
	D3D_SHADER_MACRO macros[] = { NULL };
	HRESULT result = D3DCompile(
		pShaderSource,
		(SIZE_T)sourceByteSize,
		NULL,
		macros,
		NULL,
		pEntryPoint,
		"gs_5_0",
#if _DEBUG
		D3DCOMPILE_DEBUG |
#endif
		D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&pShaderBlob,
		&pErrorBlob
	);
	if (result != S_OK && pErrorBlob != NULL)
	{
		OutputDebugStringA("Geometry Shader Error!\n");
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pShaderBlob != NULL) pShaderBlob->Release();
		LU_ASSERT(0);
		ExitProcess(1);
		return false;
	}
	bool createResult = CreateGeometryShaderFromByteCode(device, pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), shader);
	pShaderBlob->Release();
	return createResult;
}

bool lu::resources::CreateVertexShaderFromByteCode(const RendererDevice& device, const void* pShaderByteCode, uint64_t byteCodeByteSize, const VertexLayout& vertexLayout, VertexShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11VertexShader* pVertexShader = NULL;
	HRESULT result = pDevice->CreateVertexShader(pShaderByteCode, (SIZE_T)byteCodeByteSize, NULL, &pVertexShader);
	if (result != S_OK) return false;
	shader.shader = (handle_t)pVertexShader;
	ID3D11InputLayout* pInputLayout = NULL;
	D3D11_INPUT_ELEMENT_DESC* pElementDesc = (D3D11_INPUT_ELEMENT_DESC*)alloca(vertexLayout.attributeCount* sizeof(D3D11_INPUT_ELEMENT_DESC));

	for (uint32_t index = 0; index < vertexLayout.attributeCount; ++index)
	{
		lu::VertexAttribute attrib = vertexLayout.attributes[index];
		pElementDesc[index].SemanticName = "TEXCOORD";
		pElementDesc[index].Format = kResourceFormat[(uint32_t)attrib.format];
		pElementDesc[index].SemanticIndex = index;
		pElementDesc[index].InputSlot = 0;
		pElementDesc[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pElementDesc[index].InstanceDataStepRate = 0;
		pElementDesc[index].AlignedByteOffset = attrib.byteOffset;
	}
	result = pDevice->CreateInputLayout(pElementDesc, vertexLayout.attributeCount, pShaderByteCode, (SIZE_T)byteCodeByteSize, &pInputLayout);
	if (result != S_OK) return false;
	shader.layout = (handle_t)pInputLayout;
	return true;
}
bool lu::resources::CreatePixelShaderFromByteCode(const RendererDevice& device, const void* pShaderByteCode, uint64_t byteCodeByteSize, PixelShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11PixelShader* pPixelShader = NULL;
	HRESULT result = pDevice->CreatePixelShader(pShaderByteCode, (SIZE_T)byteCodeByteSize, NULL, &pPixelShader);
	if (result != S_OK) return false;
	shader.shader = (handle_t)pPixelShader;
	return true;
}
bool lu::resources::CreateComputeShaderFromByteCode(const RendererDevice& device, const void* pShaderByteCode, uint64_t byteCodeByteSize, ComputeShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11ComputeShader* pComputeShader = NULL;
	HRESULT result = pDevice->CreateComputeShader(pShaderByteCode, (SIZE_T)byteCodeByteSize, NULL, &pComputeShader);
	if (result != S_OK) return false;
	shader.shader = (handle_t)pComputeShader;
	return true;
}

bool lu::resources::CreateGeometryShaderFromByteCode(const RendererDevice& device, const void* pShaderByteCode, uint64_t byteCodeByteSize, GeometryShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11GeometryShader* pGeometryShader = NULL;
	HRESULT result = pDevice->CreateGeometryShader(pShaderByteCode, (SIZE_T)byteCodeByteSize, NULL, &pGeometryShader);
	if (result != S_OK) return false;
	shader.shader = (handle_t)pGeometryShader;
	return true;
}

bool lu::resources::CreateVertexShaderFromSourceFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, const char* pEntryPoint, const VertexLayout& vertexLayout, VertexShader& shader)
{
	byte_t* pShaderSource = NULL;
	uint64_t readSize = 0;
	if (!file::ReadFileData(allocator, pFileName, &pShaderSource, &readSize))
	{
		return false;
	}
	if (!CreateVertexShaderFromSource(device, pShaderSource, readSize, pEntryPoint, vertexLayout, shader))
	{
		file::FreeFileData(allocator, pShaderSource);
		return false;
	}
	file::FreeFileData(allocator, pShaderSource);
	return true;
}

bool lu::resources::CreatePixelShaderFromSourceFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, const char* pEntryPoint, PixelShader& shader)
{
	byte_t* pShaderSource = NULL;
	uint64_t readSize = 0;
	if (!file::ReadFileData(allocator, pFileName, &pShaderSource, &readSize))
	{
		return false;
	}
	if (!CreatePixelShaderFromSource(device, pShaderSource, readSize, pEntryPoint, shader))
	{
		file::FreeFileData(allocator, pShaderSource);
		return false;
	}
	file::FreeFileData(allocator, pShaderSource);
	return true;
}

bool lu::resources::CreateComputeShaderFromSourceFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, const char* pEntryPoint, ComputeShader& shader)
{
	byte_t* pShaderSource = NULL;
	uint64_t readSize = 0;
	if (!file::ReadFileData(allocator, pFileName, &pShaderSource, &readSize))
	{
		return false;
	}
	if (!CreateComputeShaderFromSource(device, pShaderSource, readSize, pEntryPoint, shader))
	{
		file::FreeFileData(allocator, pShaderSource);
		return false;
	}
	file::FreeFileData(allocator, pShaderSource);
	return true;
}

bool lu::resources::CreateGeometryShaderFromSourceFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, const char* pEntryPoint, GeometryShader& shader)
{
	byte_t* pShaderSource = NULL;
	uint64_t readSize = 0;
	if (!file::ReadFileData(allocator, pFileName, &pShaderSource, &readSize))
	{
		return false;
	}
	if (!CreateGeometryShaderFromSource(device, pShaderSource, readSize, pEntryPoint, shader))
	{
		file::FreeFileData(allocator, pShaderSource);
		return false;
	}
	file::FreeFileData(allocator, pShaderSource);
	return true;
}

bool lu::resources::CreateVertexShaderFromByteCodeFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, const VertexLayout& vertexLayout, VertexShader& shader)
{
	byte_t* pShaderByteCode = NULL;
	uint64_t readSize = 0;
	if (!file::ReadFileData(allocator, pFileName, &pShaderByteCode, &readSize))
	{
		return false;
	}
	if (!CreateVertexShaderFromByteCode(device, pShaderByteCode, readSize, vertexLayout, shader))
	{
		file::FreeFileData(allocator, pShaderByteCode);
		return false;
	}
	file::FreeFileData(allocator, pShaderByteCode);
	return true;
}

bool lu::resources::CreatePixelShaderFromByteCodeFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, PixelShader& shader)
{
	byte_t* pShaderByteCode = NULL;
	uint64_t readSize = 0;
	if (!file::ReadFileData(allocator, pFileName, &pShaderByteCode, &readSize))
	{
		return false;
	}
	if (!CreatePixelShaderFromByteCode(device, pShaderByteCode, readSize, shader))
	{
		file::FreeFileData(allocator, pShaderByteCode);
		return false;
	}
	file::FreeFileData(allocator, pShaderByteCode);
	return true;
}

bool lu::resources::CreateComputeShaderFromByteCodeFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, ComputeShader& shader)
{
	byte_t* pShaderByteCode = NULL;
	uint64_t readSize = 0;
	if (!file::ReadFileData(allocator, pFileName, &pShaderByteCode, &readSize))
	{
		return false;
	}
	if (!CreateComputeShaderFromByteCode(device, pShaderByteCode, readSize, shader))
	{
		file::FreeFileData(allocator, pShaderByteCode);
		return false;
	}
	file::FreeFileData(allocator, pShaderByteCode);
	return true;
}

bool lu::resources::CreateGeometryShaderFromByteCodeFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, GeometryShader& shader)
{
	byte_t* pShaderByteCode = NULL;
	uint64_t readSize = 0;
	if (!file::ReadFileData(allocator, pFileName, &pShaderByteCode, &readSize))
	{
		return false;
	}
	if (!CreateGeometryShaderFromByteCode(device, pShaderByteCode, readSize, shader))
	{
		file::FreeFileData(allocator, pShaderByteCode);
		return false;
	}
	file::FreeFileData(allocator, pShaderByteCode);
	return true;
}

void lu::resources::DestroyVertexShader(const RendererDevice& device, VertexShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11VertexShader* pVertexShader = (ID3D11VertexShader*)shader.shader;
	ID3D11InputLayout* pInputLayout = (ID3D11InputLayout*)shader.layout;

	shader.shader = LU_INVALID_HANDLE;
	shader.layout = LU_INVALID_HANDLE;

	if (pVertexShader != NULL)
	{
		pVertexShader->Release();
	}
	if (pInputLayout != NULL)
	{
		pInputLayout->Release();
	}
}
void lu::resources::DestroyPixelShader(const RendererDevice& device, PixelShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11PixelShader* pPixelShader = (ID3D11PixelShader*)shader.shader;
	shader.shader = LU_INVALID_HANDLE;
	if (pPixelShader != NULL)
	{
		pPixelShader->Release();
	}
}
void lu::resources::DestroyComputeShader(const RendererDevice& device, ComputeShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11ComputeShader* pComputeShader = (ID3D11ComputeShader*)shader.shader;
	shader.shader = LU_INVALID_HANDLE;
	if (pComputeShader != NULL)
	{
		pComputeShader->Release();
	}
}

void lu::resources::DestroyGeometryShader(const RendererDevice& device, GeometryShader& shader)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11GeometryShader* pGeometryShader = (ID3D11GeometryShader*)shader.shader;
	shader.shader = LU_INVALID_HANDLE;
	if (pGeometryShader != NULL)
	{
		pGeometryShader->Release();
	}
}
