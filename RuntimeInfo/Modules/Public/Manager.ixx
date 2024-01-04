// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "RuntimeInfoModule.hpp"
#include <mutex>
#include <source_location>
#include <vector>

export module RuntimeInfo.Manager;

import RuntimeInfo.ExecutionCounter;

namespace RuntimeInfo
{
    export class RUNTIMEINFOMODULE_H Manager
    {
        std::vector<std::source_location> m_Callstack {};
        std::mutex m_CallstackMutex {};
        std::uint8_t m_CallstackLimit {8U};

        Manager()  = default;
        ~Manager() = default;

        void InsertCallstack(std::source_location&& Location);

    public:
        static Manager& Get();

        void PushCallstack(std::source_location Location = std::source_location::current());
        [[nodiscard]] ScopedCounter PushCallstackWithCounter(std::source_location Location = std::source_location::current());

        void PopCallstack();
        void SetCallstackLimit(std::uint8_t);

        [[nodiscard]] std::vector<std::source_location> const& GetCallstack() const;
    };
}// namespace RuntimeInfo
