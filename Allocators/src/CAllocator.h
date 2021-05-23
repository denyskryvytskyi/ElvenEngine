#pragma once

#include "Allocator.h"

class CAllocator : public Allocator
{
public:
    CAllocator();

    virtual ~CAllocator() override;

    virtual void Init() override;

    virtual void* alloc(const uint32_t size_bytes) override;

    virtual void free(void* ptr) override;

};