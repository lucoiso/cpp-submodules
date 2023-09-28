// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#include "TimerManager.h"
#include "TimerObject.h"

using namespace Timer;

TimerManager TimerManager::m_Instance;

TimerManager::TimerManager()
    : m_TimerIDCounter(0u)
    , m_TickIntervalMs(1u)
    , m_TickThread(&TimerManager::Tick, this)
    , m_IsActive(true)
{
}

TimerManager::~TimerManager()
{
    std::lock_guard<std::recursive_mutex> Lock(m_Mutex);

    m_IsActive = false;
    
    if (m_TickThread.joinable())
    {
        m_TickThread.join();
    }
    m_TimerObjects.clear();
}

TimerManager &TimerManager::Get()
{
    return m_Instance;
}

std::uint64_t TimerManager::StartTimer(const TimerParameters &Parameters, std::queue<std::uint8_t> &EventIDQueue)
{
    std::lock_guard<std::recursive_mutex> Lock(m_Mutex);

    if (m_TimerObjects.empty())
    {
        m_TimerIDCounter = 0u;
    }

    m_TimerObjects.emplace_back(std::make_unique<TimerObject>(m_TimerIDCounter.fetch_add(1u),
                                                              Parameters.Interval,
                                                              Parameters.RepeatCount,
                                                              Parameters.EventID,
                                                              EventIDQueue,
                                                              std::bind(&TimerManager::TimerFinished, this, std::placeholders::_1)));

    m_TimerObjects.back()->Start();
    return m_TimerObjects.back()->GetID();
}

void TimerManager::StopTimer(const std::uint32_t TimerID)
{
    std::lock_guard<std::recursive_mutex> Lock(m_Mutex);

    if (auto MatchingTimer = std::find_if(m_TimerObjects.begin(), m_TimerObjects.end(), [TimerID](const std::unique_ptr<TimerObject> &Timer)
                                          { return Timer->GetID() == TimerID; });
        MatchingTimer != m_TimerObjects.end())
    {
        (*MatchingTimer)->Stop();
    }
}

void TimerManager::SetTickInterval(const std::chrono::milliseconds IntervalMs)
{
    m_TickIntervalMs = IntervalMs;
}

void TimerManager::TimerFinished(const std::uint64_t TimerID)
{
    std::lock_guard<std::recursive_mutex> Lock(m_Mutex);

    std::erase_if(m_TimerObjects, [TimerID](const std::unique_ptr<TimerObject> &Timer)
                  { return Timer->GetID() == TimerID; });
}

void TimerManager::Tick()
{
    static std::chrono::steady_clock::time_point LastTickTime = std::chrono::steady_clock::now();

    const std::chrono::steady_clock::time_point CurrentTime = std::chrono::steady_clock::now();
    const std::chrono::milliseconds DeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTickTime);
    LastTickTime = CurrentTime;

    {
        std::lock_guard<std::recursive_mutex> Lock(m_Mutex);

        for (const std::unique_ptr<TimerObject> &Timer : m_TimerObjects)
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
