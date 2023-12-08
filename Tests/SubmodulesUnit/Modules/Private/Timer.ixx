// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <catch2/catch_test_macros.hpp>

export module Submodules.Unit.Timer;

import <thread>;
import <latch>;
import <chrono>;
import <memory>;

import Timer.Manager;

TEST_CASE("Manager Status", "[Timer]")
{
    auto TimerManager {std::make_unique<Timer::Manager>()};
    REQUIRE(TimerManager != nullptr);
    REQUIRE(TimerManager->IsActive());
    REQUIRE(TimerManager->GetNumTimers() == 0U);

    TimerManager->SetTimer(std::chrono::milliseconds(1U), [] {
    });
    REQUIRE(TimerManager->GetNumTimers() == 1U);
    TimerManager->ClearTimers();
    REQUIRE(TimerManager->GetNumTimers() == 0U);

    TimerManager->SetActive(false);
    REQUIRE_FALSE(TimerManager->IsActive());
}

TEST_CASE("Timer Activation", "[Timer]")
{
    auto TimerManager {std::make_unique<Timer::Manager>()};
    REQUIRE(TimerManager != nullptr);
    REQUIRE(TimerManager->IsActive());
    REQUIRE(TimerManager->GetNumTimers() == 0U);

    std::latch Latch1 {1U};
    bool Called1 {false};
    auto TimerTester1 = [&Called1, &Latch1] {
        Called1 = true;
        Latch1.count_down();
    };

    TimerManager->SetTimer(std::chrono::milliseconds(1U), TimerTester1);
    REQUIRE(TimerManager->GetNumTimers() == 1U);

    std::latch Latch2 {1U};
    bool Called2 {false};
    auto TimerTester2 = [&Called2, &Latch2] {
        Called2 = true;
        Latch2.count_down();
    };

    TimerManager->SetTimer(std::chrono::milliseconds(2U), TimerTester2);
    REQUIRE(TimerManager->GetNumTimers() == 2U);

    Latch1.wait();
    REQUIRE(Called1);
    REQUIRE(TimerManager->GetNumTimers() == 1U);

    Latch2.wait();
    REQUIRE(Called2);
    REQUIRE(TimerManager->GetNumTimers() == 0U);

    TimerManager->SetActive(false);
    REQUIRE_FALSE(TimerManager->IsActive());
}