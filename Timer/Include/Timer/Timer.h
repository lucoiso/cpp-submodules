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
    class TIMERMODULE_API TimerObject
    {
    public:
        TimerObject(const std::uint8_t FrameRate);
        ~TimerObject();

        void Start(const std::function<void()> &Functor);
        void Stop();

    private:
        void TimerLoop();

        std::uint8_t m_FrameRate;
        std::chrono::milliseconds m_FrameInterval;
        bool m_IsRunning;
        std::thread m_Thread;
        std::function<void()> m_Functor;
    };
};