// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
#include "TimerModule.hpp"

export module Timer.Manager;

namespace Timer
{
    class Object final
    {
        std::uint32_t m_ID{};
        std::chrono::nanoseconds m_TimeToComplete{};
        std::chrono::nanoseconds m_ElapsedTime{};
        bool m_Active{false};

    public:
        Object(std::uint32_t, std::chrono::nanoseconds const&);

        [[nodiscard]] std::uint32_t GetID() const;

        [[nodiscard]] std::chrono::nanoseconds GetTimeToComplete() const;

        [[nodiscard]] std::chrono::nanoseconds GetElapsedTime() const;

        [[nodiscard]] bool IsActive() const;

        [[nodiscard]] bool Update(std::chrono::nanoseconds const&);
    };

    export class TIMERMODULE_API Manager
    {
        std::vector<Object> m_Timers{};
        std::atomic<std::uint32_t> m_TimerIDCounter{};
        std::jthread m_TimerThread{};
        mutable std::mutex m_Mutex{};
        std::unordered_map<std::uint32_t, std::function<void()> > m_Callbacks{};
        std::chrono::steady_clock::time_point m_LastTickTime{};
        bool m_Active{false};

    public:
        Manager();

        ~Manager();

        [[nodiscard]] std::jthread::id GetThreadID() const;

        void SetTimer(std::chrono::nanoseconds const&, std::function<void()> const&);

        void SetActive(bool);

        [[nodiscard]] bool IsActive() const;

        void ClearTimers();

        [[nodiscard]] std::uint32_t GetNumTimers() const;

    private:
        void TimerFinished(std::uint32_t);

        void InitializeThreadWork();

        void StopThreadWork();
    };
}// namespace Timer
