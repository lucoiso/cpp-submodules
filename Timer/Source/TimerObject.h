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
    class TimerObject
    {
        friend class TimerManager;

    public:
        TimerObject(const TimerObject &Other);
        TimerObject &operator=(const TimerObject &Other);

        TimerObject(const std::uint64_t ID, const std::uint32_t IntervalMs, const std::optional<std::uint32_t> &RepeatCount, const std::uint8_t EventID, std::queue<std::uint8_t> &EventIDQueue, const std::function<void(std::uint32_t)> &OnFinished);

        ~TimerObject();

        void Start();
        void Stop();

        std::uint64_t GetID() const;
        bool IsRunning() const;
        std::uint8_t GetEventID() const;
        std::optional<std::uint32_t> GetRepeatCount() const;
        std::uint32_t GetCurrentRepeatCount() const;
        std::chrono::milliseconds GetElapsedTime() const;

        inline bool operator==(const TimerObject &Other) const
        {
            return m_ID == Other.m_ID;
        }

        inline bool operator!=(const TimerObject &Other) const
        {
            return !(*this == Other);
        }

    private:
        void Tick(const std::chrono::milliseconds DeltaTime);

        void SingleTime();
        void TimerLoop();

        std::uint64_t m_ID;
        bool m_IsSingleTime;
        std::chrono::milliseconds m_Interval;
        bool m_IsRunning;
        std::uint8_t m_EventID;
        std::queue<std::uint8_t> &m_EventIDQueue;
        std::function<void(std::uint32_t)> m_OnFinished;
        std::optional<std::uint32_t> m_RepeatCount;
        std::uint32_t m_CurrentRepeatCount;
        std::chrono::milliseconds m_ElapsedTime;
    };
};
