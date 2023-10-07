// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

export module Timer.Object;

import <chrono>;
import <functional>;
import <optional>;
import <queue>;

export namespace Timer
{
    class Object final
    {
        friend class Manager;

        std::uint32_t m_ID {};
        bool m_IsSingleTime;
        std::chrono::milliseconds m_Interval {};
        bool m_IsRunning;
        std::uint8_t m_EventID {};
        std::queue<std::uint8_t>& m_EventIDQueue;
        std::function<void(std::uint32_t)> m_OnFinished {};
        std::optional<std::uint32_t> m_RepeatCount {};
        std::uint32_t m_CurrentRepeatCount {};
        std::chrono::milliseconds m_ElapsedTime {};

    public:
        Object(std::uint32_t ID,
               std::uint32_t IntervalMs,
               std::optional<std::uint32_t> const& RepeatCount,
               std::uint8_t EventID,
               std::queue<std::uint8_t>& EventIDQueue,
               std::function<void(std::uint32_t)> const& OnFinished);

        void Start();
        void Stop();

        [[nodiscard]] std::uint32_t GetID() const;
        [[nodiscard]] bool IsRunning() const;

        void Tick(std::chrono::milliseconds);

    private:
        void SingleTime();
        void TimerLoop();
    };
}// namespace Timer