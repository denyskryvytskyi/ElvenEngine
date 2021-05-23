#include "allocpch.h"

#include <iostream>

#include "LinearAllocator.h"
#include "StackAllocator.h"
#include "CAllocator.h"
#include "Benchmark.h"

int main()
{
    const std::size_t A = static_cast<std::size_t>(1e9);

    CAllocator* cAllocator = new CAllocator();
    LinearAllocator* linearAllocator = new LinearAllocator(A);
    StackAllocator* stackAllocator = new StackAllocator(A);

    Benchmark benchmark(1000);

    std::cout << "C:" << std::endl;
    benchmark.SingleAllocation(cAllocator, 4096);
    benchmark.SingleFree(cAllocator, 4096);

    std::cout << "Linear:" << std::endl;
    benchmark.SingleAllocation(linearAllocator, 4096);

    std::cout << "Stack:" << std::endl;
    benchmark.SingleAllocation(stackAllocator, 4096);
    benchmark.SingleFree(stackAllocator, 4096);

    delete cAllocator;
    delete linearAllocator;
    delete stackAllocator;

    return 0;
}