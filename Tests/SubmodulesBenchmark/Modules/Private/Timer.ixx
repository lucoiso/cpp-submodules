// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <benchmark/benchmark.h>

export module Submodules.Benchmark.Timer;

import Timer.Manager;

static void ManagerStatus(benchmark::State& State)
{
    std::unique_ptr<Timer::Manager> TimerManager {std::make_unique<Timer::Manager>()};
    std::uint32_t Interval {1U};
    bool Active {false};

    for ([[maybe_unused]] auto const _: State)
    {
        TimerManager->SetInterval(std::chrono::milliseconds(Interval));
        TimerManager->SetActive(Active);
        Active = !Active;
        ++Interval;
    }
}
BENCHMARK(ManagerStatus);

static void TimerEmpty(benchmark::State& State)
{
    std::unique_ptr<Timer::Manager> TimerManager {std::make_unique<Timer::Manager>()};
    auto empty_lambda = [] {
    };

    for ([[maybe_unused]] auto const _: State)
    {
        TimerManager->SetTimer(0U, empty_lambda);
    }
}
BENCHMARK(TimerEmpty);

static void TimerCounter(benchmark::State& State)
{
    std::unique_ptr<Timer::Manager> TimerManager {std::make_unique<Timer::Manager>()};
    std::uint64_t Counter {0U};
    auto empty_lambda = [&Counter] {
        ++Counter;
    };

    for ([[maybe_unused]] auto const _: State)
    {
        TimerManager->SetTimer(0U, empty_lambda);
    }
}
BENCHMARK(TimerCounter);

static void TimerClear(benchmark::State& State)
{
    std::unique_ptr<Timer::Manager> TimerManager {std::make_unique<Timer::Manager>()};
    std::uint64_t Counter {0U};
    auto empty_lambda = [&Counter] {
        ++Counter;
    };

    for ([[maybe_unused]] auto const _: State)
    {
        TimerManager->SetTimer(1000U, empty_lambda);

        TimerManager->ClearTimers();
    }
}
BENCHMARK(TimerClear);