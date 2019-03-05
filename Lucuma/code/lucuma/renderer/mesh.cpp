#include "mesh.h"

bool lu::resources::CreateMesh(const RendererDevice& device, const void* pVertices, uint32_t vertexCount, const void* pIndices, uint32_t indexCount, Mesh& mesh)
{
	if (!resources::CreateBuffer(device, vertexCount * sizeof(MeshVertex), BufferUsage::USAGE_IMMUTABLE, BufferBind::BIND_VERTEX_BUFFER, CPUAccess::CPU_ACCESS_NONE, ResourceType::RESOURCE_NONE, 0, pVertices, mesh.vertexBuffer))
	{
		return false;
	}
	if (pIndices != NULL)
	{
		if (!resources::CreateBuffer(device, indexCount * sizeof(uint16_t), BufferUsage::USAGE_IMMUTABLE, BufferBind::BIND_INDEX_BUFFER, CPUAccess::CPU_ACCESS_NONE, ResourceType::RESOURCE_NONE, 0, pIndices, mesh.indexBuffer))
		{
			return false;
		}
	}
	mesh.vertexCount = vertexCount;
	mesh.indexCount = indexCount;
	return true;
}

void lu::resources::DestroyMesh(const RendererDevice& device, Mesh& mesh)
{
	resources::DestroyBuffer(device, mesh.vertexBuffer);
	resources::DestroyBuffer(device, mesh.indexBuffer);
	mesh.vertexCount = 0;
	mesh.indexCount = 0;
}

void lu::mesh::DrawMesh(CommandList& cmds, Mesh& mesh)
{
	uint32_t strides = sizeof(MeshVertex);
	uint32_t offsets = 0;
	commands::SetVertexBuffers(cmds, 0, 1, &mesh.vertexBuffer, &strides, &offsets);
	if (mesh.indexCount > 0)
	{
		commands::SetIndexBuffer(cmds, ResourceFormat::FORMAT_R16_UINT, mesh.indexBuffer);
		commands::DrawIndexed(cmds, mesh.indexCount, 0, 0);
	}
	else
	{
		commands::Draw(cmds, mesh.vertexCount, 0);
	}
}
