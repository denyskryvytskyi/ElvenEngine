#pragma once

/*
* Base allocator class for linear, stack and pool allocators without alignment for now
*/
class Allocator
{
public:
    Allocator(const std::size_t sizeTotal_bytes) :
        m_TotalSize(sizeTotal_bytes)
    {}

    virtual ~Allocator()
    {
        m_TotalSize = 0;
    }

    virtual void Init() = 0;

    virtual void* alloc(const uint32_t size_bytes) = 0;

    virtual void free(void* ptr) = 0;

protected:
    uint32_t m_TotalSize;
};