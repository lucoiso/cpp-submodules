// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module Timer.Manager;

import <thread>;
import <mutex>;
import <numeric>;
import <algorithm>;

using namespace Timer;

Manager::Manager()
    : m_TimerIDCounter(0U)
{
    SetActive(true);
}

Manager::~Manager()
{
    try
    {
        StopThreadWork();
    }
    catch (...)
    {
    }
}

std::thread::id Manager::GetThreadID() const
{
    return m_TimerThread.get_id();
}

void Manager::SetTimer(std::chrono::nanoseconds const& Time, std::function<void()> const& Callback)
{
    if (std::empty(m_Timers))
    {
        m_TimerIDCounter = 0U;
    }

    std::uint32_t const TimerID = m_TimerIDCounter.fetch_add(1U);
    m_Callbacks.insert_or_assign(TimerID, Callback);
    m_Timers.emplace_back(TimerID, Time);
}

void Manager::SetActive(bool const Active)
{
    std::lock_guard const Lock(m_Mutex);

    if (m_Active == Active)
    {
        return;
    }

    m_Active = Active;

    if (m_Active)
    {
        InitializeThreadWork();
    }
    else
    {
        StopThreadWork();
    }
}

bool Manager::IsActive() const
{
    return m_Active;
}

void Manager::ClearTimers()
{
    std::lock_guard const Lock(m_Mutex);
    m_Timers.clear();
}

std::uint32_t Manager::GetNumTimers() const
{
    return std::ranges::count_if(m_Timers,
                                 [](Object const& Timer) {
                                     return Timer.IsActive();
                                 });
}

void Manager::TimerFinished(std::uint32_t const TimerID)
{
    std::lock_guard const Lock(m_Mutex);

    if (m_Callbacks.contains(TimerID))
    {
        m_Callbacks.at(TimerID)();
        m_Callbacks.erase(TimerID);
    }
}

void Manager::InitializeThreadWork()
{
    m_LastTickTime = std::chrono::steady_clock::now();

    m_TimerThread = std::jthread([this](std::stop_token const& Token) {
        while (Token.stop_possible() && !Token.stop_requested())
        {
            if (m_Mutex.try_lock())
            {
                auto const CurrentTime = std::chrono::steady_clock::now();
                auto const DeltaTime   = std::chrono::duration_cast<std::chrono::nanoseconds>(CurrentTime - m_LastTickTime);
                m_LastTickTime         = CurrentTime;

                for (std::uint32_t Iterator = 0U; Iterator < std::size(m_Timers); ++Iterator)
                {
                    if (auto& Timer = m_Timers[Iterator]; Timer.Update(DeltaTime))
                    {
                        TimerFinished(Timer.GetID());
                        m_Timers.erase(std::begin(m_Timers) + Iterator);
                        --Iterator;
                    }
                }

                m_Mutex.unlock();
            }
        }
    });
}

void Manager::StopThreadWork()
{
    std::lock_guard const Lock(m_Mutex);

    if (m_TimerThread.get_stop_token().stop_possible())
    {
        m_TimerThread.request_stop();
    }

    if (m_TimerThread.joinable())
    {
        m_TimerThread.join();
    }

    if (GetNumTimers() > 0U)
    {
        ClearTimers();
    }
}