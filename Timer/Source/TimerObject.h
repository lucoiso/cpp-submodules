// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include "TimerModule.h"
#include "TimerTags.h"
#include <chrono>
#include <cstdint>
#include <queue>
#include <functional>

namespace Timer
{
    class TIMERMODULE_API TimerObject
    {
        friend class TimerManager;

    public:
        TimerObject(Tags::SingleTimeTimerMode, const std::uint32_t ID, const std::uint32_t DelayMs, const std::uint8_t EventID, std::queue<std::uint8_t> &EventIDQueue, const std::function<void(std::uint32_t)> &OnFinished);

        TimerObject(Tags::RepeatingTimerMode, const std::uint32_t ID, const std::uint32_t IntervalMs, const std::uint32_t RepeatCount, const std::uint8_t EventID, std::queue<std::uint8_t> &EventIDQueue, const std::function<void(std::uint32_t)> &OnFinished);

        ~TimerObject();

        std::uint32_t GetID() const;

        void Start();
        void Stop();

        bool IsRunning() const;

        inline bool operator==(const TimerObject &Other) const
        {
            return m_ID == Other.m_ID;
        }

        inline bool operator!=(const TimerObject &Other) const
        {
            return !(*this == Other);
        }

    private:
        void Tick(const std::chrono::milliseconds DeltaTime);

        void SingleTime();
        void TimerLoop();

        std::uint32_t m_ID;
        bool m_IsSingleTime;
        std::chrono::milliseconds m_Interval;
        bool m_IsRunning;
        std::uint8_t m_EventID;
        std::queue<std::uint8_t> &m_EventIDQueue;
        std::function<void(std::uint32_t)> m_OnFinished;
        std::uint32_t m_RepeatCount;
        std::uint32_t m_CurrentRepeatCount;
        std::chrono::milliseconds m_ElapsedTime;
    };
};
