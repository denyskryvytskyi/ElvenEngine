#pragma once

#include "Allocator.h"

class StackAllocator : public Allocator
{
public:
    typedef uint32_t Marker;

public:
    StackAllocator(const uint32_t totalSize_bytes);

    virtual ~StackAllocator();

    virtual void Init();

    virtual void* alloc(const uint32_t size_bytes) override;

    virtual void free(void* ptr) override;

    virtual Marker getMarker() { return m_Top; }

    // Rolls the stack back to a previous marker.
    virtual void freeToMarker(Marker marker);

private:
    void* m_StartPtr;

    Marker m_Top;
    Marker m_PreviousMarker;
};
