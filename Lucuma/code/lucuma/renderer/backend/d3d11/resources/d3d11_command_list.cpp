#include "../../../resources/command_list.h"
#include "../../../../utils/macros.h"
#include <d3d11.h>
#include "../../../../debug.h"

#define LU_MAX_RENDER_TARGETS 4
#define LU_MAX_BUFFERS 4
#define LU_MAX_SAMPLERS 4
#define LU_MAX_TEXTURES 4
#define LU_MAX_UAVS 4
#define LU_MAX_VERTEX_BUFFERS 4

extern DXGI_FORMAT kResourceFormat[];

static D3D11_PRIMITIVE_TOPOLOGY kTopology[] =
{
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,	//TRIANGLE_LIST
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,	//TRIANGLE_STRIP
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,		//POINT_LIST
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,		//LINE_LIST
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP		//LINE_STRIP
};

struct CmdClearDepthStencilTarget 
{
	ID3D11DepthStencilView* pView;
	uint32_t clearFlag;
	float32_t clearDepth;
	uint8_t clearStencil;
};
struct CmdClearRenderTarget 
{
	ID3D11RenderTargetView* pView;
	float32_t clearColor[4];
};
struct CmdSetConstantBuffer 
{
	ID3D11Buffer* buffers[LU_MAX_BUFFERS];
	uint32_t count;
};
struct CmdSetSampler 
{
	ID3D11SamplerState* samplers[LU_MAX_SAMPLERS];
	uint32_t count;
};
struct CmdSetComputeShader 
{
	ID3D11ComputeShader* pShader;
};
struct CmdSetTextures 
{
	ID3D11ShaderResourceView* shaderResourceView[LU_MAX_TEXTURES];
	uint32_t count;
};
struct CmdSetComputeShaderUAV 
{
	ID3D11UnorderedAccessView* uavs[LU_MAX_UAVS];
	uint32_t count;
};
struct CmdDispatch 
{
	uint32_t x, y, z;
};
struct CmdIndirect
{
	ID3D11Buffer* pBufferForArgs;
	uint32_t byteAlignmentForArgs;
};
struct CmdDraw 
{
	uint32_t vertexCount, startVertexLocation;
};
struct CmdDrawIndexed 
{
	uint32_t indexCount, startIndexLocation, baseVertexLocation;
};
struct CmdDrawIndexedInstanced
{
	uint32_t indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation;
};
struct CmdDrawInstanced 
{
	uint32_t vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation;
};
struct CmdSetPrimitiveTopology 
{
	D3D11_PRIMITIVE_TOPOLOGY topology;
};
struct CmdSetIndexBuffer 
{
	ID3D11Buffer* pIndexBuffer;
	DXGI_FORMAT format;
};
struct CmdSetVertexBuffers 
{
	ID3D11Buffer* vertexBuffers[LU_MAX_VERTEX_BUFFERS];
	uint32_t strides[LU_MAX_VERTEX_BUFFERS];
	uint32_t offsets[LU_MAX_VERTEX_BUFFERS];
	uint32_t count;
};
struct CmdSetBlendState 
{
	ID3D11BlendState* pBlendState;
	float32_t blendFactor[4];
	uint32_t sampleMask;
};
struct CmdSetDepthStencilState 
{
	ID3D11DepthStencilState* pDepthStencilState;
	uint32_t stencilRef;
};
struct CmdSetRenderTargets 
{
	ID3D11RenderTargetView* renderTargets[LU_MAX_RENDER_TARGETS];
	ID3D11DepthStencilView* pDepthStencilView;
	uint32_t count;
};
struct CmdSetPixelShader 
{
	ID3D11PixelShader* pShader;
};
struct CmdSetRasterizerState 
{
	ID3D11RasterizerState* pRasterizerState;
};
struct CmdSetViewports 
{
	D3D11_VIEWPORT viewports[LU_MAX_RENDER_TARGETS];
	uint32_t count;
};
struct CmdSetScissors 
{
	D3D11_RECT scissors[LU_MAX_RENDER_TARGETS];
	uint32_t count;
};
struct CmdSetVertexShader 
{
	ID3D11VertexShader* pShader;
	ID3D11InputLayout* pInputLayout;
};
struct CmdSetGeometryShader
{
	ID3D11GeometryShader* pShader;
};

union CmdAll
{
	CmdSetConstantBuffer setConstantBuffer;
	CmdSetSampler setSampler;
	CmdSetComputeShader setComputeShader;
	CmdSetTextures setTextures;
	CmdSetComputeShaderUAV setComputeShaderUAV;
	CmdDispatch dispatch;
	CmdIndirect dispatchIndirect;
	CmdDraw draw;
	CmdDrawIndexed drawIndexed;
	CmdDrawIndexedInstanced drawIndexedInstanced;
	CmdDrawInstanced drawInstanced;
	CmdSetPrimitiveTopology setPrimitiveTopology;
	CmdSetIndexBuffer setIndexBuffer;
	CmdSetVertexBuffers setVertexBuffers;
	CmdSetBlendState setBlendState;
	CmdSetDepthStencilState setDepthStencilState;
	CmdSetRenderTargets setRenderTargets;
	CmdSetPixelShader setPixelShader;
	CmdSetRasterizerState setRasterizerState;
	CmdSetViewports setViewports;
	CmdSetScissors setScissors;
	CmdSetVertexShader setVertexShader;
};

uint32_t lu::commands::GetCommandSize()
{
	return sizeof(CmdAll);
}
bool lu::commands::CreateCommandList(IAllocator& allocator, uint32_t maxCommandCount, CommandList& commandList)
{
	uint64_t size = maxCommandCount * GetCommandSize();
	commandList.commandAllocator.initialize((byte_t*)allocator.allocate(size), size);
	if (commandList.commandAllocator.pBufferHead != NULL)
	{
		return true;
	}
	return false;
}
void lu::commands::DestroyCommandList(IAllocator& allocator, CommandList& commandList)
{
	allocator.deallocate(commandList.commandAllocator.pBufferHead);
	commandList.commandAllocator.finalize();
	commandList.pConsumeAddress = NULL;
}
void lu::commands::ClearDepthStencilTarget(CommandList& cmds, DepthStencilTarget& target, uint32_t clearFlag, float32_t clearDepth, uint8_t clearStencil)
{
	CmdClearDepthStencilTarget cmd;
	cmd.pView = (ID3D11DepthStencilView*)target.depthStencilTarget;
	cmd.clearFlag = clearFlag;
	cmd.clearDepth = clearDepth;
	cmd.clearStencil = clearStencil;;
	cmds.addCommand(CmdId::ClearDepthStencilTarget, cmd);
}
void lu::commands::ClearRenderTarget(CommandList& cmds, RenderTarget& target, float32_t clearColor[4])
{
	CmdClearRenderTarget cmd;
	cmd.pView = (ID3D11RenderTargetView*)target.renderTarget;
	cmd.clearColor[0] = clearColor[0];
	cmd.clearColor[1] = clearColor[1];
	cmd.clearColor[2] = clearColor[2];
	cmd.clearColor[3] = clearColor[3];
	cmds.addCommand(CmdId::ClearRenderTarget, cmd);
}
void lu::commands::SetComputeShaderConstantBuffer(CommandList& cmds, uint32_t bufferCount, Buffer* pBuffers)
{
	CmdSetConstantBuffer cmd;
	if (bufferCount == 1)
	{
		cmd.count = 1;
		cmd.buffers[0] = (ID3D11Buffer*)pBuffers[0].buffer;
	}
	else 
	{
		LU_ASSERT_MSG(bufferCount < LU_MAX_BUFFERS, "Buffer count exceeds max amount of buffers");
		for (uint32_t i = 0; i < bufferCount; ++i)
		{
			cmd.buffers[i] = (ID3D11Buffer*)pBuffers[i].buffer;
		}
		cmd.count = bufferCount;
	}
	cmds.addCommand(CmdId::SetComputeShaderConstantBuffer, cmd);
}
void lu::commands::SetComputeShaderSampler(CommandList& cmds, uint32_t samplerCount, SamplerState* pSamplers)
{
	CmdSetSampler cmd;
	if (samplerCount == 1)
	{
		cmd.count = 1;
		cmd.samplers[0] = (ID3D11SamplerState*)pSamplers[0].sampler;
	}
	else
	{
		LU_ASSERT_MSG(samplerCount < LU_MAX_SAMPLERS, "Sampler count exceeds the max amount of samplers");
		for (uint32_t i = 0; i < samplerCount; ++i)
		{
			cmd.samplers[i] = (ID3D11SamplerState*)pSamplers[i].sampler;
		}
		cmd.count = samplerCount;
	}
	cmds.addCommand(CmdId::SetComputeShaderSampler, cmd);
}
void lu::commands::SetComputeShader(CommandList& cmds, ComputeShader* pShader)
{
	CmdSetComputeShader cmd;
	if (pShader != NULL)
		cmd.pShader = (ID3D11ComputeShader*)pShader->shader;
	else
		cmd.pShader = NULL;
	cmds.addCommand(CmdId::SetComputeShader, cmd);
}
void lu::commands::SetComputeShaderTextures(CommandList& cmds, uint32_t textureCount, Texture* pTextures)
{
	CmdSetTextures cmd;
	if (textureCount == 1 || pTextures == NULL)
	{
		cmd.count = 1;
		if (pTextures != NULL)
			cmd.shaderResourceView[0] = (ID3D11ShaderResourceView*)pTextures[0].resource;
		else
			cmd.shaderResourceView[0] = NULL;
	}
	else
	{
		LU_ASSERT_MSG(textureCount < LU_MAX_TEXTURES, "Texture count exceeds the max amount of textures");
		for (uint32_t i = 0; i < textureCount; ++i)
		{
			cmd.shaderResourceView[i] = (ID3D11ShaderResourceView*)pTextures[i].resource;
		}
		cmd.count = textureCount;
	}
	cmds.addCommand(CmdId::SetComputeShaderTextures, cmd);
}
void lu::commands::SetComputeShaderUAV(CommandList& cmds, uint32_t uavCount, UnorderedAccessView* pUAVs)
{
	CmdSetComputeShaderUAV cmd;
	if (uavCount == 1)
	{
		cmd.count = 1;
		cmd.uavs[0] = (ID3D11UnorderedAccessView*)pUAVs[0].uav;
	}
	else
	{
		LU_ASSERT_MSG(uavCount < LU_MAX_UAVS, "UAV count exceeds the max amount of UAVs");
		for (uint32_t i = 0; i < uavCount; ++i)
		{
			cmd.uavs[i] = (ID3D11UnorderedAccessView*)pUAVs[i].uav;
		}
		cmd.count = uavCount;
	}
	cmds.addCommand(CmdId::SetComputeShaderUAV, cmd);
}
void lu::commands::Dispatch(CommandList& cmds, uint32_t x, uint32_t y, uint32_t z)
{
	CmdDispatch cmd;
	cmd.x = x;
	cmd.y = y;
	cmd.z = z;
	cmds.addCommand(CmdId::Dispatch, cmd);
}
void lu::commands::DispatchIndirect(CommandList& cmds, Buffer& bufferForArgs, uint32_t byteAlignmentForArgs)
{
	CmdIndirect cmd;
	cmd.pBufferForArgs = (ID3D11Buffer*)bufferForArgs.buffer;
	cmd.byteAlignmentForArgs = byteAlignmentForArgs;
	cmds.addCommand(CmdId::DispatchIndirect, cmd);
}
void lu::commands::Draw(CommandList& cmds, uint32_t vertexCount, uint32_t startVertexLocation)
{
	CmdDraw cmd;
	cmd.vertexCount = vertexCount;
	cmd.startVertexLocation = startVertexLocation;
	cmds.addCommand(CmdId::Draw, cmd);
}
void lu::commands::DrawIndexed(CommandList& cmds, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation)
{
	CmdDrawIndexed cmd;
	cmd.indexCount = indexCount;
	cmd.startIndexLocation = startIndexLocation;
	cmd.baseVertexLocation = baseVertexLocation;
	cmds.addCommand(CmdId::DrawIndexed, cmd);
}
void lu::commands::DrawIndexedInstanced(CommandList& cmds, uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndexLocation, int32_t baseVertexLocation, uint32_t startInstanceLocation)
{
	CmdDrawIndexedInstanced cmd;
	cmd.indexCountPerInstance = indexCountPerInstance;
	cmd.instanceCount = instanceCount;
	cmd.startIndexLocation = startIndexLocation;
	cmd.baseVertexLocation = baseVertexLocation;
	cmd.startInstanceLocation = startInstanceLocation;
	cmds.addCommand(CmdId::DrawIndexedInstanced, cmd);
}
void lu::commands::DrawIndexedInstancedIndirect(CommandList& cmds, Buffer* pBufferForArgs, uint32_t byteAlignmentForArgs)
{
	CmdIndirect cmd;
	cmd.pBufferForArgs = (ID3D11Buffer*)pBufferForArgs->buffer;
	cmd.byteAlignmentForArgs = byteAlignmentForArgs;
	cmds.addCommand(CmdId::DrawIndexedInstancedIndirect, cmd);
}
void lu::commands::DrawInstanced(CommandList& cmds, uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation)
{
	CmdDrawInstanced cmd;
	cmd.vertexCountPerInstance = vertexCountPerInstance;
	cmd.instanceCount = instanceCount;
	cmd.startVertexLocation = startVertexLocation;
	cmd.startInstanceLocation = startInstanceLocation;
	cmds.addCommand(CmdId::DrawInstanced, cmd);
}
void lu::commands::DrawInstancedIndirect(CommandList& cmds, Buffer& bufferForArgs, uint32_t byteAlignmentForArgs)
{
	CmdIndirect cmd;
	cmd.pBufferForArgs = (ID3D11Buffer*)bufferForArgs.buffer;
	cmd.byteAlignmentForArgs = byteAlignmentForArgs;
	cmds.addCommand(CmdId::DrawInstancedIndirect, cmd);
}
void lu::commands::SetPrimitiveTopology(CommandList& cmds, PrimitiveTopology topology)
{
	CmdSetPrimitiveTopology cmd;
	cmd.topology = kTopology[(uint32_t)topology];
	cmds.addCommand(CmdId::SetPrimitiveTopology, cmd);
}
void lu::commands::SetIndexBuffer(CommandList& cmds, ResourceFormat format, Buffer& indexBuffer)
{
	CmdSetIndexBuffer cmd;
	cmd.format = kResourceFormat[(uint32_t)format];
	cmd.pIndexBuffer = (ID3D11Buffer*)indexBuffer.buffer;
	cmds.addCommand(CmdId::SetIndexBuffer, cmd);
}
void lu::commands::SetVertexBuffers(CommandList& cmds, uint32_t bufferCount, Buffer* pVertexBuffers, uint32_t* pStrides, uint32_t* pOffsets)
{
	CmdSetVertexBuffers cmd;
	if (bufferCount == 1)
	{
		cmd.count = 1;
		cmd.offsets[0] = pOffsets[0];
		cmd.strides[0] = pStrides[0];
		cmd.vertexBuffers[0] = (ID3D11Buffer*)pVertexBuffers[0].buffer;
	}
	else
	{
		LU_ASSERT_MSG(bufferCount < LU_MAX_VERTEX_BUFFERS, "Buffer count exceeds the max amount of vertex buffers");
		for (uint32_t i = 0; i < bufferCount; ++i)
		{
			cmd.offsets[i] = pOffsets[i];
			cmd.strides[i] = pStrides[i];
			cmd.vertexBuffers[i] = (ID3D11Buffer*)pVertexBuffers[i].buffer;
		}
		cmd.count = bufferCount;
	}
	cmds.addCommand(CmdId::SetVertexBuffers, cmd);
}
void lu::commands::SetBlendState(CommandList& cmds, BlendState& blendState, const float32_t blendFactor[4], uint32_t sampleMask)
{
	CmdSetBlendState cmd;
	cmd.pBlendState = (ID3D11BlendState*)blendState.blend;
	cmd.blendFactor[0] = blendFactor[0];
	cmd.blendFactor[1] = blendFactor[1];
	cmd.blendFactor[2] = blendFactor[2];
	cmd.blendFactor[3] = blendFactor[3];
	cmd.sampleMask = sampleMask;
	cmds.addCommand(CmdId::SetBlendState, cmd);
}
void lu::commands::SetDepthStencilState(CommandList& cmds, DepthStencilState& depthStencilState, uint32_t stencilRef)
{
	CmdSetDepthStencilState cmd;
	cmd.pDepthStencilState = (ID3D11DepthStencilState*)depthStencilState.depthStencilState;
	cmd.stencilRef = stencilRef;
	cmds.addCommand(CmdId::SetDepthStencilState, cmd);
}
void lu::commands::SetRenderTargets(CommandList& cmds, uint32_t rtCount, RenderTarget* pRenderTargets, DepthStencilTarget* pDepthStencilTarget)
{
	CmdSetRenderTargets cmd;
	if (rtCount == 1)
	{
		cmd.count = 1;
		cmd.renderTargets[0] = (ID3D11RenderTargetView*)pRenderTargets[0].renderTarget;
	}
	else
	{
		LU_ASSERT_MSG(rtCount < LU_MAX_RENDER_TARGETS, "Render Target count exceeds the max amount of render targets");
		for (uint32_t i = 0; i < rtCount; ++i)
		{
			cmd.renderTargets[i] = (ID3D11RenderTargetView*)pRenderTargets[i].renderTarget;
		}
		cmd.count = rtCount;
	}
	if (pDepthStencilTarget != NULL)
		cmd.pDepthStencilView = (ID3D11DepthStencilView*)pDepthStencilTarget->depthStencilTarget;
	else
		cmd.pDepthStencilView = NULL;
	
	cmds.addCommand(CmdId::SetRenderTargets, cmd);
}
void lu::commands::SetPixelShaderConstantBuffers(CommandList& cmds, uint32_t bufferCount, Buffer* pBuffers)
{
	CmdSetConstantBuffer cmd;
	if (bufferCount == 1)
	{
		cmd.count = 1;
		cmd.buffers[0] = (ID3D11Buffer*)pBuffers[0].buffer;
	}
	else
	{
		LU_ASSERT_MSG(bufferCount < LU_MAX_BUFFERS, "Buffer count exceeds the max amount of buffers");
		for (uint32_t i = 0; i < bufferCount; ++i)
		{
			cmd.buffers[i] = (ID3D11Buffer*)pBuffers[i].buffer;
		}
		cmd.count = bufferCount;
	}
	cmds.addCommand(CmdId::SetPixelShaderConstantBuffers, cmd);
}
void lu::commands::SetPixelShaderSamplers(CommandList& cmds, uint32_t samplerCount, SamplerState* pSamplers)
{
	CmdSetSampler cmd;
	if (samplerCount == 1)
	{
		cmd.count = 1;
		cmd.samplers[0] = (ID3D11SamplerState*)pSamplers[0].sampler;
	}
	else
	{
		LU_ASSERT_MSG(samplerCount < LU_MAX_SAMPLERS, "Sampler count exceeds the max amount of samplers");
		for (uint32_t i = 0; i < samplerCount; ++i)
		{
			cmd.samplers[i] = (ID3D11SamplerState*)pSamplers[i].sampler;
		}
		cmd.count = samplerCount;
	}
	cmds.addCommand(CmdId::SetPixelShaderSamplers, cmd);
}
void lu::commands::SetPixelShader(CommandList& cmds, PixelShader* pShader)
{
	CmdSetPixelShader cmd;
	if (pShader != NULL)
		cmd.pShader = (ID3D11PixelShader*)pShader->shader;
	else
		cmd.pShader = NULL;
	cmds.addCommand(CmdId::SetPixelShader, cmd);
}
void lu::commands::SetPixelShaderTextures(CommandList& cmds, uint32_t textureCount, Texture* pTextures)
{
	CmdSetTextures cmd;
	if (textureCount == 1 || pTextures == NULL)
	{
		cmd.count = 1;
		if (pTextures != NULL)
			cmd.shaderResourceView[0] = (ID3D11ShaderResourceView*)pTextures[0].resource;
		else
			cmd.shaderResourceView[0] = NULL;
	}
	else
	{
		LU_ASSERT_MSG(textureCount < LU_MAX_TEXTURES, "Texture count exceeds the max amount of textures");
		for (uint32_t i = 0; i < textureCount; ++i)
		{
			cmd.shaderResourceView[i] = (ID3D11ShaderResourceView*)pTextures[i].resource;
		}
		cmd.count = textureCount;
	}
	cmds.addCommand(CmdId::SetPixelShaderTextures, cmd);
}
void lu::commands::SetRasterizerState(CommandList& cmds, RasterizerState& rasterizerState)
{
	CmdSetRasterizerState cmd;
	cmd.pRasterizerState = (ID3D11RasterizerState*)rasterizerState.rasterizer;
	cmds.addCommand(CmdId::SetRasterizerState, cmd);
}
void lu::commands::SetViewports(CommandList& cmds, uint32_t viewportCount, Viewport* pViewports)
{
	CmdSetViewports cmd;
	D3D11_VIEWPORT vp;
	if (viewportCount == 1)
	{
		cmd.count = 1;
		vp.TopLeftX = pViewports->x;
		vp.TopLeftY = pViewports->y;
		vp.Width = pViewports->width;
		vp.Height = pViewports->height;
		vp.MinDepth = pViewports->nearDepth;
		vp.MaxDepth = pViewports->farDepth;

		cmd.viewports[0] = vp;
	}
	else
	{
		LU_ASSERT_MSG(viewportCount < LU_MAX_RENDER_TARGETS, "Viewport count exceeds the max amount of viewports");
		for (uint32_t i = 0; i < viewportCount; ++i)
		{
			vp.TopLeftX = pViewports[i].x;
			vp.TopLeftY = pViewports[i].y;
			vp.Width = pViewports[i].width;
			vp.Height = pViewports[i].height;
			vp.MinDepth = pViewports[i].nearDepth;
			vp.MaxDepth = pViewports[i].farDepth;

			cmd.viewports[i] = vp;
		}
		cmd.count = viewportCount;
	}
	cmds.addCommand(CmdId::SetViewports, cmd);
}
void lu::commands::SetScissors(CommandList& cmds, uint32_t scissorCount, Scissor* pScissors)
{
	CmdSetScissors cmd;
	D3D11_RECT sc;
	if (scissorCount == 1)
	{
		cmd.count = 1;
		sc.left = pScissors[0].x;
		sc.top = pScissors[0].y;
		sc.right = pScissors[0].width;
		sc.bottom = pScissors[0].height;
		cmd.scissors[0] = sc;
	}
	else
	{
		LU_ASSERT_MSG(scissorCount < LU_MAX_RENDER_TARGETS, "Scissor count exceeds the max amount of scissors");
		for (uint32_t i = 0; i < scissorCount; ++i)
		{
			sc.left = pScissors[i].x;
			sc.top = pScissors[i].y;
			sc.right = pScissors[i].width;
			sc.bottom = pScissors[i].height;
			cmd.scissors[i] = sc;
		}
		cmd.count = scissorCount;
	}
	cmds.addCommand(CmdId::SetScissors, cmd);
}
void lu::commands::SetVertexShaderConstantBuffers(CommandList& cmds, uint32_t bufferCount, Buffer* pBuffers)
{
	CmdSetConstantBuffer cmd;
	if (bufferCount == 1)
	{
		cmd.count = 1;
		cmd.buffers[0] = (ID3D11Buffer*)pBuffers[0].buffer;
	}
	else
	{
		LU_ASSERT_MSG(bufferCount < LU_MAX_BUFFERS, "Buffer count exceeds the max amount of buffers");
		for (uint32_t i = 0; i < bufferCount; ++i)
		{
			cmd.buffers[i] = (ID3D11Buffer*)pBuffers[i].buffer;
		}
		cmd.count = bufferCount;
	}
	cmds.addCommand(CmdId::SetVertexShaderConstantBuffer, cmd);
}
void lu::commands::SetVertexShaderSamplers(CommandList& cmds, uint32_t samplerCount, SamplerState* pSamplers)
{
	CmdSetSampler cmd;
	if (samplerCount == 1)
	{
		cmd.count = 1;
		cmd.samplers[0] = (ID3D11SamplerState*)pSamplers[0].sampler;
	}
	else
	{
		LU_ASSERT_MSG(samplerCount < LU_MAX_SAMPLERS, "Sampler count exceeds the max amount of samplers");
		for (uint32_t i = 0; i < samplerCount; ++i)
		{
			cmd.samplers[i] = (ID3D11SamplerState*)pSamplers[i].sampler;
		}
		cmd.count = samplerCount;
	}
	cmds.addCommand(CmdId::SetVertexShaderSamplers, cmd);
}
void lu::commands::SetVertexShader(CommandList& cmds, VertexShader* pShader)
{
	CmdSetVertexShader cmd;
	if (pShader != NULL)
	{
		cmd.pShader = (ID3D11VertexShader*)pShader->shader;
		cmd.pInputLayout = (ID3D11InputLayout*)pShader->layout;
	}
	else
	{
		cmd.pShader = NULL;
		cmd.pInputLayout = NULL;
	}
	cmds.addCommand(CmdId::SetVertexShader, cmd);
}
void lu::commands::SetVertexShaderTextures(CommandList& cmds, uint32_t textureCount, Texture* pTextures)
{
	CmdSetTextures cmd;
	if (textureCount == 1 || pTextures == NULL)
	{
		cmd.count = 1;
		if (pTextures != NULL)
			cmd.shaderResourceView[0] = (ID3D11ShaderResourceView*)pTextures[0].resource;
		else
			cmd.shaderResourceView[0] = NULL;
	}
	else
	{
		LU_ASSERT_MSG(textureCount < LU_MAX_TEXTURES, "Texture count exceeds the max amount of textures");
		for (uint32_t i = 0; i < textureCount; ++i)
		{
			cmd.shaderResourceView[i] = (ID3D11ShaderResourceView*)pTextures[i].resource;
		}
		cmd.count = textureCount;
	}
	cmds.addCommand(CmdId::SetVertexShaderTextures, cmd);
}
void lu::commands::SetGeometryShaderConstantBuffers(CommandList& cmds, uint32_t bufferCount, Buffer* pBuffers)
{
	CmdSetConstantBuffer cmd;
	if (bufferCount == 1)
	{
		cmd.count = 1;
		cmd.buffers[0] = (ID3D11Buffer*)pBuffers[0].buffer;
	}
	else
	{
		LU_ASSERT_MSG(bufferCount < LU_MAX_BUFFERS, "Buffer count exceeds the max amount of buffers");
		for (uint32_t i = 0; i < bufferCount; ++i)
		{
			cmd.buffers[i] = (ID3D11Buffer*)pBuffers[i].buffer;
		}
		cmd.count = bufferCount;
	}
	cmds.addCommand(CmdId::SetGeometryShaderConstantBuffer, cmd);
}
void lu::commands::SetGeometryShaderSamplers(CommandList& cmds, uint32_t samplerCount, SamplerState* pSamplers)
{
	CmdSetSampler cmd;
	if (samplerCount == 1)
	{
		cmd.count = 1;
		cmd.samplers[0] = (ID3D11SamplerState*)pSamplers[0].sampler;
	}
	else
	{
		LU_ASSERT_MSG(samplerCount < LU_MAX_SAMPLERS, "Sampler count exceeds the max amount of samplers");
		for (uint32_t i = 0; i < samplerCount; ++i)
		{
			cmd.samplers[i] = (ID3D11SamplerState*)pSamplers[i].sampler;
		}
		cmd.count = samplerCount;
	}
	cmds.addCommand(CmdId::SetGeometryShaderSamplers, cmd);
}
void lu::commands::SetGeometryShader(CommandList& cmds, GeometryShader* pShader)
{
	CmdSetGeometryShader cmd;
	if (pShader != NULL)
		cmd.pShader = (ID3D11GeometryShader*)pShader->shader;
	else
		cmd.pShader = NULL;
	cmds.addCommand(CmdId::SetGeometryShader, cmd);
}
void lu::commands::SetGeometryShaderTextures(CommandList& cmds, uint32_t textureCount, Texture* pTextures)
{
	CmdSetTextures cmd;
	if (textureCount == 1 || pTextures == NULL)
	{
		cmd.count = 1;
		if (pTextures != NULL)
			cmd.shaderResourceView[0] = (ID3D11ShaderResourceView*)pTextures[0].resource;
		else
			cmd.shaderResourceView[0] = NULL;
	}
	else
	{
		LU_ASSERT_MSG(textureCount < LU_MAX_TEXTURES, "Texture count exceeds the max amount of textures");
		for (uint32_t i = 0; i < textureCount; ++i)
		{
			cmd.shaderResourceView[i] = (ID3D11ShaderResourceView*)pTextures[i].resource;
		}
		cmd.count = textureCount;
	}
	cmds.addCommand(CmdId::SetGeometryShaderTextures, cmd);
}
void lu::commands::ExecuteCommandList(RendererContext& context, CommandList& cmds)
{
	ID3D11DeviceContext* pC = (ID3D11DeviceContext*)context;
	while (cmds.canConsume())
	{
		CmdId id = cmds.consumeId();
		switch (id)
		{
		case lu::CmdId::ClearDepthStencilTarget:
		{
			const CmdClearDepthStencilTarget* pCmd = cmds.consumeCommand<CmdClearDepthStencilTarget>();
			pC->ClearDepthStencilView(pCmd->pView, pCmd->clearFlag, pCmd->clearDepth, pCmd->clearStencil);
			break;
		}
		case lu::CmdId::ClearRenderTarget:
		{
			const CmdClearRenderTarget* pCmd = cmds.consumeCommand<CmdClearRenderTarget>();
			pC->ClearRenderTargetView(pCmd->pView, pCmd->clearColor);
			break;
		}
		case lu::CmdId::SetComputeShaderConstantBuffer:
		{
			const CmdSetConstantBuffer* pCmd = cmds.consumeCommand<CmdSetConstantBuffer>();
			pC->CSSetConstantBuffers(0, pCmd->count, pCmd->buffers);
			break;
		}
		case lu::CmdId::SetComputeShaderSampler:
		{
			const CmdSetSampler* pCmd = cmds.consumeCommand<CmdSetSampler>();
			pC->CSSetSamplers(0, pCmd->count, pCmd->samplers);
			break;
		}
		case lu::CmdId::SetComputeShader:
		{
			const CmdSetComputeShader* pCmd = cmds.consumeCommand<CmdSetComputeShader>();
			pC->CSSetShader(pCmd->pShader, NULL, 0);
			break;
		}
		case lu::CmdId::SetComputeShaderTextures:
		{
			const CmdSetTextures* pCmd = cmds.consumeCommand<CmdSetTextures>();
			pC->CSSetShaderResources(0, pCmd->count, pCmd->shaderResourceView);
			break;
		}
		case lu::CmdId::SetComputeShaderUAV:
		{
			const CmdSetComputeShaderUAV* pCmd = cmds.consumeCommand<CmdSetComputeShaderUAV>();
			pC->CSSetUnorderedAccessViews(0, pCmd->count, pCmd->uavs, NULL);
			break;
		}
		case lu::CmdId::Dispatch:
		{
			const CmdDispatch* pCmd = cmds.consumeCommand<CmdDispatch>();
			pC->Dispatch(pCmd->x, pCmd->y, pCmd->z);
			break;
		}
		case lu::CmdId::DispatchIndirect:
		{
			const CmdIndirect* pCmd = cmds.consumeCommand<CmdIndirect>();
			pC->DispatchIndirect(pCmd->pBufferForArgs, pCmd->byteAlignmentForArgs);
			break;
		}
		case lu::CmdId::Draw:
		{
			const CmdDraw* pCmd = cmds.consumeCommand<CmdDraw>();
			pC->Draw(pCmd->vertexCount, pCmd->startVertexLocation);
			break;
		}
		case lu::CmdId::DrawIndexed:
		{
			const CmdDrawIndexed* pCmd = cmds.consumeCommand<CmdDrawIndexed>();
			pC->DrawIndexed(pCmd->indexCount, pCmd->startIndexLocation, pCmd->baseVertexLocation);
			break;
		}
		case lu::CmdId::DrawIndexedInstanced:
		{
			const CmdDrawIndexedInstanced* pCmd = cmds.consumeCommand<CmdDrawIndexedInstanced>();
			pC->DrawIndexedInstanced(pCmd->indexCountPerInstance, pCmd->instanceCount, pCmd->startIndexLocation, pCmd->baseVertexLocation, pCmd->startInstanceLocation);
			break;
		}
		case lu::CmdId::DrawIndexedInstancedIndirect:
		{
			const CmdIndirect* pCmd = cmds.consumeCommand<CmdIndirect>();
			pC->DrawIndexedInstancedIndirect(pCmd->pBufferForArgs, pCmd->byteAlignmentForArgs);
			break;
		}
		case lu::CmdId::DrawInstanced:
		{
			const CmdDrawInstanced* pCmd = cmds.consumeCommand<CmdDrawInstanced>();
			pC->DrawInstanced(pCmd->vertexCountPerInstance, pCmd->instanceCount, pCmd->startVertexLocation, pCmd->startInstanceLocation);
			break;
		}
		case lu::CmdId::DrawInstancedIndirect:
		{
			const CmdIndirect* pCmd = cmds.consumeCommand<CmdIndirect>();
			pC->DrawInstancedIndirect(pCmd->pBufferForArgs, pCmd->byteAlignmentForArgs);
			break;
		}
		case lu::CmdId::SetPrimitiveTopology:
		{
			const CmdSetPrimitiveTopology* pCmd = cmds.consumeCommand<CmdSetPrimitiveTopology>();
			pC->IASetPrimitiveTopology(pCmd->topology);
			break;
		}
		case lu::CmdId::SetIndexBuffer:
		{
			const CmdSetIndexBuffer* pCmd = cmds.consumeCommand<CmdSetIndexBuffer>();
			pC->IASetIndexBuffer(pCmd->pIndexBuffer, pCmd->format, 0);
			break;
		}
		case lu::CmdId::SetVertexBuffers:
		{
			const CmdSetVertexBuffers* pCmd = cmds.consumeCommand<CmdSetVertexBuffers>();
			pC->IASetVertexBuffers(0, pCmd->count, pCmd->vertexBuffers, pCmd->strides, pCmd->offsets);
			break;
		}
		case lu::CmdId::SetBlendState:
		{
			const CmdSetBlendState* pCmd = cmds.consumeCommand<CmdSetBlendState>();
			pC->OMSetBlendState(pCmd->pBlendState, pCmd->blendFactor, pCmd->sampleMask);
			break;
		}
		case lu::CmdId::SetDepthStencilState:
		{
			const CmdSetDepthStencilState* pCmd = cmds.consumeCommand<CmdSetDepthStencilState>();
			pC->OMSetDepthStencilState(pCmd->pDepthStencilState, pCmd->stencilRef);
			break;
		}
		case lu::CmdId::SetRenderTargets:
		{
			const CmdSetRenderTargets* pCmd = cmds.consumeCommand<CmdSetRenderTargets>();
			pC->OMSetRenderTargets(pCmd->count, pCmd->renderTargets, pCmd->pDepthStencilView);
			break;
		}
		case lu::CmdId::SetPixelShaderConstantBuffers:
		{
			const CmdSetConstantBuffer* pCmd = cmds.consumeCommand<CmdSetConstantBuffer>();
			pC->PSSetConstantBuffers(1, pCmd->count, pCmd->buffers);
			break;
		}
		case lu::CmdId::SetPixelShaderSamplers:
		{
			const CmdSetSampler* pCmd = cmds.consumeCommand<CmdSetSampler>();
			pC->PSSetSamplers(0, pCmd->count, pCmd->samplers);
			break;
		}
		case lu::CmdId::SetPixelShader:
		{
			const CmdSetPixelShader* pCmd = cmds.consumeCommand<CmdSetPixelShader>();
			pC->PSSetShader(pCmd->pShader, NULL, 0);
			break;
		}
		case lu::CmdId::SetPixelShaderTextures:
		{
			const CmdSetTextures* pCmd = cmds.consumeCommand<CmdSetTextures>();
			pC->PSSetShaderResources(0, pCmd->count, pCmd->shaderResourceView);
			break;
		}
		case lu::CmdId::SetRasterizerState:
		{
			const CmdSetRasterizerState* pCmd = cmds.consumeCommand<CmdSetRasterizerState>();
			pC->RSSetState(pCmd->pRasterizerState);
			break;
		}
		case lu::CmdId::SetViewports:
		{
			const CmdSetViewports* pCmd = cmds.consumeCommand<CmdSetViewports>();
			pC->RSSetViewports(pCmd->count, pCmd->viewports);
			break;
		}
		case lu::CmdId::SetScissors:
		{
			const CmdSetScissors* pCmd = cmds.consumeCommand<CmdSetScissors>();
			pC->RSSetScissorRects(pCmd->count, pCmd->scissors);
			break;
		}
		case lu::CmdId::SetVertexShaderConstantBuffer:
		{
			const CmdSetConstantBuffer* pCmd = cmds.consumeCommand<CmdSetConstantBuffer>();
			pC->VSSetConstantBuffers(0, pCmd->count, pCmd->buffers);
			break;
		}
		case lu::CmdId::SetVertexShaderSamplers:
		{
			const CmdSetSampler* pCmd = cmds.consumeCommand<CmdSetSampler>();
			pC->VSSetSamplers(0, pCmd->count, pCmd->samplers);
			break;
		}
		case lu::CmdId::SetVertexShader:
		{
			const CmdSetVertexShader* pCmd = cmds.consumeCommand<CmdSetVertexShader>();
			pC->VSSetShader(pCmd->pShader, NULL, 0);
			pC->IASetInputLayout(pCmd->pInputLayout);
			break;
		}
		case lu::CmdId::SetVertexShaderTextures:
		{
			const CmdSetTextures* pCmd = cmds.consumeCommand<CmdSetTextures>();
			pC->VSSetShaderResources(0, pCmd->count, pCmd->shaderResourceView);
			break;
		}
		case lu::CmdId::SetGeometryShaderConstantBuffer:
		{
			const CmdSetConstantBuffer* pCmd = cmds.consumeCommand<CmdSetConstantBuffer>();
			pC->GSSetConstantBuffers(0, pCmd->count, pCmd->buffers);
			break;
		}
		case lu::CmdId::SetGeometryShaderSamplers:
		{
			const CmdSetSampler* pCmd = cmds.consumeCommand<CmdSetSampler>();
			pC->GSSetSamplers(0, pCmd->count, pCmd->samplers);
			break;
		}
		case lu::CmdId::SetGeometryShader:
		{
			const CmdSetGeometryShader* pCmd = cmds.consumeCommand<CmdSetGeometryShader>();
			pC->GSSetShader(pCmd->pShader, NULL, 0);
			break;
		}
		case lu::CmdId::SetGeometryShaderTextures:
		{
			const CmdSetTextures* pCmd = cmds.consumeCommand<CmdSetTextures>();
			pC->GSSetShaderResources(0, pCmd->count, pCmd->shaderResourceView);
			break;
		}
		default:
			LU_ASSERT_MSG(0, "Invalid Command ID %u", (uint32_t)id);
			return;
		}
	}
}
