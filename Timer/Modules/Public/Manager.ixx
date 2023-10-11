// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <TimerModule.h>

export module Timer.Manager;

import <atomic>;
import <chrono>;
import <memory>;
import <mutex>;
import <queue>;
import <thread>;
import <optional>;
import <cstdint>;
import <functional>;

namespace Timer
{
    class Object final
    {
        friend class Manager;

        std::uint32_t m_ID {};
        std::chrono::milliseconds m_TimeToComplete {};
        std::function<void(std::uint32_t)> m_OnFinished {};
        std::chrono::milliseconds m_ElapsedTime {};
        bool m_HasFinished {false};

    public:
        Object(std::uint32_t ID,
               std::uint32_t TimeToComplete,
               std::function<void(std::uint32_t)> const& OnFinished)
            : m_ID(ID),
              m_TimeToComplete(TimeToComplete),
              m_OnFinished(OnFinished),
              m_ElapsedTime(0U)
        {
        }

        [[nodiscard]] std::uint32_t GetID() const
        {
            return m_ID;
        }

        void Tick(std::chrono::milliseconds const DeltaTime)
        {
            if (m_HasFinished)
            {
                return;
            }

            m_ElapsedTime += DeltaTime;

            if (m_ElapsedTime < m_TimeToComplete)
            {
                return;
            }

            m_OnFinished(m_ID);
        }
    };

    export class TIMERMODULE_API Manager
    {
        std::vector<std::unique_ptr<Object>> m_Timers {};
        std::atomic<std::uint32_t> m_TimerIDCounter {};
        std::thread m_TimerThread {};
        std::recursive_mutex m_Mutex {};
        std::unordered_map<std::uint32_t, std::function<void()>> m_Callbacks {};
        std::chrono::milliseconds m_Interval {1U};
        bool m_Active {false};

    public:
        Manager();
        ~Manager();

        [[nodiscard]] std::thread::id GetThreadID() const;
        void SetTimer(std::uint32_t, std::function<void()> const&);

        void SetInterval(std::chrono::milliseconds const&);
        [[nodiscard]] std::chrono::milliseconds GetInterval() const;

        void SetActive(bool);
        [[nodiscard]] bool IsActive() const;

        [[nodiscard]] std::uint32_t GetNumTimers() const;

    private:
        void TimerFinished(std::uint32_t);
        [[noreturn]] void Tick();
    };
}// namespace Timer