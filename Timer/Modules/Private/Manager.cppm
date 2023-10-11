// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module Timer.Manager;

import <atomic>;
import <chrono>;
import <functional>;
import <memory>;
import <mutex>;
import <optional>;
import <queue>;
import <thread>;

using namespace Timer;

Manager::Manager()
    : m_Timers(),
      m_TimerIDCounter(0U),
      m_TimerThread(),
      m_Mutex(),
      m_Callbacks(),
      m_Interval(1U),
      m_Active(false)
{
    SetActive(true);
}

Manager::~Manager()
{
    try
    {
        m_Active = false;

        std::lock_guard const Lock(m_Mutex);

        if (m_TimerThread.joinable())
        {
            m_TimerThread.join();
        }

        m_Timers.clear();
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

    if (m_Timers.empty())
    {
        m_TimerIDCounter = 0U;
    }

    std::uint32_t const TimerID = m_TimerIDCounter.fetch_add(1U);
    m_Callbacks.insert_or_assign(TimerID, Callback);

    m_Timers.push_back(
            std::make_unique<Object>(
                    TimerID,
                    Time,
                    [this](std::uint32_t const EventID) {
                        TimerFinished(std::forward<std::uint32_t const>(EventID));
                    }));
}

void Manager::SetInterval(std::chrono::milliseconds const& Interval)
{
    m_Interval = Interval;
}

std::chrono::milliseconds Manager::GetInterval() const
{
    return m_Interval;
}

void Manager::SetActive(bool const Active)
{
    if (m_Active == Active)
    {
        return;
    }

    m_Active = Active;

    if (Active)
    {
        m_TimerThread = std::thread([this]() {
            Tick();
        });
    }
    else
    {
        if (m_TimerThread.joinable())
        {
            m_TimerThread.join();
        }
    }
}

bool Manager::IsActive() const
{
    return m_Active;
}

std::uint32_t Manager::GetNumTimers() const
{
    return static_cast<std::uint32_t>(m_Timers.size());
}

void Manager::TimerFinished(std::uint32_t const TimerID)
{
    std::lock_guard const Lock(m_Mutex);

    if (m_Callbacks.contains(TimerID))
    {
        m_Callbacks[TimerID]();
        m_Callbacks.erase(TimerID);
    }

    std::erase_if(
            m_Timers,
            [TimerID](std::unique_ptr<Object> const& Timer) {
                return Timer->GetID() == TimerID;
            });
}

void Manager::Tick()
{
    static std::chrono::steady_clock::time_point LastTickTime = std::chrono::steady_clock::now();

    while (m_Active)
    {
        if (m_Mutex.try_lock())
        {
            auto const CurrentTime = std::chrono::steady_clock::now();
            auto const DeltaTime   = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTickTime);
            LastTickTime           = CurrentTime;

            for (std::unique_ptr<Object> const& Timer: m_Timers)
            {
                if (!Timer)
                {
                    continue;
                }

                Timer->Tick(DeltaTime);
            }

            m_Mutex.unlock();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(m_Interval));
    }
}