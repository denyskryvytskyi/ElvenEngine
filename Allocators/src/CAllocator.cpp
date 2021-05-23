#include "allocpch.h"
#include "CAllocator.h"

CAllocator::CAllocator()
    : Allocator(0)
{
}

CAllocator::~CAllocator()
{
}

void CAllocator::Init()
{
}

void* CAllocator::alloc(const uint32_t size_bytes)
{
    return malloc(size_bytes);
}

void CAllocator::free(void* ptr)
{
    ::free(ptr);
}
