// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "TimerModule.h"
#include <boost/log/trivial.hpp>

export module Timer.ExecutionCounter;

import <chrono>;
import <memory>;
import <string>;
import <typeinfo>;
import <utility>;
import <cstdint>;

namespace Timer
{
    void PrintExecutionTime(std::string_view const Identifier, std::chrono::nanoseconds const& Duration)
    {
        auto const CastedDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(Duration);
        auto const DurationCount  = static_cast<std::uint64_t>(CastedDuration.count());

        BOOST_LOG_TRIVIAL(info) << "[" << Identifier << "]: Execution time: " << static_cast<float>(DurationCount) * 0.000001f << "ms";
    }

    export template<typename Function, typename... Args>
    auto TimedExecution(Function&& InFunction, Args&&... InArgs)
    {
        auto const Start  = std::chrono::high_resolution_clock::now();
        auto const Result = InFunction(std::forward<Args>(InArgs)...);
        auto const End    = std::chrono::high_resolution_clock::now();
        return std::pair {Result, End - Start};
    }

    export class TIMERMODULE_API ScopedTimer
    {
        std::string m_Identifier;
        std::chrono::high_resolution_clock::time_point m_StartPoint;

    public:
        explicit ScopedTimer(std::string_view const Identifier) noexcept
            : m_Identifier(Identifier), m_StartPoint(std::chrono::high_resolution_clock::now())
        {
        }

        ~ScopedTimer() noexcept
        {
            const auto EllapsedTime   = std::chrono::high_resolution_clock::now() - m_StartPoint;
            const auto CastedDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(EllapsedTime);

            PrintExecutionTime(m_Identifier, CastedDuration);
        }
    };
}// namespace Timer