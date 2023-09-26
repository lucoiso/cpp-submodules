// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include "TimerModule.h"
#include "TimerObject.h"
#include "TimerTags.h"
#include <atomic>
#include <chrono>
#include <queue>
#include <thread>
#include <mutex>
#include <vector>

namespace Timer
{
    class TimerObject;

    class TIMERMODULE_API TimerManager
    {
    public:
        TimerManager();
        ~TimerManager();

        static TimerManager &Get();

        std::uint32_t StartTimer(Tags::SingleTimeTimerMode, const std::uint8_t EventID, const std::uint32_t DelayMs, std::queue<std::uint8_t> &EventIDQueue);

        std::uint32_t StartTimer(Tags::RepeatingTimerMode, const std::uint8_t EventID, const std::uint32_t Interval, const std::uint32_t RepeatCount, std::queue<std::uint8_t> &EventIDQueue);

        void StopTimer(const std::uint32_t TimerID);

        void SetTickInterval(const std::chrono::milliseconds IntervalMs);

    private:
        void TimerFinished(const std::uint32_t TimerID);

        void Tick();

        static TimerManager m_Instance;

        std::atomic<std::uint32_t> m_TimerIDCounter;
        std::vector<TimerObject> m_TimerObjects;

        std::chrono::milliseconds m_TickIntervalMs;

        std::thread m_TickThread;
        std::recursive_mutex m_Mutex;

        bool m_IsActive;
    };
};
