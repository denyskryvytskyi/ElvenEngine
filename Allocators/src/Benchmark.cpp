#include "allocpch.h"
#include "Benchmark.h"

Benchmark::Benchmark(const uint32_t operationsCount)
    : m_OperationsCount(operationsCount)
    , m_Elapsed(0)
{
}

void Benchmark::SingleAllocation(Allocator* allocator, const std::size_t size)
{
    std::cout << "ALLOC-BENCHMARK (Single allocation):" << std::endl;
    std::cout << "\tSize: " << size << std::endl;

    StartRound();

    allocator->Init();

    uint32_t operationsCount = 0;
    while (operationsCount < m_OperationsCount)
    {
        allocator->alloc(size);
        ++operationsCount;
    }

    FinishRound();

    BenchmarkResults results = BuildResults(m_OperationsCount, m_Elapsed);

    PrintResults(results);
}

void Benchmark::SingleFree(Allocator* allocator, const std::size_t size)
{
    std::cout << "ALLOC-BENCHMARK (Single free):" << std::endl;
    std::cout << "\tSize: " << size << std::endl;

    StartRound();

    const uint64_t operationsCount = 1000;
    void* addresses[operationsCount];

    allocator->Init();

    auto operations = 0u;

    while (operations < operationsCount) {
        addresses[operations] = allocator->alloc(size);
        ++operations;
    }

    while (operations) {
        allocator->free(addresses[--operations]);
    }

    FinishRound();

    BenchmarkResults results = BuildResults(m_OperationsCount, m_Elapsed);

    PrintResults(results);
}

void Benchmark::MultipleAllocation(Allocator* allocator, const std::vector<std::size_t>& allocationSizes)
{
}

void Benchmark::MultipleFree(Allocator* allocator, const std::vector<std::size_t>& allocationSizes)
{
}


void Benchmark::PrintResults(const BenchmarkResults& results) const {
    std::cout << "\t\tOperations:    \t" << results.operations << std::endl;
    std::cout << "\t\tTime elapsed: \t" << results.elapsed.count() << " ms" << std::endl;
    std::cout << "\t\tOp per sec:    \t" << results.operationsPerSec << " ops/ms" << std::endl;
    std::cout << "\t\tTimer per op:  \t" << results.timePerOperation << " ms/ops" << std::endl;

    std::cout << std::endl;
}

const BenchmarkResults Benchmark::BuildResults(std::size_t operationsCount, std::chrono::milliseconds elapsedTime) const {
    BenchmarkResults results;

    results.operations = operationsCount;
    results.elapsed = std::move(elapsedTime);
    results.operationsPerSec = results.operations / static_cast<double>(results.elapsed.count());
    results.timePerOperation = static_cast<double>(results.elapsed.count()) / static_cast<double>(results.operations);

    return results;
}