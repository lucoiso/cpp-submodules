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
    m_IsActive = false;

    m_TimerObjects.clear();

    if (m_TickThread.joinable())
    {
        m_TickThread.join();
    }
}

TimerManager &TimerManager::Get()
{
    return m_Instance;
}

std::uint32_t TimerManager::StartTimer(Tags::SingleTimeTimerMode, const std::uint8_t EventID, const std::uint32_t DelayMs, std::queue<std::uint8_t> &EventIDQueue)
{
    std::lock_guard<std::recursive_mutex> Lock(m_Mutex);

    m_TimerObjects.emplace_back(std::make_unique<TimerObject>(Tags::SingleTime, m_TimerIDCounter.fetch_add(1u), DelayMs, EventID, EventIDQueue, std::bind(&TimerManager::TimerFinished, this, std::placeholders::_1)));

    m_TimerObjects.back()->Start();
    return m_TimerObjects.back()->GetID();
}

std::uint32_t TimerManager::StartTimer(Tags::RepeatingTimerMode, const std::uint8_t EventID, const std::uint32_t Interval, const std::uint32_t RepeatCount, std::queue<std::uint8_t> &EventIDQueue)
{
    std::lock_guard<std::recursive_mutex> Lock(m_Mutex);

    m_TimerObjects.emplace_back(std::make_unique<TimerObject>(Tags::Repeating, m_TimerIDCounter.fetch_add(1u), Interval, RepeatCount, EventID, EventIDQueue, std::bind(&TimerManager::TimerFinished, this, std::placeholders::_1)));

    m_TimerObjects.back()->Start();
    return m_TimerObjects.back()->GetID();
}

void TimerManager::StopTimer(const std::uint32_t TimerID)
{
    std::lock_guard<std::recursive_mutex> Lock(m_Mutex);

    if (auto MatchingTimer = std::find_if(m_TimerObjects.begin(), m_TimerObjects.end(), [TimerID](const std::unique_ptr<TimerObject> &Timer)
                                          { return Timer && Timer->GetID() == TimerID; });
        MatchingTimer != m_TimerObjects.end())
    {
        m_TimerObjects.erase(MatchingTimer);
    }
}

void TimerManager::TimerFinished(const std::uint32_t TimerID)
{
    StopTimer(TimerID);
}

void TimerManager::Tick()
{
    static std::chrono::steady_clock::time_point LastTickTime = std::chrono::steady_clock::now();

    while (m_IsActive)
    {
        const std::chrono::steady_clock::time_point CurrentTime = std::chrono::steady_clock::now();
        const std::chrono::milliseconds DeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTickTime);
        LastTickTime = CurrentTime;

        {
            std::lock_guard<std::recursive_mutex> Lock(m_Mutex);

            for (std::uint32_t Iterator = 0u; Iterator < m_TimerObjects.size(); ++Iterator)
            {
                if (!m_TimerObjects[Iterator] || !m_TimerObjects[Iterator]->IsRunning())
                {
                    continue;
                }

                m_TimerObjects[Iterator]->Tick(DeltaTime);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(m_TickIntervalMs));
    }
}
