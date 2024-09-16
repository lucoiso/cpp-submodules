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
        std::thread                       m_Thread {};
        std::queue<std::function<void()>> m_Queue {};
        std::mutex                        m_Mutex {};
        std::condition_variable           m_Signal {};

        void Loop();

    public:
        Thread();
        ~Thread();

        void SetAffinity(std::uint8_t, std::string_view);
        void Enqueue(std::function<void()> const &);
        void Wait();
        void Detach();
    };

    export class THREADPOOLMODULE_API Pool
    {
        std::uint8_t m_NumThread { 0U };
        std::vector<std::unique_ptr<Thread>> m_Threads {};

    public:
        Pool()  = default;
        ~Pool() = default;

        Pool(Pool &&other)            = delete;
        Pool &operator=(Pool &&other) = delete;

        void AddTask(std::function<void()> const &, std::uint8_t) const;

        void SetThreadCount(std::uint8_t);
        [[nodiscard]] inline std::uint8_t GetThreadCount() const
        {
            return m_NumThread;
        }

        void SetupCPUThreads(std::string_view);
        void Wait() const;

        void Abort();
        void Detach() const;
    };
} // namespace ThreadPool
