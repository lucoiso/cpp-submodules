// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

export module Submodules.Benchmark.Timer;

import <benchmark/benchmark.h>;

import Timer.Manager;

static void TimerPlaceholder(benchmark::State& State)
{
    for ([[maybe_unused]] auto const _: State)
    {
    }
}

BENCHMARK(TimerPlaceholder);