// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#include "Timer.h"

using namespace Timer;

TimerObject::TimerObject(const std::uint8_t Delay, SingleTimeTimerMode)
    : m_IsSingleTime(true)
    , m_Interval(std::chrono::milliseconds(static_cast<std::int32_t>((Delay == 0u ? 1u : Delay) * 1000u)))
    , m_IsRunning(false)
{
}

TimerObject::TimerObject(const std::uint8_t Rate, RepeatingTimerMode)
    : m_IsSingleTime(false)
    , m_Interval(std::chrono::milliseconds(1000) / static_cast<std::int32_t>(Rate == 0u ? 1u : Rate))
    , m_IsRunning(false)
{
}

TimerObject::~TimerObject()
{
    Stop();
}

void TimerObject::Start(const std::function<void()> &Functor)
{
    if (m_IsRunning)
    {
        return;
    }

    m_IsRunning = true;
    m_Functor = Functor;

    if (m_IsSingleTime)
    {
        m_Thread = std::thread(&TimerObject::SingleTime, this);
    }
    else
    {
        m_Thread = std::thread(&TimerObject::TimerLoop, this);
    }
}

void TimerObject::Stop()
{
    if (!m_IsRunning)
    {
        return;
    }

    m_IsRunning = false;
    m_Thread.join();
}

void TimerObject::SingleTime()
{
    if (m_IsRunning)
    {
        std::this_thread::sleep_for(m_Interval);
        m_Functor();
        Stop();
    }
}

void TimerObject::TimerLoop()
{
    while (m_IsRunning)
    {
        const auto Start = std::chrono::high_resolution_clock::now();

        m_Functor();

        const auto End = std::chrono::high_resolution_clock::now();
        const auto Elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(End - Start);

        if (Elapsed < m_Interval)
        {
            std::this_thread::sleep_for(m_Interval - Elapsed);
        }
    }
}
