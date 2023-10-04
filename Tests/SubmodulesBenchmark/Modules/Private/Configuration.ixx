// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <benchmark/benchmark.h>

export module Submodules.Benchmark.Config;

import Configuration.Manager;

static void ConfigurationInsertion(benchmark::State& State)
{
    for ([[maybe_unused]] auto const _: State)
    {
        Configuration::Manager::Get().SetValue("BenchmarkValue", 42);
    }
}

BENCHMARK(ConfigurationInsertion);

static void ConfigurationRemoval(benchmark::State& State)
{
    for ([[maybe_unused]] auto const _: State)
    {
        Configuration::Manager::Get().RemoveValue("BenchmarkValue");
    }
}

BENCHMARK(ConfigurationRemoval);

static void ConfigurationContains(benchmark::State& State)
{
    Configuration::Manager::Get().SetValue("BenchmarkValue", 42);

    for ([[maybe_unused]] auto const _: State)
    {
        benchmark::DoNotOptimize(Configuration::Manager::Get().Contains("BenchmarkValue"));
    }
}

BENCHMARK(ConfigurationContains);

static void ConfigurationSaveData(benchmark::State& State)
{
    Configuration::Manager::Get().SetValue("Value1", 123);
    Configuration::Manager::Get().SetValue("Value2", "TestString");
    Configuration::Manager::Get().SetValue("Value3", 3.14);

    std::string Path = ".\\benchmark_test_data.json";

    for ([[maybe_unused]] auto const _: State)
    {
        Configuration::Manager::Get().SaveData(Path);
        benchmark::DoNotOptimize(Path);
    }
}

BENCHMARK(ConfigurationSaveData);

static void ConfigurationLoadData(benchmark::State& State)
{
    std::string Path = ".\\benchmark_test_data.json";

    for ([[maybe_unused]] auto const _: State)
    {
        Configuration::Manager::Get().LoadData(Path);
        benchmark::DoNotOptimize(Path);
    }
}

BENCHMARK(ConfigurationLoadData);
