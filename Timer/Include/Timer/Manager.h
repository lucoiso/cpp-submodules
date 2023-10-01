// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include "Parameters.h"
#include "TimerModule.h"
#include <atomic>
#include <chrono>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace Timer
{
    class Object;

    class TIMERMODULE_API Manager
    {
    public:
        Manager();

        Manager(Manager const& Other)            = delete;
        Manager& operator=(Manager const& Other) = delete;

        ~Manager();

        static Manager& Get();

        std::uint64_t StartTimer(Parameters const& Parameters, std::queue<std::uint8_t>& EventIDQueue);
        void StopTimer(std::uint64_t TimerID);

        void SetTickInterval(std::chrono::milliseconds IntervalMs);

    private:
        void TimerFinished(std::uint64_t TimerID);
        void Tick();

        std::atomic<std::uint64_t> m_TimerIDCounter;
        std::vector<std::unique_ptr<Object>> m_TimerObjects;
        std::chrono::milliseconds m_TickIntervalMs;
        std::thread m_TickThread;
        std::recursive_mutex m_Mutex;
        bool m_IsActive;
    };
}// namespace Timer
