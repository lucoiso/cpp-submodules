// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include <benchmark/benchmark.h>
#include <chrono>

import Timer.Manager;

static void ManagerStatus(benchmark::State& State)
{
    Timer::Manager TimerManager;
    benchmark::DoNotOptimize(TimerManager);

    bool Active {false};
    benchmark::DoNotOptimize(Active);

    for ([[maybe_unused]] auto const _: State)
    {
        TimerManager.SetActive(Active);
        Active = !Active;
    }
}
BENCHMARK(ManagerStatus);

static void TimerEmpty(benchmark::State& State)
{
    Timer::Manager TimerManager;
    benchmark::DoNotOptimize(TimerManager);

    auto EmptyLambda = [] {
    };
    benchmark::DoNotOptimize(EmptyLambda);

    for ([[maybe_unused]] auto const _: State)
    {
        TimerManager.SetTimer(std::chrono::nanoseconds(0U), EmptyLambda);
    }
}
BENCHMARK(TimerEmpty);

static void TimerCounter(benchmark::State& State)
{
    Timer::Manager TimerManager;
    benchmark::DoNotOptimize(TimerManager);

    std::uint64_t Counter {0U};
    benchmark::DoNotOptimize(Counter);

    auto EmptyLambda = [&Counter] {
        ++Counter;
    };
    benchmark::DoNotOptimize(EmptyLambda);

    for ([[maybe_unused]] auto const _: State)
    {
        TimerManager.SetTimer(std::chrono::nanoseconds(0U), EmptyLambda);
    }
}
BENCHMARK(TimerCounter);

static void TimerClear(benchmark::State& State)
{
    Timer::Manager TimerManager;
    benchmark::DoNotOptimize(TimerManager);

    std::uint64_t Counter {0U};
    benchmark::DoNotOptimize(Counter);

    auto EmptyLambda = [&Counter] {
        ++Counter;
    };
    benchmark::DoNotOptimize(EmptyLambda);

    for ([[maybe_unused]] auto const _: State)
    {
        TimerManager.SetTimer(std::chrono::milliseconds(1U), EmptyLambda);

        TimerManager.ClearTimers();
    }
}
BENCHMARK(TimerClear);