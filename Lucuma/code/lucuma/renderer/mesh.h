#pragma once

#include "../math.h"
#include "resources/buffer.h"
#include "resources/command_list.h"

namespace lu
{
	struct MeshVertex
	{
		Vec3 position;
		Vec2 texCoord;
		Vec3 normal;
	};

	struct Mesh
	{
		uint32_t vertexCount;
		uint32_t indexCount;
		Buffer vertexBuffer;
		Buffer indexBuffer;
	};

	namespace resources
	{
		bool CreateMesh(const RendererDevice& device, const void* pVertices, uint32_t vertexCount, const void* pIndices, uint32_t indexCount, Mesh& mesh);
		void DestroyMesh(const RendererDevice& device, Mesh& mesh);
	}

	namespace mesh
	{
		void DrawMesh(CommandList& cmds, Mesh& mesh);
	}
}
