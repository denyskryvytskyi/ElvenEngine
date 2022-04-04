#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
    explicit Semaphore(int initialCount)
        : mCount(initialCount)
        , mMutex()
        , mCv()
    {}

    void Take()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        // put the thread to sleep as long as the count is zero
        while (mCount == 0)
        {
            mCv.wait(lock);
        }
        --mCount;
    }

    void Give()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mCount++;
        // if the count was zero before the increment, wake up a waiting thread
        if (mCount == 1)
        {
            mCv.notify_one();
        }
    }

private:
    int mCount;
    std::mutex mMutex;
    std::condition_variable mCv;
};