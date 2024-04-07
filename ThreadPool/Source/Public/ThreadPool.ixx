// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "ThreadPoolModule.hpp"

export module ThreadPool;

namespace ThreadPool
{
    export class THREADPOOLMODULE_API Thread
    {
        bool                              m_Destroying { false };
        std::jthread                      m_Thread {};
        std::queue<std::function<void()>> m_Queue {};
        std::mutex                        m_Mutex {};
        std::condition_variable           m_Signal {};

        void Loop();

    public:
        Thread();
        ~Thread();

        void Enqueue(std::function<void()> const &);
        void Wait();
    };

    export class THREADPOOLMODULE_API Pool
    {
        std::vector<std::unique_ptr<Thread>> m_Threads {};

    public:
        Pool()  = default;
        ~Pool() = default;

        Pool(Pool&& other) = delete;
        Pool& operator=(Pool&& other) = delete;

        void MoveToThread(std::function<void()> const&, std::uint8_t) const;

        void SetThreadCount(uint8_t);
        void Wait() const;
    };
} // namespace ThreadPool
