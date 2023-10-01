// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include <benchmark/benchmark.h>
#include <Configuration/Manager.h>

static void ConfigurationInsertion(benchmark::State& State)
{
    for (const auto _ : State)
    {
        Configuration::Manager::Get().SetValue("BenchmarkValue", 42);
    }
}

BENCHMARK(ConfigurationInsertion);

static void ConfigurationRemoval(benchmark::State& State)
{
    for (const auto _ : State)
    {
        Configuration::Manager::Get().RemoveValue("BenchmarkValue");
    }
}

BENCHMARK(ConfigurationRemoval);

static void ConfigurationContains(benchmark::State& State)
{
    Configuration::Manager::Get().SetValue("BenchmarkValue", 42);

    for (const auto _ : State)
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

    // ReSharper disable once CppLocalVariableMayBeConst
    std::string Path = ".\\benchmark_test_data.json";

    for (const auto _ : State)
    {
        Configuration::Manager::Get().SaveData(Path);
        benchmark::DoNotOptimize(Path);
    }
}

BENCHMARK(ConfigurationSaveData);

static void ConfigurationLoadData(benchmark::State& State)
{
    // ReSharper disable once CppLocalVariableMayBeConst
    std::string Path = ".\\benchmark_test_data.json";

    for (const auto _ : State)
    {
        Configuration::Manager::Get().LoadData(Path);
        benchmark::DoNotOptimize(Path);
    }
}

BENCHMARK(ConfigurationLoadData);
