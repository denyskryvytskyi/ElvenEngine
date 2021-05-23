#include "allocpch.h"
#include "StackAllocator.h"

StackAllocator::StackAllocator(const uint32_t totalSize_bytes)
    : Allocator(totalSize_bytes)
{
}

StackAllocator::~StackAllocator()
{
    free(m_StartPtr);
}

void StackAllocator::Init()
{
    m_StartPtr = malloc(m_TotalSize);
    m_Top = 0;
}

void* StackAllocator::alloc(const uint32_t size_bytes)
{
    const uint32_t currentAddress = (uint32_t)m_StartPtr + m_Top;

    uint32_t nextAddress = m_Top;

    m_Top += size_bytes;

    return (void*) nextAddress;
}

void StackAllocator::free(void* ptr)
{
    ::free(m_StartPtr);
    m_StartPtr = nullptr;
}

void StackAllocator::freeToMarker(Marker marker)
{
    m_Top = marker;
}
