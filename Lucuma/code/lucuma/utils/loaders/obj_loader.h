#pragma once

#include "../allocator.h"
#include "../../renderer/mesh.h"

namespace lu
{
	namespace loader
	{
		bool CreateMeshFromOBJFile(const RendererDevice& device, IAllocator& allocator, const void* pFileData, uint64_t fileDataSize, Mesh& mesh);
		bool OpenOBJFileAndCreateMesh(const RendererDevice& device, IAllocator& allocator, const char* pFileName, Mesh& mesh);
	}
}
