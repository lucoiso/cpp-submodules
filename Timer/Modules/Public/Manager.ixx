// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "TimerModule.h"

export module Timer.Manager;

import <atomic>;
import <chrono>;
import <functional>;
import <memory>;
import <mutex>;
import <optional>;
import <queue>;
import <thread>;

import Timer.Object;
import Timer.Parameters;

export namespace Timer
{
    class TIMERMODULE_API Manager
    {
        std::vector<std::unique_ptr<class Object>> m_Timer;
        std::chrono::milliseconds m_TickIntervalMs;
        std::thread m_TickThread;
        std::atomic<std::uint64_t> m_TimerIDCounter;
        std::recursive_mutex m_Mutex;
        bool m_IsActive;

    public:
        Manager();
        ~Manager();

        static Manager& Get();

        [[nodiscard]] std::uint64_t StartTimer(struct Parameters const& Parameters, std::queue<std::uint8_t>& EventIDQueue);
        void StopTimer(std::uint64_t TimerID);
        void SetTickInterval(std::chrono::milliseconds IntervalMs);

    private:
        void TimerFinished(std::uint64_t TimerID);
        void Tick();
    };
}// namespace Timer