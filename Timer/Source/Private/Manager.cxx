// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <algorithm>
#include <chrono>
#include <mutex>
#include <numeric>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <pthread.h>
#endif

module Timer.Manager;

using namespace Timer;

Object::Object(std::uint32_t const ID, std::chrono::nanoseconds const &TimeToComplete)
    : m_ID(ID)
  , m_TimeToComplete(TimeToComplete)
  , m_ElapsedTime(0U)
  , m_Active(true)
{
}

std::uint32_t Object::GetID() const
{
    return m_ID;
}

std::chrono::nanoseconds Object::GetTimeToComplete() const
{
    return m_TimeToComplete;
}

std::chrono::nanoseconds Object::GetElapsedTime() const
{
    return m_ElapsedTime;
}

bool Object::IsActive() const
{
    return m_Active;
}

bool Object::Update(std::chrono::nanoseconds const &DeltaTime)
{
    m_ElapsedTime += DeltaTime;
    m_Active = m_ElapsedTime < m_TimeToComplete;
    return !m_Active;
}

Manager::Manager()
    : m_TimerIDCounter(0U)
{
}

Manager::~Manager()
{
    StopThreadWork();
}

std::jthread::id Manager::GetThreadID() const
{
    return m_TimerThread.get_id();
}

void Manager::SetTimer(std::chrono::nanoseconds const &Time, std::function<void()> const &Callback)
{
    if (std::empty(m_Timers))
    {
        m_TimerIDCounter.fetch_sub(m_TimerIDCounter.load());
    }

    std::uint32_t const TimerID = m_TimerIDCounter.fetch_add(1U);
    m_Callbacks.insert_or_assign(TimerID, Callback);
    m_Timers.emplace_back(TimerID, Time);
}

void Manager::SetupThread(std::string_view const &ThreadName)
{
    #ifdef _WIN32
    HANDLE           ThreadHandle = m_TimerThread.native_handle();
    PROCESSOR_NUMBER ProcessorNumber { .Group = 0U, .Number = 0u, .Reserved = 0U };
    SetThreadIdealProcessorEx(ThreadHandle, &ProcessorNumber, nullptr);
    SetThreadPriority(ThreadHandle, THREAD_PRIORITY_NORMAL);

    std::wstring const       WThreadName { std::begin(ThreadName), std::end(ThreadName) };
    [[maybe_unused]] HRESULT const _ = SetThreadDescription(ThreadHandle, std::data(WThreadName));
    #else
    pthread_t ThreadHandle = m_TimerThread.native_handle();
    cpu_set_t CPUSet;
    CPU_ZERO(&CPUSet);
    CPU_SET(0U, &CPUSet);
    pthread_setaffinity_np(ThreadHandle, sizeof(cpu_set_t), &CPUSet);
    #endif
}

void Manager::SetActive(bool const Active)
{
    if (m_Active == Active)
    {
        return;
    }

    m_Active = Active;

    if (m_Active)
    {
        InitializeThreadWork();
    }
    else
    {
        StopThreadWork();
    }
}

bool Manager::IsActive() const
{
    return m_Active;
}

void Manager::ClearTimers()
{
    std::lock_guard const Lock(m_Mutex);

    m_Timers.clear();
}

std::size_t Manager::GetNumTimers() const
{
    std::lock_guard const Lock(m_Mutex);

    return std::ranges::count_if(m_Timers,
                                 [](Object const &Timer)
                                 {
                                     return Timer.IsActive();
                                 });
}

void Manager::TimerFinished(std::uint32_t const TimerID)
{
    if (m_Callbacks.contains(TimerID))
    {
        m_Callbacks.at(TimerID)();
        m_Callbacks.erase(TimerID);
    }
}

void Manager::InitializeThreadWork()
{
    m_LastTickTime = std::chrono::steady_clock::now();

    m_TimerThread = std::thread([this]
    {
        while (m_Active)
        {
            std::unique_lock const Lock(m_Mutex);

            auto const CurrentTime = std::chrono::steady_clock::now();
            auto const DeltaTime   = std::chrono::duration_cast<std::chrono::nanoseconds>(CurrentTime - m_LastTickTime);
            m_LastTickTime         = CurrentTime;

            for (std::uint32_t Iterator = 0U; Iterator < std::size(m_Timers); ++Iterator)
            {
                if (!m_Active)
                {
                    break;
                }

                if (auto &Timer = m_Timers[Iterator];
                    Timer.Update(DeltaTime))
                {
                    TimerFinished(Timer.GetID());
                    m_Timers.erase(std::begin(m_Timers) + Iterator);
                    --Iterator;
                }
            }
        }
    });
}

void Manager::StopThreadWork()
{
    m_Active = false;

    if (m_TimerThread.joinable())
    {
        m_TimerThread.join();
    }

    if (GetNumTimers() > 0U)
    {
        ClearTimers();
    }
}
