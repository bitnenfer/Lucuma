#include "lucuma/lucuma.h"
#include "lucuma/utils/loaders/gltf.h"

void demo4()
{
	lu::PageAllocator pageAllocator;
	byte_t* pFileBuffer = NULL;
	uint64_t fileSize = 0;
	lu::GLTF boat;
	LU_ASSERT(lu::file::ReadFileData(pageAllocator, "data/boat.glb", &pFileBuffer, &fileSize));
	lu::gltf::Parse(pageAllocator, pFileBuffer, fileSize, boat);

	lu::file::FreeFileData(pageAllocator, pFileBuffer);
	pFileBuffer = NULL;
	fileSize = 0;
}
