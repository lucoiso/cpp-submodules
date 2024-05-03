// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <chrono>
#include <string>
#include <utility>
#include "RuntimeInfoModule.hpp"

export module RuntimeInfo.ExecutionCounter;

namespace RuntimeInfo
{
    export template <typename ReturnType, typename Function, typename... Args>
    RUNTIMEINFOMODULE_API auto CountExecution(Function &&InFunction, Args &&...InArgs)
    {
        if constexpr (std::is_void_v<ReturnType>)
        {
            auto const Start = std::chrono::high_resolution_clock::now();
            InFunction(std::forward<Args>(InArgs)...);
            auto const End = std::chrono::high_resolution_clock::now();
            return std::pair {nullptr, End - Start};
        }
        else
        {
            auto const Start  = std::chrono::high_resolution_clock::now();
            auto const Result = InFunction(std::forward<Args>(InArgs)...);
            auto const End    = std::chrono::high_resolution_clock::now();
            return std::pair {Result, End - Start};
        }
    }

    export class RUNTIMEINFOMODULE_API ScopedCounter
    {
        std::string                                    m_Identifier;
        std::chrono::high_resolution_clock::time_point m_StartPoint;

    public:
        explicit ScopedCounter(std::string_view);

        ~ScopedCounter();
    };
} // namespace RuntimeInfo
