#pragma once

#include "../types.h"

struct _LU_PlacementNew {};
inline void* operator new(size_t, _LU_PlacementNew, void* ptr) { return ptr; }
inline void operator delete(void*, _LU_PlacementNew, void*) {}

#define LU_NEW(p)  new(_LU_PlacementNew (), p)
#define LU_UNUSED(n) ((void)&(n))
#define LU_ALIGN_ADDRESS(address, alignment) (void*)(((uintptr_t)(address) + (((uintptr_t)alignment) - 1)) & ~(((uintptr_t)alignment) - 1))
#define LU_FWD_PTR(address, bytes) (void*)((uintptr_t)(address) + bytes)
#define LU_BWD_PTR(address, bytes) (void*)((uintptr_t)(address) - bytes)
#define LU_PTR_IN_RANGE(address, min, max) ((uintptr_t)(address) >= (uintptr_t)(min) && (uintptr_t)(address) < (uintptr_t)(max))
#define LU_ADDRESS_ALIGNED(pointer, alignment) (((uintptr_t)pointer & (uintptr_t)(alignment - 1L)) == 0)
#define LU_OFFSETOF(st, m) ((size_t)&(((st *)0)->m))
#define LU_RGBA_TO_ABGR(rgba) (((rgba & 0xFF) << 24) | (((rgba >> 8) & 0xFF) << 16) | (((rgba >> 16) & 0xFF) << 8) | ((rgba >> 24) & 0xFF))
