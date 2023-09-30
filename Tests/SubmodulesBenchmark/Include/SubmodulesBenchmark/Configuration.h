// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include <benchmark/benchmark.h>
#include <Configuration/Manager.h>

static void ConfigurationInsertion(benchmark::State& State)
{
    auto TestManager = Configuration::Manager::Get();
    for (const auto& _ : State)
    {
        TestManager.SetValue("BenchmarkValue", 42);
    }
}

BENCHMARK(ConfigurationInsertion);

static void ConfigurationRemoval(benchmark::State& State)
{
    auto TestManager = Configuration::Manager::Get();
    for (const auto& _ : State)
    {
        TestManager.RemoveValue("BenchmarkValue");
    }
}

BENCHMARK(ConfigurationRemoval);

static void ConfigurationContains(benchmark::State& State)
{
    auto TestManager = Configuration::Manager::Get();
    TestManager.SetValue("BenchmarkValue", 42);

    for (const auto& _ : State)
    {
        bool Contains = TestManager.Contains("BenchmarkValue");
        benchmark::DoNotOptimize(Contains);
    }
}

BENCHMARK(ConfigurationContains);

static void ConfigurationSaveData(benchmark::State& State)
{
    auto TestManager = Configuration::Manager::Get();
    TestManager.SetValue("Value1", 123);
    TestManager.SetValue("Value2", "TestString");
    TestManager.SetValue("Value3", 3.14);

    const std::string Path = ".\\benchmark_test_data.json";
    for (const auto& _ : State)
    {
        TestManager.SaveData(Path);
        benchmark::DoNotOptimize(Path);
    }
}

BENCHMARK(ConfigurationSaveData);

static void ConfigurationLoadData(benchmark::State& State)
{
    auto              TestManager = Configuration::Manager::Get();
    const std::string Path        = ".\\benchmark_test_data.json";

    for (const auto& _ : State)
    {
        TestManager.LoadData(Path);
        benchmark::DoNotOptimize(Path);
    }
}

BENCHMARK(ConfigurationLoadData);
