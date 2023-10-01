// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#include "Manager.h"
#include "Object.h"

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
        m_TimerObjects.clear();
    }
    catch (...)
    {
    }
}

Manager& Manager::Get()
{
    static Manager Instance{};
    return Instance;
}

std::uint64_t Manager::StartTimer(Parameters const& Parameters, std::queue<std::uint8_t>& EventIDQueue)
{
    std::lock_guard const Lock(m_Mutex);

    if (m_TimerObjects.empty())
    {
        m_TimerIDCounter = 0U;
    }

    m_TimerObjects.emplace_back(std::make_unique<Object>(m_TimerIDCounter.fetch_add(1U),
                                                         Parameters.Interval,
                                                         Parameters.RepeatCount,
                                                         Parameters.EventID,
                                                         EventIDQueue,
                                                         [this](std::uint64_t const EventID)
                                                         {
                                                             TimerFinished(std::forward<std::uint64_t const>(EventID));
                                                         }));

    m_TimerObjects.back()->Start();
    return m_TimerObjects.back()->GetID();
}

void Manager::StopTimer(std::uint64_t const TimerID)
{
    std::lock_guard const Lock(m_Mutex);

    if (auto const MatchingTimer = std::ranges::find_if(m_TimerObjects,
                                                        [TimerID](std::unique_ptr<Object> const& Timer)
                                                        {
                                                            return Timer->GetID() == TimerID;
                                                        });
        MatchingTimer != m_TimerObjects.end())
    {
        (*MatchingTimer)->Stop();
    }
}

void Manager::SetTickInterval(std::chrono::milliseconds const IntervalMs)
{
    m_TickIntervalMs = IntervalMs;
}

void Manager::TimerFinished(std::uint64_t const TimerID)
{
    std::lock_guard const Lock(m_Mutex);

    std::erase_if(m_TimerObjects,
                  [TimerID](std::unique_ptr<Object> const& Timer)
                  {
                      return Timer->GetID() == TimerID;
                  });
}

void Manager::Tick()
{
    static std::chrono::steady_clock::time_point LastTickTime = std::chrono::steady_clock::now();

    auto const CurrentTime = std::chrono::steady_clock::now();
    auto const DeltaTime   = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTickTime);
    LastTickTime           = CurrentTime;

    {
        std::lock_guard const Lock(m_Mutex);

        for (std::unique_ptr<Object> const& Timer : m_TimerObjects)
        {
            if (!Timer || !Timer->IsRunning())
            {
                continue;
            }

            Timer->Tick(DeltaTime);
        }
    }

    if (m_IsActive)
    {
        std::this_thread::sleep_for(m_TickIntervalMs);
        Tick();
    }
}
