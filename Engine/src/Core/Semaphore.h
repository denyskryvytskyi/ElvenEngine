#pragma once

#include <condition_variable>
#include <mutex>

namespace Elven {

class Semaphore {
public:
    explicit Semaphore(int initialCount)
        : m_count(initialCount)
        , m_mutex()
        , m_cv()
    {
    }

    void Take()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        // put the thread to sleep as long as the count is zero
        while (m_count == 0) {
            m_cv.wait(lock);
        }
        --m_count;
    }

    void Give()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_count++;
        // if the count was zero before the increment, wake up a waiting thread
        if (m_count == 1) {
            m_cv.notify_one();
        }
    }

private:
    int m_count { 0 };
    std::mutex m_mutex;
    std::condition_variable m_cv;
};

} // namespace Elven
