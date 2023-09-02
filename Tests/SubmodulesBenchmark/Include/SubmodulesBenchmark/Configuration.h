// Copyright Notices: [...]

#pragma once

#include <benchmark/benchmark.h>
#include <Configuration/Manager.h>

static void ConfigurationInsertion(benchmark::State& state)
{
    const auto TestManager = Configuration::Manager::GetInstance();
    for (const auto& _ : state)
    {
        TestManager->SetValue("BenchmarkValue", 42);
    }
}
BENCHMARK(ConfigurationInsertion);

static void ConfigurationRemoval(benchmark::State& state)
{
    const auto TestManager = Configuration::Manager::GetInstance();
    for (const auto& _ : state)
    {
        TestManager->RemoveValue("BenchmarkValue");
    }
}
BENCHMARK(ConfigurationRemoval);

static void ConfigurationContains(benchmark::State& state)
{
    const auto TestManager = Configuration::Manager::GetInstance();
    TestManager->SetValue("BenchmarkValue", 42);

    for (const auto& _ : state)
    {
        bool Contains = TestManager->Contains("BenchmarkValue");
        benchmark::DoNotOptimize(Contains);
    }
}
BENCHMARK(ConfigurationContains);

static void ConfigurationSaveData(benchmark::State& state)
{
    const auto TestManager = Configuration::Manager::GetInstance();
    TestManager->SetValue("Value1", 123);
    TestManager->SetValue("Value2", "TestString");
    TestManager->SetValue("Value3", 3.14);

    std::string Path = ".\\benchmark_test_data.json";
    for (const auto& _ : state)
    {
        TestManager->SaveData(Path);
        benchmark::DoNotOptimize(Path);
    }
}
BENCHMARK(ConfigurationSaveData);

static void ConfigurationLoadData(benchmark::State& state)
{
    const auto TestManager = Configuration::Manager::GetInstance();
    std::string Path = ".\\benchmark_test_data.json";

    for (const auto& _ : state)
    {
        TestManager->LoadData(Path);
        benchmark::DoNotOptimize(Path);
    }
}
BENCHMARK(ConfigurationLoadData);
