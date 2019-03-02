#include "obj_loader.h"
#include "objpar.h"
#include "../../file.h"

bool lu::loader::CreateMeshFromOBJFile(const RendererDevice& device, IAllocator& allocator, const void* pFileData, uint64_t fileDataSize, Mesh& mesh)
{
	uint64_t objDataSize = objpar_get_size(pFileData, (uint32_t)fileDataSize);
	void* pObjDataBuff = allocator.allocate(objDataSize);
	objpar_data obj;
	objpar((const char*)pFileData, (uint32_t)fileDataSize, pObjDataBuff, &obj);
	void* pMeshData = allocator.allocate(objpar_get_mesh_size(&obj));
	objpar_mesh meshData;
	objpar_build_mesh(&obj, pMeshData, &meshData);
	mesh.vertexCount = meshData.vertex_count;
	bool result = resources::CreateMesh(device, meshData.p_vertices, meshData.vertex_count, NULL, 0, mesh);
	allocator.deallocate(pMeshData);
	allocator.deallocate(pObjDataBuff);
	return result;
}

bool lu::loader::OpenOBJFileAndCreateMesh(const RendererDevice& device, IAllocator& allocator, const char* pFileName, Mesh& mesh)
{
	byte_t* pObjFileData = NULL;
	uint64_t objFileSize = 0;
	if (!file::ReadFileData(allocator, pFileName, &pObjFileData, &objFileSize))
	{
		return false;
	}
	if (!loader::CreateMeshFromOBJFile(device, allocator, pObjFileData, objFileSize, mesh))
	{
		file::FreeFileData(allocator, pObjFileData);
		return false;
	}
	file::FreeFileData(allocator, pObjFileData);
	return true;
}
