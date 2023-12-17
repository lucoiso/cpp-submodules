// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <latch>
#include <thread>

import Timer.Manager;

TEST_CASE("Manager Status", "[Timer]")
{
    Timer::Manager TimerManager;
    REQUIRE(TimerManager.IsActive());
    REQUIRE(TimerManager.GetNumTimers() == 0U);

    TimerManager.SetTimer(std::chrono::milliseconds(1U), [] {
    });
    REQUIRE(TimerManager.GetNumTimers() == 1U);
    TimerManager.ClearTimers();
    REQUIRE(TimerManager.GetNumTimers() == 0U);

    TimerManager.SetActive(false);
    REQUIRE_FALSE(TimerManager.IsActive());
}

TEST_CASE("Timer Activation", "[Timer]")
{
    Timer::Manager TimerManager;
    REQUIRE(TimerManager.IsActive());
    REQUIRE(TimerManager.GetNumTimers() == 0U);

    std::latch Latch1 {1U};
    bool Called1 {false};
    auto TimerTester1 = [&Called1, &Latch1] {
        Called1 = true;
        Latch1.count_down();
    };

    TimerManager.SetTimer(std::chrono::milliseconds(5U), TimerTester1);
    REQUIRE(TimerManager.GetNumTimers() == 1U);

    std::latch Latch2 {1U};
    bool Called2 {false};
    auto TimerTester2 = [&Called2, &Latch2] {
        Called2 = true;
        Latch2.count_down();
    };

    TimerManager.SetTimer(std::chrono::milliseconds(10U), TimerTester2);
    REQUIRE(TimerManager.GetNumTimers() == 2U);

    Latch1.wait();
    REQUIRE(Called1);
    REQUIRE(TimerManager.GetNumTimers() == 1U);

    Latch2.wait();
    REQUIRE(Called2);
    REQUIRE(TimerManager.GetNumTimers() == 0U);

    TimerManager.SetActive(false);
    REQUIRE_FALSE(TimerManager.IsActive());
}