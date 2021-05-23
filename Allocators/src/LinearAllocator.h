#pragma once

#include "Allocator.h"

class LinearAllocator : public Allocator
{
public:
    LinearAllocator(const uint32_t sizeTotal_bytes);

    virtual ~LinearAllocator() override;

    virtual void Init() override;

    virtual void* alloc(const uint32_t size_bytes) override;

    virtual void free(void* ptr) override final;

private:
    void* m_StartPtr = nullptr;
    uint32_t m_Marker;
};

