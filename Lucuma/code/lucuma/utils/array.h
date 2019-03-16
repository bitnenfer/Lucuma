#pragma once

#include "../debug.h"
#include "../types.h"
#include "allocator.h"

#include <string.h>

namespace lu
{
	template<class T>
	struct IArray
	{
		// Abstract Methods
		virtual void resize(uint32_t newSize) = 0;
		virtual void finalize() {}

		IArray() : pBuffer(NULL), size(0), capacity(0) {}
		IArray(uint32_t size)
		{
			initialize(size);
		}
		~IArray()
		{
			finalize();
		}

		void initialize(uint32_t startSize)
		{
			if (pBuffer != NULL) return;
			size = 0;
			resize(startSize);
		}

		T* alloc()
		{
			if (size + 1 > capacity || pBuffer == NULL)
			{
				uint32_t newCapacity = capacity * 2; // Maybe have something more flexible
				if (newCapacity < 1) newCapacity = 2;
				resize(newCapacity);
				capacity = newCapacity;
			}
			size += 1;
			return &pBuffer[size - 1];
		}
		
		void push(const T& element)
		{
			if (size + 1 > capacity || pBuffer == NULL)
			{
				uint32_t newCapacity = capacity * 2; // Maybe have something more flexible
				if (newCapacity < 1) newCapacity = 2;
				resize(newCapacity);
				capacity = newCapacity;
			}
			pBuffer[size++] = element;
		}

		void pushBuffer(const T* pBuffer, uint32_t elementCount)
		{
			for (uint32_t i = 0; i < elementCount; ++i)
			{
				push(pBuffer[i]);
			}
		}

		void setBuffer(uint32_t start, const T* pBuffer, uint32_t elementCount)
		{
			LU_ASSERT(start <= size - 1);
			const uint32_t newSize = start + elementCount;
			if (newSize > capacity)
			{
				resize(newSize);
			}

			for (uint32_t i = 0; i < elementCount; ++i)
			{
				this->pBuffer[start + i] = pBuffer[i];
			}
			if (newSize > size)
			{
				size = newSize;
			}
		}

		void pushRepeat(const T& element, uint32_t count)
		{
			for (uint32_t i = 0; i < elementCount; ++i)
			{
				push(element);
			}
		}

		void pushArray(const IArray<T>& otherArray)
		{
			pushBuffer(otherArray.pBuffer, otherArray.size);
		}

		void setArray(uint32_t start, const IArray<T>& otherArray)
		{
			setBuffer(start, otherArray.pBuffer, otherArray.size);
		}

		void pop()
		{
			LU_ASSERT(pBuffer != NULL);
			if (size > 0)
			{
				size -= 1;
			}
		}

		void removeAt(uint32_t index)
		{
			if (index == size - 1)
			{
				size -= 1;
			}
			else if (index < size)
			{
				for (uint32_t i = index + 1; i < size; ++i)
				{
					pBuffer[i - 1] = pBuffer[i];
				}
			}
		}

		T& operator[](uint32_t index)
		{
			return at(index);
		}

		T& at(uint32_t index)
		{
			LU_ASSERT(pBuffer != NULL && index < size);
			return pBuffer[index];
		}

		const T& constAt(uint32_t index) const
		{
			LU_ASSERT(pBuffer != NULL && index < size);
			return pBuffer[index];
		}

		T* getBuffer() { return pBuffer; }
		uint32_t getSize() const { return size; }
		uint32_t getCapacity() const { return capacity; }
		bool isInitialized() const { return pBuffer != NULL; }

	protected:
		T* pBuffer;
		uint32_t size;
		uint32_t capacity;
	};

	template<class T>
	struct Array : public IArray<T>
	{
		void resize(uint32_t newSize) override
		{
			if (newSize < capacity) return;

			LU_ASSERT(pAllocator != NULL);
			if (pBuffer != NULL)
			{
				T* pNewBuffer = (T*)pAllocator->allocate(newSize * sizeof(T));
				memcpy(pNewBuffer, pBuffer, size * sizeof(T));
				pAllocator->deallocate(pBuffer);
				pBuffer = pNewBuffer;
			}
			else
			{
				pBuffer = (T*)pAllocator->allocate(newSize * sizeof(T));
			}
			capacity = newSize;
		}
		void finalize() override
		{
			if (pBuffer != NULL)
			{
				pAllocator->deallocate(pBuffer);
				pBuffer = NULL;
				capacity = 0;
				size = 0;
			}
		}

		typedef IArray<T> ParentType;
		Array() : ParentType(), pAllocator(NULL) {};
		Array(IAllocator* pAllocator) : ParentType(), pAllocator(pAllocator) {}
		Array(IAllocator* pAllocator, uint32_t size) : ParentType(), pAllocator(pAllocator) 
		{
			initialize(size);
		}

		/*
		* BE WARNED! Use this function carefully!
		* Changing an already set allocator will allocate
		* a new buffer with the new allocator and then memcpy
		* the old buffer data to the new one and finally 
		* release the old buffer that is attached to the old 
		* allocator. This process will invalidate any address
		* assigned from the old allocator.
		* If no allocator is set it'll just set the allocator.
		*/
		void setAllocator(IAllocator* pNewAllocator)
		{
			if (pAllocator == NULL)
			{
				pAllocator = pNewAllocator;
			}
			else if (pAllocator != pNewAllocator)
			{
				T* pNewBuffer = (T*)pNewAllocator->allocate(capacity);
				memcpy(pNewBuffer, pBuffer, size);
				pAllocator->deallocate(pBuffer);
				pBuffer = pNewBuffer;
			}
		}
		IAllocator* getAllocator() { return pAllocator; }

	protected:
		IAllocator* pAllocator;
	};

	template<class T, unsigned int Size>
	struct SmallArray : public IArray<T>
	{
		static constexpr uint32_t kMaxSize = 4096;
		
		LU_STATIC_ASSERT(Size < kMaxSize);

		typedef IArray<T> ParentType;
		void resize(uint32_t newSize) override
		{
			if (pBuffer == NULL)
			{
				pBuffer = &fixedBuffer[0];
			}
			else
			{
				LU_ASSERT(0); // Can't grow
			}
			capacity = newSize;
		}
		void finalize() override
		{
			if (pBuffer != NULL)
			{
				pBuffer = NULL;
				capacity = 0;
				size = 0;
			}
		}

		SmallArray() : ParentType() 
		{
			initialize(Size);
		}

	protected:
		T fixedBuffer[Size];
	};
}