// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include <chrono>
#include <cstdint>
#include <queue>
#include <functional>
#include <optional>

namespace Timer
{
    class Object final // NOLINT(cppcoreguidelines-special-member-functions)
    {
        friend class Manager;

    public:
        Object(std::uint64_t ID,
               std::uint32_t IntervalMs,
               std::optional<std::uint32_t> const& RepeatCount,
               std::uint8_t EventID,
               std::queue<std::uint8_t>& EventIDQueue,
               std::function<void(std::uint64_t)> const& OnFinished);

        Object(Object const& Other)            = delete;
        Object& operator=(Object const& Other) = delete;

        ~Object() = default;

        void Start();
        void Stop();

        [[nodiscard]] std::uint64_t GetID() const;
        [[nodiscard]] bool IsRunning() const;
        [[nodiscard]] std::uint8_t GetEventID() const;
        [[nodiscard]] std::optional<std::uint32_t> GetRepeatCount() const;
        [[nodiscard]] std::uint32_t GetCurrentRepeatCount() const;
        [[nodiscard]] std::chrono::milliseconds GetElapsedTime() const;

        bool operator==(Object const& Other) const
        {
            return m_ID == Other.m_ID;
        }

        bool operator!=(Object const& Other) const
        {
            return !(*this == Other);
        }

    private:
        void Tick(std::chrono::milliseconds DeltaTime);

        void SingleTime();
        void TimerLoop();

        std::uint64_t m_ID;
        bool m_IsSingleTime;
        std::chrono::milliseconds m_Interval;
        bool m_IsRunning;
        std::uint8_t m_EventID;
        std::queue<std::uint8_t>& m_EventIDQueue; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
        std::function<void(std::uint64_t)> m_OnFinished;
        std::optional<std::uint32_t> m_RepeatCount;
        std::uint32_t m_CurrentRepeatCount;
        std::chrono::milliseconds m_ElapsedTime;
    };
}
