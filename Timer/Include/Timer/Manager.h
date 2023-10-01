// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include "TimerModule.h"
#include "Parameters.h"
#include <atomic>
#include <chrono>
#include <queue>
#include <thread>
#include <mutex>
#include <vector>

namespace Timer
{
    class Object;

    class TIMERMODULE_API Manager // NOLINT(cppcoreguidelines-special-member-functions)
    {
    public:
        Manager();

        Manager(const Manager& Other)            = delete;
        Manager& operator=(const Manager& Other) = delete;

        ~Manager();

        static Manager& Get();

        std::uint64_t StartTimer(const Parameters& Parameters, std::queue<std::uint8_t>& EventIDQueue);
        void StopTimer(std::uint64_t TimerID);

        void SetTickInterval(std::chrono::milliseconds IntervalMs);

    private:
        void TimerFinished(std::uint64_t TimerID);
        void Tick();

        static Manager g_Instance;
        std::atomic<std::uint64_t> m_TimerIDCounter;
        std::vector<std::unique_ptr<Object>> m_TimerObjects;
        std::chrono::milliseconds m_TickIntervalMs;
        std::thread m_TickThread;
        std::recursive_mutex m_Mutex;
        bool m_IsActive;
    };
}
