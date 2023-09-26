// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include "TimerModule.h"

namespace Timer::Tags
{
    struct TIMERMODULE_API SingleTimeTimerMode
    {
    };
    constexpr inline SingleTimeTimerMode SingleTime{};

    struct TIMERMODULE_API RepeatingTimerMode
    {
    };
    constexpr inline RepeatingTimerMode Repeating{};
};