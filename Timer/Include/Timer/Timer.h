// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include "TimerModule.h"
#include <functional>
#include <chrono>
#include <thread>
#include <cstdint>

namespace Timer
{
    struct TIMERMODULE_API SingleTimeTimerMode
    {
    };
    constexpr inline SingleTimeTimerMode TAG_SingleTime{};

    struct TIMERMODULE_API RepeatingTimerMode
    {
    };
    constexpr inline RepeatingTimerMode TAG_Repeating{};

    class TIMERMODULE_API TimerObject
    {
    public:
        TimerObject(const std::uint8_t Delay, SingleTimeTimerMode);
        TimerObject(const std::uint8_t Rate, RepeatingTimerMode);
        ~TimerObject();

        void Start(const std::function<void()> &Functor);
        void Stop();

    private:
        void SingleTime();
        void TimerLoop();

        bool m_IsSingleTime;
        std::chrono::milliseconds m_Interval;
        bool m_IsRunning;
        std::thread m_Thread;
        std::function<void()> m_Functor;
    };
};
