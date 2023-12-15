// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "TimerModule.h"

export module Timer.Manager;

import <functional>;
import <cstdint>;
import <chrono>;
import <thread>;
import <mutex>;

namespace Timer
{
    class Object final
    {
        friend class Manager;

        std::uint32_t m_ID {};
        std::chrono::nanoseconds m_TimeToComplete {};
        std::chrono::nanoseconds m_ElapsedTime {};
        bool m_Active {false};

    public:
        Object(std::uint32_t const ID,
               std::chrono::nanoseconds const TimeToComplete)
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

        [[nodiscard]] std::chrono::nanoseconds GetTimeToComplete() const
        {
            return m_TimeToComplete;
        }

        [[nodiscard]] std::chrono::nanoseconds GetElapsedTime() const
        {
            return m_ElapsedTime;
        }

        [[nodiscard]] bool IsActive() const
        {
            return m_Active;
        }

        [[nodiscard]] bool Update(std::chrono::nanoseconds const DeltaTime)
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
        std::unordered_map<std::uint32_t, std::function<void()>> m_Callbacks {};
        std::chrono::steady_clock::time_point m_LastTickTime {};
        bool m_Active {false};

    public:
        Manager();
        ~Manager();

        [[nodiscard]] std::jthread::id GetThreadID() const;
        void SetTimer(std::chrono::nanoseconds const&, std::function<void()> const&);

        void SetActive(bool);
        [[nodiscard]] bool IsActive() const;

        void ClearTimers();

        [[nodiscard]] std::uint32_t GetNumTimers() const;

    private:
        void TimerFinished(std::uint32_t);
        void InitializeThreadWork();
        void StopThreadWork();
    };
}// namespace Timer