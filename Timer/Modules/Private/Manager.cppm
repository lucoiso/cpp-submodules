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
    : m_TimerIDCounter(0U),
      m_TickIntervalMs(1U),
      m_TickThread(&Manager::Tick, this),
      m_IsActive(true)
{
}

Manager::~Manager()
{
    try
    {
        std::lock_guard const Lock(m_Mutex);

        m_IsActive = false;

        if (m_TickThread.joinable())
        {
            m_TickThread.join();
        }
        m_Timer.clear();
    }
    catch (...)
    {
    }
}

Manager& Manager::Get()
{
    static Manager Instance {};
    return Instance;
}

std::uint32_t Manager::StartTimer(Parameters const& Parameters, std::queue<std::uint8_t>& EventIDQueue)
{
    std::lock_guard const Lock(m_Mutex);

    if (m_Timer.empty())
    {
        m_TimerIDCounter = 0U;
    }

    m_Timer.push_back(
            std::make_unique<Object>(
                    m_TimerIDCounter.fetch_add(1U),
                    Parameters.Interval,
                    Parameters.RepeatCount,
                    Parameters.EventID,
                    EventIDQueue,
                    [this](std::uint32_t const EventID) {
                        TimerFinished(std::forward<std::uint32_t const>(EventID));
                    }));

    m_Timer.back()->Start();
    return m_Timer.back()->GetID();
}

void Manager::StopTimer(std::uint32_t const TimerID)
{
    std::lock_guard const Lock(m_Mutex);

    if (auto const MatchingTimer = std::ranges::find_if(
                m_Timer,
                [TimerID](std::unique_ptr<Object> const& Timer) {
                    return Timer->GetID() == TimerID;
                });
        MatchingTimer != m_Timer.end())
    {
        (*MatchingTimer)->Stop();
    }
}

void Manager::SetTickInterval(std::chrono::milliseconds const IntervalMs)
{
    m_TickIntervalMs = IntervalMs;
}

void Manager::TimerFinished(std::uint32_t const TimerID)
{
    std::lock_guard const Lock(m_Mutex);

    std::erase_if(
            m_Timer,
            [TimerID](std::unique_ptr<Object> const& Timer) {
                return Timer->GetID() == TimerID;
            });
}

void Manager::Tick()
{
    static std::chrono::steady_clock::time_point LastTickTime = std::chrono::steady_clock::now();

    while (m_IsActive)
    {
        if (m_Mutex.try_lock())
        {
            auto const CurrentTime = std::chrono::steady_clock::now();
            auto const DeltaTime   = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTickTime);
            LastTickTime           = CurrentTime;

            for (std::unique_ptr<Object> const& Timer: m_Timer)
            {
                if (!Timer || !Timer->IsRunning())
                {
                    continue;
                }

                Timer->Tick(DeltaTime);
            }

            m_Mutex.unlock();
        }

        std::this_thread::sleep_for(m_TickIntervalMs);
    }
}