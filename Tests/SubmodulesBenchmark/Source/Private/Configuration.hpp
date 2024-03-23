// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include <benchmark/benchmark.h>

import Configuration.Manager;

static void ConfigurationInsertion(benchmark::State& State)
{
    for ([[maybe_unused]] auto const _: State)
    {
        Configuration::SetValue("BenchmarkValue", 42);
    }
}

BENCHMARK(ConfigurationInsertion);

static void ConfigurationRemoval(benchmark::State& State)
{
    for ([[maybe_unused]] auto const _: State)
    {
        Configuration::RemoveValue("BenchmarkValue");
    }
}

BENCHMARK(ConfigurationRemoval);

static void ConfigurationContains(benchmark::State& State)
{
    Configuration::SetValue("BenchmarkValue", 42);

    for ([[maybe_unused]] auto const _: State)
    {
        benchmark::DoNotOptimize(Configuration::Contains("BenchmarkValue"));
    }
}

BENCHMARK(ConfigurationContains);

static void ConfigurationSaveData(benchmark::State& State)
{
    Configuration::SetValue("Value1", 123);
    Configuration::SetValue("Value2", "TestString");
    Configuration::SetValue("Value3", 3.14);

    std::string Path = ".\\benchmark_test_data.json";
    benchmark::DoNotOptimize(Path);

    for ([[maybe_unused]] auto const _: State)
    {
        bool Discard = Configuration::SaveData(Path);
        benchmark::DoNotOptimize(Discard);
    }
}

BENCHMARK(ConfigurationSaveData);

static void ConfigurationLoadData(benchmark::State& State)
{
    std::string Path = ".\\benchmark_test_data.json";
    benchmark::DoNotOptimize(Path);

    for ([[maybe_unused]] auto const _: State)
    {
        bool Discard = Configuration::LoadData(Path);
        benchmark::DoNotOptimize(Discard);
    }
}

BENCHMARK(ConfigurationLoadData);
