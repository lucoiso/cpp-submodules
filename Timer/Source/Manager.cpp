// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#include "Manager.h"
#include "Object.h"

using namespace Timer;

Manager Manager::g_Instance;

Manager::Manager()
    : m_TimerIDCounter(0u)
  , m_TickIntervalMs(1u)
  , m_TickThread(&Manager::Tick, this)
  , m_IsActive(true)
{
}

Manager::~Manager()
{
    std::lock_guard Lock(m_Mutex);

    m_IsActive = false;

    if (m_TickThread.joinable())
    {
        m_TickThread.join();
    }
    m_TimerObjects.clear();
}

Manager& Manager::Get()
{
    return g_Instance;
}

std::uint64_t Manager::StartTimer(const Parameters& Parameters, std::queue<std::uint8_t>& EventIDQueue)
{
    std::lock_guard Lock(m_Mutex);

    if (m_TimerObjects.empty())
    {
        m_TimerIDCounter = 0u;
    }

    m_TimerObjects.emplace_back(std::make_unique<Object>(m_TimerIDCounter.fetch_add(1u),
                                                         Parameters.Interval,
                                                         Parameters.RepeatCount,
                                                         Parameters.EventID,
                                                         EventIDQueue,
                                                         std::bind(&Manager::TimerFinished, this, std::placeholders::_1)));

    m_TimerObjects.back()->Start();
    return m_TimerObjects.back()->GetID();
}

void Manager::StopTimer(const std::uint32_t TimerID)
{
    std::lock_guard Lock(m_Mutex);

    if (const auto                                   MatchingTimer = std::ranges::find_if(m_TimerObjects,
            [TimerID](const std::unique_ptr<Object>& Timer)
            {
                return Timer->GetID() == TimerID;
            });
        MatchingTimer != m_TimerObjects.end())
    {
        (*MatchingTimer)->Stop();
    }
}

void Manager::SetTickInterval(const std::chrono::milliseconds IntervalMs)
{
    m_TickIntervalMs = IntervalMs;
}

void Manager::TimerFinished(const std::uint64_t TimerID)
{
    std::lock_guard Lock(m_Mutex);

    std::erase_if(m_TimerObjects,
                  [TimerID](const std::unique_ptr<Object>& Timer)
                  {
                      return Timer->GetID() == TimerID;
                  });
}

void Manager::Tick()
{
    static std::chrono::steady_clock::time_point LastTickTime = std::chrono::steady_clock::now();

    const std::chrono::steady_clock::time_point CurrentTime = std::chrono::steady_clock::now();
    const std::chrono::milliseconds             DeltaTime   = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTickTime);
    LastTickTime                                            = CurrentTime;

    {
        std::lock_guard Lock(m_Mutex);

        for (const std::unique_ptr<Object>& Timer : m_TimerObjects)
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
