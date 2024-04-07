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
                              return !m_Queue.empty() || m_Destroying;
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
        m_Thread.join();
    }
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
                  [this]()
                  {
                      return m_Queue.empty();
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

void Pool::MoveToThread(std::function<void()> const& Execution, std::uint8_t const Index) const
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
