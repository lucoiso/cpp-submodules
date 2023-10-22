// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module Timer.Manager;

using namespace Timer;

Manager::Manager()
    : m_Timers(),
      m_TimerIDCounter(0U),
      m_TimerThread(),
      m_Mutex(),
      m_Callbacks(),
      m_Interval(1U)
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

void Manager::SetTimer(std::uint32_t const Time, std::function<void()> const& Callback)
{
    std::lock_guard const Lock(m_Mutex);

    if (std::empty(m_Timers))
    {
        m_TimerIDCounter = 0U;
    }

    std::uint32_t const TimerID = m_TimerIDCounter.fetch_add(1U);
    m_Callbacks.insert_or_assign(TimerID, Callback);

    m_Timers.push_back(
            std::make_unique<Object>(
                    TimerID,
                    Time,
                    [this](std::uint32_t const EventID) {
                        TimerFinished(std::forward<std::uint32_t const>(EventID));
                    }));
}

void Manager::SetInterval(std::chrono::milliseconds const& Interval)
{
    m_Interval = Interval;
}

std::chrono::milliseconds Manager::GetInterval() const
{
    return m_Interval;
}

void Manager::SetActive(bool const Active)
{
    if (Active && !m_TimerThread.get_stop_token().stop_possible())
    {
        InitializeThreadWork();
    }
    else if (!Active && m_TimerThread.get_stop_token().stop_possible())
    {
        StopThreadWork();
    }
}

bool Manager::IsActive() const
{
    return m_TimerThread.get_stop_token().stop_possible();
}

void Manager::ClearTimers()
{
    std::lock_guard const Lock(m_Mutex);
    m_Timers.clear();
}

std::uint32_t Manager::GetNumTimers() const
{
    return static_cast<std::uint32_t>(std::size(m_Timers));
}

void Manager::TimerFinished(std::uint32_t const TimerID)
{
    std::lock_guard const Lock(m_Mutex);

    if (m_Callbacks.contains(TimerID))
    {
        m_Callbacks[TimerID]();
        m_Callbacks.erase(TimerID);
    }

    std::erase_if(
            m_Timers,
            [TimerID](std::unique_ptr<Object> const& Timer) {
                return Timer->GetID() == TimerID;
            });
}

void Manager::InitializeThreadWork()
{
    if (IsActive())
    {
        StopThreadWork();
    }

    m_TimerThread = std::jthread([this]() {
        Tick();
    });
}

void Manager::StopThreadWork()
{
    if (m_TimerThread.get_stop_token().stop_possible())
    {
        m_TimerThread.request_stop();
    }

    if (m_TimerThread.joinable())
    {
        m_TimerThread.join();
    }

    ClearTimers();
}

void Manager::Tick()
{
    static std::chrono::steady_clock::time_point LastTickTime = std::chrono::steady_clock::now();

    while (!m_TimerThread.get_stop_token().stop_requested())
    {
        if (m_Mutex.try_lock())
        {
            auto const CurrentTime = std::chrono::steady_clock::now();
            auto const DeltaTime   = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTickTime);
            LastTickTime           = CurrentTime;

            for (std::unique_ptr<Object> const& Timer: m_Timers)
            {
                if (!Timer)
                {
                    continue;
                }

                Timer->Tick(DeltaTime);
            }

            m_Mutex.unlock();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(m_Interval));
    }
}