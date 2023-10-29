// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <TimerModule.h>

export module Timer.Manager;

export import <functional>;
export import <cstdint>;
export import <chrono>;
import <thread>;
import <mutex>;
import <condition_variable>;

namespace Timer
{
    class Object final
    {
        friend class Manager;

        std::uint32_t m_ID {};
        std::chrono::milliseconds m_TimeToComplete {};
        std::chrono::milliseconds m_ElapsedTime {};
        bool m_Active {false};

    public:
        Object(std::uint32_t ID,
               std::uint32_t TimeToComplete)
            : m_ID(ID),
              m_TimeToComplete(TimeToComplete),
              m_ElapsedTime(0U),
              m_Active(true)
        {
        }

        [[nodiscard]] std::uint32_t GetID() const
        {
            return m_ID;
        }

        [[nodiscard]] std::chrono::milliseconds GetTimeToComplete() const
        {
            return m_TimeToComplete;
        }

        [[nodiscard]] std::chrono::milliseconds GetElapsedTime() const
        {
            return m_ElapsedTime;
        }

        [[nodiscard]] bool IsActive() const
        {
            return m_Active;
        }

        [[nodiscard]] bool Update(std::chrono::milliseconds const DeltaTime)
        {
            m_ElapsedTime += DeltaTime;
            m_Active = m_ElapsedTime < m_TimeToComplete;
            return !m_Active;
        }
    };

    export class TIMERMODULE_API Manager
    {
        std::vector<Object> m_Timers {};
        std::atomic<std::uint32_t> m_TimerIDCounter {};
        std::jthread m_TimerThread {};
        std::recursive_mutex m_Mutex {};
        std::mutex m_IntervalMutex {};
        std::condition_variable m_ConditionVariable {};
        std::unordered_map<std::uint32_t, std::function<void()>> m_Callbacks {};
        std::chrono::milliseconds m_Interval {1U};
        bool m_Active {false};

    public:
        Manager();
        ~Manager();

        [[nodiscard]] std::jthread::id GetThreadID() const;
        void SetTimer(std::uint32_t, std::function<void()> const&);

        void SetInterval(std::chrono::milliseconds);
        [[nodiscard]] std::chrono::milliseconds GetInterval() const;

        void SetActive(bool);
        [[nodiscard]] bool IsActive() const;

        void ClearTimers();

        [[nodiscard]] std::uint32_t GetNumTimers() const;

    private:
        void TimerFinished(std::uint32_t);
        void InitializeThreadWork();
        void StopThreadWork();

        [[noreturn]] void RefreshTimers();
    };
}// namespace Timer