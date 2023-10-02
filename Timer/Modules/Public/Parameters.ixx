// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "TimerModule.h"
#include <optional>

export module Timer.Parameters;

namespace Timer
{
    export struct TIMERMODULE_API Parameters
    {
        std::uint8_t EventID {};
        std::uint32_t Interval {};
        std::optional<std::uint32_t> RepeatCount;
    };
}// namespace Timer
