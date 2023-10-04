// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

module Timer.Object;

import <chrono>;
import <functional>;
import <optional>;
import <queue>;

using namespace Timer;

Object::Object(std::uint64_t const ID,
               std::uint32_t const IntervalMs,
               std::optional<std::uint32_t> const& RepeatCount,
               std::uint8_t const EventID,
               std::queue<std::uint8_t>& EventIDQueue,
               std::function<void(std::uint64_t)> const& OnFinished)
    : m_ID(ID),
      m_IsSingleTime(RepeatCount == 0U),
      m_Interval(std::chrono::milliseconds(static_cast<std::int32_t>(IntervalMs == 0U ? 1U : IntervalMs))),
      m_IsRunning(false),
      m_EventID(EventID),
      m_EventIDQueue(EventIDQueue),
      m_OnFinished(OnFinished),
      m_RepeatCount(RepeatCount),
      m_CurrentRepeatCount(0U),
      m_ElapsedTime(0U)
{
}

void Object::Start()
{
    if (m_IsRunning)
    {
        return;
    }

    m_IsRunning = true;
}

void Object::Stop()
{
    if (!m_IsRunning)
    {
        return;
    }

    m_IsRunning = false;
    m_OnFinished(m_ID);
}

std::uint64_t Object::GetID() const
{
    return m_ID;
}

bool Object::IsRunning() const
{
    return m_IsRunning;
}

std::uint8_t Object::GetEventID() const
{
    return m_EventID;
}

std::optional<std::uint32_t> Object::GetRepeatCount() const
{
    return m_RepeatCount;
}

std::uint32_t Object::GetCurrentRepeatCount() const
{
    return m_CurrentRepeatCount;
}

std::chrono::milliseconds Object::GetElapsedTime() const
{
    return m_ElapsedTime;
}

void Object::Tick(std::chrono::milliseconds const DeltaTime)
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
    m_ElapsedTime = std::chrono::milliseconds(0U);

    if (m_IsSingleTime)
    {
        SingleTime();
    }
    else
    {
        TimerLoop();
    }
}

void Object::SingleTime()
{
    if (!m_IsRunning)
    {
        return;
    }

    m_EventIDQueue.push(m_EventID);
    Stop();
}

void Object::TimerLoop()
{
    if (!m_IsRunning)
    {
        return;
    }

    m_EventIDQueue.push(m_EventID);

    ++m_CurrentRepeatCount;
    if (m_RepeatCount.has_value() && m_CurrentRepeatCount >= m_RepeatCount)
    {
        Stop();
    }
}