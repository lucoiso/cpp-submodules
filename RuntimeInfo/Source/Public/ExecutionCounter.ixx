// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "RuntimeInfoModule.hpp"
#include <chrono>
#include <string>
#include <utility>

export module RuntimeInfo.ExecutionCounter;

namespace RuntimeInfo
{
    export template <typename Function, typename... Args>
    RUNTIMEINFOMODULE_API auto CountExecution(Function&& InFunction,
                        Args&&... InArgs)
    {
        auto const Start = std::chrono::high_resolution_clock::now();
        auto const Result = InFunction(std::forward<Args>(InArgs)...);
        auto const End = std::chrono::high_resolution_clock::now();
        return std::pair{
            Result,
            End - Start
        };
    }

    export class RUNTIMEINFOMODULE_API ScopedCounter
    {
        std::string m_Identifier;
        std::chrono::high_resolution_clock::time_point m_StartPoint;

    public:
        explicit ScopedCounter(std::string_view);

        ~ScopedCounter();
    };
} // namespace RuntimeInfo
