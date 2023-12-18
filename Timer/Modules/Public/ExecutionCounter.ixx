// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "TimerModule.hpp"
#include <boost/log/trivial.hpp>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <typeinfo>
#include <utility>

export module Timer.ExecutionCounter;

namespace Timer
{
    void PrintExecutionTimer(std::string_view const& Identifier, std::chrono::nanoseconds const& Duration)
    {
        BOOST_LOG_TRIVIAL(info) << "[" << Identifier << "]: Execution time: " << static_cast<float>(Duration.count()) * 0.000001f << "ms";
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
        explicit ScopedTimer(std::string Identifier) noexcept
            : m_Identifier(std::move(Identifier)), m_StartPoint(std::chrono::high_resolution_clock::now())
        {
        }

        ~ScopedTimer() noexcept
        {
            const auto EllapsedTime   = std::chrono::high_resolution_clock::now() - m_StartPoint;
            const auto CastedDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(EllapsedTime);

            PrintExecutionTimer(m_Identifier, CastedDuration);
        }
    };
}// namespace Timer