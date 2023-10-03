// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "TimerModule.h"

export module TimerParameters;

import <optional>;

export namespace Timer
{
    struct TIMERMODULE_API Parameters
    {
        std::uint8_t EventID {};
        std::uint32_t Interval {};
        std::optional<std::uint32_t> RepeatCount;
    };
}// namespace Timer
