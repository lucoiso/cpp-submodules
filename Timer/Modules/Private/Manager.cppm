// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module Timer.Manager;

import <thread>;
import <mutex>;
import <numeric>;
import <algorithm>;

using namespace Timer;

Manager::Manager()
    : m_Timers(),
      m_TimerIDCounter(0U),
      m_TimerThread(),
      m_Mutex(),
      m_Callbacks(),
      m_Interval(1U)
{
    SetActive(true);
}

Manager::~Manager()
{
    try
    {
        StopThreadWork();
    }
    catch (...)
    {
    }
}

std::thread::id Manager::GetThreadID() const
{
    return m_TimerThread.get_id();
}

void Manager::SetTimer(std::uint32_t const Time, std::function<void()> const& Callback)
{
    std::lock_guard const Lock(m_Mutex);

    if (std::empty(m_Timers))
    {
        m_TimerIDCounter = 0U;
    }

    std::uint32_t const TimerID = m_TimerIDCounter.fetch_add(1U);
    m_Callbacks.insert_or_assign(TimerID, Callback);

    // avoid spamming the log with the same message
    {
        static std::uint32_t LastTimerID {std::numeric_limits<std::uint32_t>::max()};
        if (LastTimerID != TimerID)
        {
            LastTimerID = TimerID;
        }
    }

    m_Timers.emplace_back(TimerID, Time);
}

void Manager::SetInterval(std::chrono::milliseconds const Interval)
{
    m_Interval = Interval;
}

std::chrono::milliseconds Manager::GetInterval() const
{
    return m_Interval;
}

void Manager::SetActive(bool const Active)
{
    std::lock_guard const Lock(m_Mutex);

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

std::uint32_t Manager::GetNumTimers() const
{
    return std::count_if(
            std::begin(m_Timers),
            std::end(m_Timers),
            [](Object const& Timer) {
                return Timer.IsActive();
            });
}

void Manager::TimerFinished(std::uint32_t const TimerID)
{
    std::lock_guard const Lock(m_Mutex);

    if (m_Callbacks.contains(TimerID))
    {
        m_Callbacks.at(TimerID)();
        m_Callbacks.erase(TimerID);
    }
}

void Manager::InitializeThreadWork()
{
    std::lock_guard const Lock(m_Mutex);

    m_TimerThread = std::jthread([this]() {
        RefreshTimers();
    });
}

void Manager::StopThreadWork()
{
    std::lock_guard const Lock(m_Mutex);

    if (m_TimerThread.get_stop_token().stop_possible())
    {
        m_TimerThread.request_stop();
    }

    if (m_TimerThread.joinable())
    {
        m_TimerThread.join();
    }

    if (GetNumTimers() > 0U)
    {
        ClearTimers();
    }
}

void Manager::RefreshTimers()
{
    static std::chrono::steady_clock::time_point LastTickTime = std::chrono::steady_clock::now();

    while (IsActive() && !m_TimerThread.get_stop_token().stop_requested())
    {
        auto const CurrentTime = std::chrono::steady_clock::now();
        auto const DeltaTime   = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTickTime);
        LastTickTime           = CurrentTime;

        if (m_Mutex.try_lock())
        {
            for (Object& Iterator: m_Timers)
            {
                if (!Iterator.Update(DeltaTime))
                {
                    continue;
                }

                TimerFinished(Iterator.GetID());
            }

            std::erase_if(
                    m_Timers,
                    [](Object const& Timer) {
                        return !Timer.IsActive();
                    });

            m_Mutex.unlock();
        }

        std::unique_lock UniqueLock(m_IntervalMutex);
        m_ConditionVariable.wait_for(UniqueLock, GetInterval());
    }
}