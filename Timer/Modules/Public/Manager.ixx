// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#pragma once

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
        bool m_IsSingleTime;
        std::chrono::milliseconds m_Interval {};
        bool m_IsRunning;
        std::uint8_t m_EventID {};
        std::queue<std::uint8_t>& m_EventIDQueue;
        std::function<void(std::uint32_t)> m_OnFinished {};
        std::optional<std::uint32_t> m_RepeatCount {};
        std::uint32_t m_CurrentRepeatCount {};
        std::chrono::milliseconds m_ElapsedTime {};

    public:
        Object(std::uint32_t ID,
               std::uint32_t IntervalMs,
               std::optional<std::uint32_t> const& RepeatCount,
               std::uint8_t EventID,
               std::queue<std::uint8_t>& EventIDQueue,
               std::function<void(std::uint32_t)> const& OnFinished)
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

        void Start()
        {
            if (m_IsRunning)
            {
                return;
            }

            m_IsRunning = true;
        }

        void Stop()
        {
            if (!m_IsRunning)
            {
                return;
            }

            m_IsRunning = false;
            m_OnFinished(m_ID);
        }

        [[nodiscard]] std::uint32_t GetID() const
        {
            return m_ID;
        }

        [[nodiscard]] bool IsRunning() const
        {
            return m_IsRunning;
        }

        void Tick(std::chrono::milliseconds const DeltaTime)
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
                m_EventIDQueue.push(m_EventID);
                Stop();
            }
            else
            {
                m_EventIDQueue.push(m_EventID);

                ++m_CurrentRepeatCount;
                if (m_RepeatCount.has_value() && m_CurrentRepeatCount >= m_RepeatCount)
                {
                    Stop();
                }
            }
        }
    };

    export struct TIMERMODULE_API Parameters
    {
        std::uint8_t EventID {};
        std::uint32_t Interval {};
        std::optional<std::uint32_t> RepeatCount {};
    };

    export class TIMERMODULE_API Manager
    {
        std::vector<std::unique_ptr<Object>> m_Timer;
        std::chrono::milliseconds m_TickIntervalMs;
        std::thread m_TickThread;
        std::atomic<std::uint32_t> m_TimerIDCounter;
        std::recursive_mutex m_Mutex;
        bool m_IsActive;

        Manager();
        ~Manager();

    public:
        static Manager& Get();

        [[nodiscard]] std::uint32_t StartTimer(Parameters const&, std::queue<std::uint8_t>&);
        void StopTimer(std::uint32_t);
        void SetTickInterval(std::chrono::milliseconds);

    private:
        void TimerFinished(std::uint32_t);
        void Tick();
    };
}// namespace Timer