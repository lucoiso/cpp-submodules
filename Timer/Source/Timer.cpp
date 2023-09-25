// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#include "Timer.h"

using namespace Timer;

TimerObject::TimerObject(const std::uint8_t FrameRate)
    : m_FrameRate(FrameRate)
    , m_FrameInterval(std::chrono::milliseconds(1000) / (FrameRate == 0u ? 1u : FrameRate))
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
    m_Thread = std::thread(&TimerObject::TimerLoop, this);
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

void TimerObject::TimerLoop()
{
    while (m_IsRunning)
    {
        const auto Start = std::chrono::high_resolution_clock::now();

        m_Functor();

        const auto End = std::chrono::high_resolution_clock::now();
        const auto Elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(End - Start);

        if (Elapsed < m_FrameInterval)
        {
            std::this_thread::sleep_for(m_FrameInterval - Elapsed);
        }
    }
}