// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <TimerModule.h>

export module Timer.Manager;

export import <chrono>;
export import <functional>;
export import <thread>;
export import <mutex>;
export import <cstdint>;

namespace Timer
{
    class Object final
    {
        friend class Manager;

        std::uint32_t m_ID {};
        std::chrono::milliseconds m_TimeToComplete {};
        std::function<void(std::uint32_t)> m_OnFinished {};
        std::chrono::milliseconds m_ElapsedTime {};

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
        std::jthread m_TimerThread {};
        std::recursive_mutex m_Mutex {};
        std::unordered_map<std::uint32_t, std::function<void()>> m_Callbacks {};
        std::chrono::milliseconds m_Interval {1U};

    public:
        Manager();
        ~Manager();

        [[nodiscard]] std::jthread::id GetThreadID() const;
        void SetTimer(std::uint32_t, std::function<void()> const&);

        void SetInterval(std::chrono::milliseconds const&);
        [[nodiscard]] std::chrono::milliseconds GetInterval() const;

        void SetActive(bool);
        [[nodiscard]] bool IsActive() const;

        void ClearTimers();

        [[nodiscard]] std::uint32_t GetNumTimers() const;

    private:
        void TimerFinished(std::uint32_t);
        void InitializeThreadWork();
        void StopThreadWork();

        [[noreturn]] void Tick();
    };
}// namespace Timer