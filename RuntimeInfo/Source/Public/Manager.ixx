// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <mutex>
#include <source_location>
#include <vector>
#include "RuntimeInfoModule.hpp"

export module RuntimeInfo.Manager;

import RuntimeInfo.ExecutionCounter;

namespace RuntimeInfo
{
    export class RUNTIMEINFOMODULE_API Manager
    {
        std::vector<std::source_location> m_Callstack {};
        std::mutex                        m_CallstackMutex {};
        std::uint8_t                      m_CallstackLimit { 8U };
        bool                              m_Active { true };

        Manager() = default;

        ~Manager() = default;

        void InsertCallstack(std::source_location &&Location);

    public:
        [[nodiscard]] static Manager &Get();

        void PushCallstack(std::source_location Location = std::source_location::current());

        [[nodiscard]] ScopedCounter PushCallstackWithCounter(std::source_location Location = std::source_location::current());

        [[nodiscard]] bool IsActive() const;

        void SetActive(bool);

        void PopCallstack();

        void Reset();

        void SetCallstackLimit(std::uint8_t);

        [[nodiscard]] std::vector<std::source_location> const &GetCallstack() const;

        [[nodiscard]] std::uint8_t GetCallstackLimit() const;
    };
} // namespace RuntimeInfo
