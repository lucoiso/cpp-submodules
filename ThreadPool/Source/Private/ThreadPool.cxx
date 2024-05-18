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

#ifdef _WIN32
#include <Windows.h>
#else
    #include <pthread.h>
#endif

module ThreadPool;

using namespace ThreadPool;

void Thread::Loop()
{
    while (!m_Thread.get_stop_source().stop_requested())
    {
        std::function<void()> Execution;
        {
            std::unique_lock Lock(m_Mutex);
            m_Signal.wait(Lock,
                          [this]
                          {
                              return !std::empty(m_Queue) || m_Destroying;
                          });

            if (m_Destroying)
            {
                break;
            }
            Execution = m_Queue.front();
        }

        Execution();

        {
            std::unique_lock Lock(m_Mutex);
            m_Queue.pop();
            m_Signal.notify_one();
        }
    }
}

Thread::Thread()
    : m_Thread([this]
    {
        Loop();
    })
{
}

Thread::~Thread()
{
    if (m_Thread.joinable())
    {
        Wait();
        m_Mutex.lock();
        m_Destroying = true;
        m_Signal.notify_one();
        m_Mutex.unlock();
        m_Thread.request_stop();
        m_Thread.join();
    }
}

void Thread::SetAffinity(std::uint8_t const ThreadIndex, std::string_view const Prefix)
{
    #ifdef _WIN32
    HANDLE           ThreadHandle = m_Thread.native_handle();
    PROCESSOR_NUMBER ProcessorNumber { .Group = 0U, .Number = ThreadIndex, .Reserved = 0U };
    SetThreadIdealProcessorEx(ThreadHandle, &ProcessorNumber, nullptr);
    SetThreadPriority(ThreadHandle, THREAD_PRIORITY_NORMAL);

    std::string const        StandardName = std::format("{} {}", Prefix, ThreadIndex);
    std::wstring const       ThreadName { std::begin(StandardName), std::end(StandardName) };
    [[maybe_unused]] HRESULT const _ = SetThreadDescription(ThreadHandle, std::data(ThreadName));
    #else
    pthread_t ThreadHandle = m_Thread.native_handle();
    cpu_set_t CPUSet;
    CPU_ZERO(&CPUSet);
    CPU_SET(ThreadIndex, &CPUSet);
    pthread_setaffinity_np(ThreadHandle, sizeof(cpu_set_t), &CPUSet);
    #endif
}

void Thread::Enqueue(std::function<void()> const &Execution)
{
    std::unique_lock Lock(m_Mutex);
    m_Queue.push(std::move(Execution));
    m_Signal.notify_one();
}

void Thread::Wait()
{
    std::unique_lock Lock(m_Mutex);
    m_Signal.wait(Lock,
                  [this]
                  {
                      return std::empty(m_Queue);
                  });
}

void Pool::SetThreadCount(uint8_t const Value)
{
    m_Threads.clear();

    if (Value > 0U)
    {
        m_Threads.reserve(Value);
        for (uint8_t Iterator = 0; Iterator < Value; ++Iterator)
        {
            m_Threads.push_back(std::make_unique<Thread>());
        }
    }
}

void Pool::SetupCPUThreads(std::string_view const Prefix)
{
    SetThreadCount(std::thread::hardware_concurrency());

    for (uint8_t Iterator = 0; Iterator < std::size(m_Threads); ++Iterator)
    {
        m_Threads.at(Iterator)->SetAffinity(Iterator, Prefix);
    }
}

void Pool::AddTask(std::function<void()> const &Execution, std::uint8_t const Index) const
{
    m_Threads.at(Index)->Enqueue(std::move(Execution));
}

void Pool::Wait() const
{
    for (auto &ThreadIter : m_Threads)
    {
        ThreadIter->Wait();
    }
}
