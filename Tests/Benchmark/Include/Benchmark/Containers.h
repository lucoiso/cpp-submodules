// Copyright Notices: [...]

#pragma once

#include <benchmark/benchmark.h>

struct Target
{
    Target() = delete;
    Target(const std::uint8_t& InRef) : m_Member(InRef, UINT8_MAX) {};

private:
    std::vector<std::uint8_t> m_Member;
};

static void ContainersEmplaceCopy(benchmark::State& State)
{
    for (const auto& _ : State)
    {
        const Target Target_(0u);

        std::vector<Target> Container;
        for (std::uint8_t It = 0u; It < UINT8_MAX; ++It)
        {
            Container.emplace_back(Target_);
        }
    }
}
BENCHMARK(ContainersEmplaceCopy);

static void ContainersPushCopy(benchmark::State& State)
{
    for (const auto& _ : State)
    {
        const Target Target_(0u);

        std::vector<Target> Container;
        for (std::uint8_t It = 0u; It < UINT8_MAX; ++It)
        {
            Container.push_back(Target_);
        }
    }
}
BENCHMARK(ContainersPushCopy);

static void ContainersEmplaceConstruction(benchmark::State& State)
{
    for (const auto& _ : State)
    {
        std::vector<Target> Container;
        for (std::uint8_t It = 0u; It < UINT8_MAX; ++It)
        {
            Container.emplace_back(It);
        }
    }
}
BENCHMARK(ContainersEmplaceConstruction);

static void ContainersPushConstruction(benchmark::State& State)
{
    for (const auto& _ : State)
    {
        std::vector<Target> Container;
        for (std::uint8_t It = 0u; It < UINT8_MAX; ++It)
        {
            Container.push_back(Target(It));
        }
    }
}
BENCHMARK(ContainersPushConstruction);
