#pragma once

#include "buffer.h"
#include "depth_stencil_target.h"
#include "format.h"
#include "render_target.h"
#include "shader.h"
#include "texture.h"
#include "unordered_access_view.h"

#include "../states/blend_state.h"
#include "../states/depth_stencil_state.h"
#include "../states/rasterizer_state.h"
#include "../states/sampler_state.h"

#include "../../utils/allocator.h"
#include "../../utils/linear_allocator.h"

namespace lu
{
	enum class CmdId : uint32_t
	{
		ClearDepthStencilTarget,
		ClearRenderTarget,
		SetComputeShaderConstantBuffer,
		SetComputeShaderSampler,
		SetComputeShader,
		SetComputeShaderTextures,
		SetComputeShaderUAV,
		Dispatch,
		DispatchIndirect,
		Draw,
		DrawIndexed,
		DrawIndexedInstanced,
		DrawIndexedInstancedIndirect,
		DrawInstanced,
		DrawInstancedIndirect,
		SetPrimitiveTopology,
		SetIndexBuffer,
		SetVertexBuffers,
		SetBlendState,
		SetDepthStencilState,
		SetRenderTargets,
		SetPixelShaderConstantBuffers,
		SetPixelShaderSamplers,
		SetPixelShader,
		SetPixelShaderTextures,
		SetRasterizerState,
		SetViewports,
		SetScissors,
		SetVertexShaderConstantBuffer,
		SetVertexShaderSamplers,
		SetVertexShader,
		SetVertexShaderTextures,
		SetGeometryShaderConstantBuffer,
		SetGeometryShaderSamplers,
		SetGeometryShader,
		SetGeometryShaderTextures
	};

	struct CommandList 
	{
		void reset() 
		{ 
			commandAllocator.reset(); 
			pConsumeAddress = commandAllocator.pBufferHead; 
		}
		void resetConsumerAddress()
		{
			pConsumeAddress = commandAllocator.pBufferHead;
		}
		template<class Cmd>
		void addCommand(CmdId id, const Cmd& cmd)
		{
			CmdId* pId = (CmdId*)commandAllocator.allocate(sizeof(CmdId));
			Cmd* pCmd = (Cmd*)commandAllocator.allocate(sizeof(Cmd));

			*pId = id;
			*pCmd = cmd;
		}
		CmdId consumeId()
		{
			CmdId* pId = (CmdId*)pConsumeAddress;
			pConsumeAddress += sizeof(CmdId);
			return *pId;
		}
		template<class Cmd>
		const Cmd* consumeCommand()
		{
			const Cmd* pCmd = (Cmd*)pConsumeAddress;
			pConsumeAddress += sizeof(Cmd);
			return pCmd;
		}
		bool canConsume() const
		{
			return pConsumeAddress < commandAllocator.pBufferTail;
		}
		LinearAllocator commandAllocator;
		byte_t* pConsumeAddress;
	};

	enum DepthStencilClearFlag
	{
		CLEAR_STENCIL = 1,
		CLEAR_DEPTH = 2
	};

	enum PrimitiveTopology
	{
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
		POINT_LIST,
		LINE_LIST,
		LINE_STRIP
	};

	namespace commands
	{
		uint32_t GetCommandSize();

		bool CreateCommandList(IAllocator& allocator, uint32_t maxCommandCount, CommandList& commandList);
		void DestroyCommandList(IAllocator& allocator, CommandList& commandList);

		void ClearDepthStencilTarget(CommandList& cmds, DepthStencilTarget& target, uint32_t clearFlag, float32_t clearDepth, uint8_t clearStencil);
		void ClearRenderTarget(CommandList& cmds, RenderTarget& target, float32_t clearColor[4]);
		void SetComputeShaderConstantBuffer(CommandList& cmds, uint32_t slot, uint32_t bufferCount, Buffer* pBuffers);
		void SetComputeShaderSampler(CommandList& cmds, uint32_t slot, uint32_t samplerCount, SamplerState* pSamplers);
		void SetComputeShader(CommandList& cmds, ComputeShader* pShader);
		void SetComputeShaderTextures(CommandList& cmds, uint32_t slot, uint32_t textureCount, Texture* pTextures);
		void SetComputeShaderUAV(CommandList& cmds, uint32_t slot, uint32_t uavCount, UnorderedAccessView* pUAVs);
		void Dispatch(CommandList& cmds, uint32_t x, uint32_t y, uint32_t z);
		void DispatchIndirect(CommandList& cmds, Buffer& bufferForArgs, uint32_t byteAlignmentForArgs);
		void Draw(CommandList& cmds, uint32_t vertexCount, uint32_t startVertexLocation);
		void DrawIndexed(CommandList& cmds, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation);
		void DrawIndexedInstanced(CommandList& cmds, uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndexLocation, int32_t baseVertexLocation, uint32_t startInstanceLocation);
		void DrawIndexedInstancedIndirect(CommandList& cmds, Buffer* pBufferForArgs, uint32_t byteAlignmentForArgs);
		void DrawInstanced(CommandList& cmds, uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation);
		void DrawInstancedIndirect(CommandList& cmds, Buffer& bufferForArgs, uint32_t byteAlignmentForArgs);
		void SetPrimitiveTopology(CommandList& cmds, PrimitiveTopology topology);
		void SetIndexBuffer(CommandList& cmds, ResourceFormat format, Buffer& indexBuffer);
		void SetVertexBuffers(CommandList& cmds, uint32_t slot, uint32_t bufferCount, Buffer* pVertexBuffers, uint32_t* pStrides, uint32_t* pOffsets);
		void SetBlendState(CommandList& cmds, BlendState& blendState, const float32_t blendFactor[4], uint32_t sampleMask);
		void SetDepthStencilState(CommandList& cmds, DepthStencilState& depthStencilState, uint32_t stencilRef);
		void SetRenderTargets(CommandList& cmds, uint32_t rtCount, RenderTarget* pRenderTargets, DepthStencilTarget* pDepthStencilTarget);
		void SetPixelShaderConstantBuffers(CommandList& cmds, uint32_t slot, uint32_t bufferCount, Buffer* pBuffers);
		void SetPixelShaderSamplers(CommandList& cmds, uint32_t slot, uint32_t samplerCount, SamplerState* pSamplers);
		void SetPixelShader(CommandList& cmds, PixelShader* pShader);
		void SetPixelShaderTextures(CommandList& cmds, uint32_t slot, uint32_t textureCount, Texture* pTextures);
		void SetRasterizerState(CommandList& cmds, RasterizerState& rasterizerState);
		void SetViewports(CommandList& cmds, uint32_t viewportCount, Viewport* pViewports);
		void SetScissors(CommandList& cmds, uint32_t scissorCount, Scissor* pScissors);
		void SetVertexShaderConstantBuffers(CommandList& cmds, uint32_t slot, uint32_t bufferCount, Buffer* pBuffers);
		void SetVertexShaderSamplers(CommandList& cmds, uint32_t slot, uint32_t samplerCount, SamplerState* pSamplers);
		void SetVertexShader(CommandList& cmds, VertexShader* pShader);
		void SetVertexShaderTextures(CommandList& cmds, uint32_t slot, uint32_t textureCount, Texture* pTextures);
		void SetGeometryShaderConstantBuffers(CommandList& cmds, uint32_t slot, uint32_t bufferCount, Buffer* pBuffers);
		void SetGeometryShaderSamplers(CommandList& cmds, uint32_t slot, uint32_t samplerCount, SamplerState* pSamplers);
		void SetGeometryShader(CommandList& cmds, GeometryShader* pShader);
		void SetGeometryShaderTextures(CommandList& cmds, uint32_t slot, uint32_t textureCount, Texture* pTextures);

		void ExecuteCommandList(RendererContext& context, CommandList& cmds);
	}
}
