#pragma once

#include "../../types.h"
#include "../renderer.h"
#include "format.h"
#include "../../utils/allocator.h"

namespace lu
{
	struct VertexAttribute
	{
		ResourceFormat format;
		uint32_t byteOffset;
	};

	struct VertexLayout
	{
		VertexLayout();

		void addAttribute(ResourceFormat format, uint32_t byteOffset);

		static const uint32_t kMaxAttributeCount = 16;
		VertexAttribute attributes[kMaxAttributeCount];
		uint32_t attributeCount;
		uint32_t vertexStride;
	};

	struct VertexShader
	{
		VertexShader() : layout(LU_INVALID_HANDLE), shader(LU_INVALID_HANDLE) {}	
		bool isValid() { return layout != LU_INVALID_HANDLE && shader != LU_INVALID_HANDLE; }
		handle_t layout;
		handle_t shader;
	};

	struct PixelShader
	{
		PixelShader() : shader(LU_INVALID_HANDLE) {}
		bool isValid() { return shader != LU_INVALID_HANDLE; }
		handle_t shader;
	};

	struct ComputeShader
	{
		ComputeShader() : shader(LU_INVALID_HANDLE) {}
		bool isValid() { return shader != LU_INVALID_HANDLE; }
		handle_t shader;
	};

	struct GeometryShader
	{
		GeometryShader() : shader(LU_INVALID_HANDLE) {}
		bool isValid() { return shader != LU_INVALID_HANDLE; }
		handle_t shader;
	};

	namespace resources
	{
		bool CreateVertexShaderFromSource(const RendererDevice& device, const void* pShaderSource, uint64_t sourceByteSize, const char* pEntryPoint, const VertexLayout& vertexLayout, VertexShader& shader);
		bool CreatePixelShaderFromSource(const RendererDevice& device, const void* pShaderSource, uint64_t sourceByteSize, const char* pEntryPoint, PixelShader& shader);
		bool CreateComputeShaderFromSource(const RendererDevice& device, const void* pShaderSource, uint64_t sourceByteSize, const char* pEntryPoint, ComputeShader& shader);
		bool CreateGeometryShaderFromSource(const RendererDevice& device, const void* pShaderSource, uint64_t sourceByteSize, const char* pEntryPoint, GeometryShader& shader);

		bool CreateVertexShaderFromByteCode(const RendererDevice& device, const void* pShaderByteCode, uint64_t byteCodeByteSize, const VertexLayout& vertexLayout, VertexShader& shader);
		bool CreatePixelShaderFromByteCode(const RendererDevice& device, const void* pShaderByteCode, uint64_t byteCodeByteSize, PixelShader& shader);
		bool CreateComputeShaderFromByteCode(const RendererDevice& device, const void* pShaderByteCode, uint64_t byteCodeByteSize, ComputeShader& shader);
		bool CreateGeometryShaderFromByteCode(const RendererDevice& device, const void* pShaderByteCode, uint64_t byteCodeByteSize, GeometryShader& shader);

		bool CreateVertexShaderFromSourceFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, const char* pEntryPoint, const VertexLayout& vertexLayout, VertexShader& shader);
		bool CreatePixelShaderFromSourceFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, const char* pEntryPoint, PixelShader& shader);
		bool CreateComputeShaderFromSourceFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, const char* pEntryPoint, ComputeShader& shader);
		bool CreateGeometryShaderFromSourceFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, const char* pEntryPoint, GeometryShader& shader);

		bool CreateVertexShaderFromByteCodeFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, const VertexLayout& vertexLayout, VertexShader& shader);
		bool CreatePixelShaderFromByteCodeFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, PixelShader& shader);
		bool CreateComputeShaderFromByteCodeFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, ComputeShader& shader);
		bool CreateGeometryShaderFromByteCodeFile(IAllocator& allocator, const RendererDevice& device, const char* pFileName, GeometryShader& shader);

		void DestroyVertexShader(const RendererDevice& device, VertexShader& shader);
		void DestroyPixelShader(const RendererDevice& device, PixelShader& shader);
		void DestroyComputeShader(const RendererDevice& device, ComputeShader& shader);
		void DestroyGeometryShader(const RendererDevice& device, GeometryShader& shader);
	}
}