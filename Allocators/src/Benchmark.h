#pragma once

#include <chrono>
#include <vector>

#include "Allocator.h"

struct BenchmarkResults
{
    std::size_t operations;
    std::chrono::milliseconds elapsed;
    double operationsPerSec;
    double timePerOperation;
};

class Benchmark
{
public:
    Benchmark() = delete;

    Benchmark(const uint32_t operationsCount);

    void SingleAllocation(Allocator* allocator, const std::size_t size);
    void SingleFree(Allocator* allocator, const std::size_t size);

    void MultipleAllocation(Allocator* allocator, const std::vector<std::size_t>& allocationSizes);
    void MultipleFree(Allocator* allocator, const std::vector<std::size_t>& allocationSizes);

private:
    void SetStartTime() noexcept { m_Start = std::chrono::high_resolution_clock::now(); }
    void SetFinishTime() noexcept { m_Finish = std::chrono::high_resolution_clock::now(); }
    void SetElapsedTime() noexcept { m_Elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(m_Finish - m_Start); }

    void StartRound() noexcept { SetStartTime(); }
    void FinishRound() noexcept
    {
        SetFinishTime();
        SetElapsedTime();
    }

    void PrintResults(const BenchmarkResults& results) const;
    const BenchmarkResults BuildResults(std::size_t operations, std::chrono::milliseconds elapsedTime) const;

private:
    const std::size_t m_OperationsCount;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Finish;

    std::chrono::milliseconds m_Elapsed;
};

