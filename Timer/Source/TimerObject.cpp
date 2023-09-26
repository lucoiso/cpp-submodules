// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#include "TimerObject.h"

using namespace Timer;

TimerObject::TimerObject(const TimerObject &Other)
    : m_ID(Other.m_ID),
      m_IsSingleTime(Other.m_IsSingleTime),
      m_Interval(Other.m_Interval),
      m_IsRunning(Other.m_IsRunning),
      m_EventID(Other.m_EventID),
      m_EventIDQueue(Other.m_EventIDQueue),
      m_OnFinished(Other.m_OnFinished),
      m_RepeatCount(Other.m_RepeatCount),
      m_CurrentRepeatCount(Other.m_CurrentRepeatCount),
      m_ElapsedTime(Other.m_ElapsedTime)
{
}

TimerObject &TimerObject::operator=(const TimerObject &Other)
{
    m_ID = Other.m_ID;
    m_IsSingleTime = Other.m_IsSingleTime;
    m_Interval = Other.m_Interval;
    m_IsRunning = Other.m_IsRunning;
    m_EventID = Other.m_EventID;
    m_EventIDQueue = Other.m_EventIDQueue;
    m_OnFinished = Other.m_OnFinished;
    m_RepeatCount = Other.m_RepeatCount;
    m_CurrentRepeatCount = Other.m_CurrentRepeatCount;
    m_ElapsedTime = Other.m_ElapsedTime;

    return *this;
}

TimerObject::TimerObject(Tags::SingleTimeTimerMode, const std::uint32_t ID, const std::uint32_t DelayMs, const std::uint8_t EventID, std::queue<std::uint8_t> &EventIDQueue, const std::function<void(std::uint32_t)> &OnFinished)
    : m_ID(ID)
    , m_IsSingleTime(true)
    , m_Interval(std::chrono::milliseconds(static_cast<std::int32_t>(DelayMs == 0u ? 1u : DelayMs)))
    , m_IsRunning(false)
    , m_EventID(EventID)
    , m_EventIDQueue(EventIDQueue)
    , m_OnFinished(OnFinished)
    , m_RepeatCount(0u)
    , m_CurrentRepeatCount(0u)
    , m_ElapsedTime(0u)
{
}

TimerObject::TimerObject(Tags::RepeatingTimerMode, const std::uint32_t ID, const std::uint32_t IntervalMs, const std::uint32_t RepeatCount, const std::uint8_t EventID, std::queue<std::uint8_t> &EventIDQueue, const std::function<void(std::uint32_t)> &OnFinished)
    : m_ID(ID)
    , m_IsSingleTime(false)
    , m_Interval(std::chrono::milliseconds(static_cast<std::int32_t>(IntervalMs == 0u ? 1u : IntervalMs)))
    , m_IsRunning(false)
    , m_EventID(EventID)
    , m_EventIDQueue(EventIDQueue)
    , m_OnFinished(OnFinished)
    , m_RepeatCount(RepeatCount)
    , m_CurrentRepeatCount(0u)
    , m_ElapsedTime(0u)
{
}

TimerObject::~TimerObject()
{
    Stop();
}

std::uint32_t TimerObject::GetID() const
{
    return m_ID;
}

void TimerObject::Start()
{
    if (m_IsRunning)
    {
        return;
    }

    m_IsRunning = true;
}

void TimerObject::Stop()
{
    if (!m_IsRunning)
    {
        return;
    }

    m_IsRunning = false;
    m_OnFinished(m_ID);
}

bool TimerObject::IsRunning() const
{
    return m_IsRunning;
}

void TimerObject::Tick(const std::chrono::milliseconds DeltaTime)
{
    if (!m_IsRunning)
    {
        return;
    }

    m_ElapsedTime += DeltaTime;

    if (m_ElapsedTime < m_Interval)
    {
        return;
    }
    else
    {
        m_ElapsedTime = std::chrono::milliseconds(0u);
    }

    if (m_IsSingleTime)
    {
        SingleTime();
    }
    else
    {
        TimerLoop();
    }
}

void TimerObject::SingleTime()
{
    if (!m_IsRunning)
    {
        return;
    }

    m_EventIDQueue.push(m_EventID);
    Stop();
}

void TimerObject::TimerLoop()
{
    if (!m_IsRunning)
    {
        return;
    }
    
    m_EventIDQueue.push(m_EventID);

    ++m_CurrentRepeatCount;
    if (m_RepeatCount > 0 && m_CurrentRepeatCount >= m_RepeatCount)
    {
        Stop();
    }
}
