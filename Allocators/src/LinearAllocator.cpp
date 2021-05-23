#include "allocpch.h"
#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(const uint32_t sizeTotal_bytes)
    : Allocator(sizeTotal_bytes)
    , m_Marker(0)
{
}

LinearAllocator::~LinearAllocator()
{
    ::free(m_StartPtr);
    m_StartPtr = nullptr;
}

void LinearAllocator::Init()
{
    m_StartPtr = malloc(m_TotalSize);
    m_Marker = 0;
}

void* LinearAllocator::alloc(const uint32_t size_bytes)
{
    const uint32_t currentAddress = (uint32_t)m_StartPtr + m_Marker;

    uint32_t nextAddress = currentAddress;
    m_Marker += size_bytes;

    return (void*)nextAddress;
}

void LinearAllocator::free(void* ptr)
{
    m_Marker = 0;
}
