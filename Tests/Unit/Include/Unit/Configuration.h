// Copyright Notices: [...]

#pragma once

#include <catch2/catch_test_macros.hpp>
#include <Configuration/Manager.h>
#include <filesystem>

TEST_CASE("Insertion", "[Configuration]")
{
    const auto TestManager = Configuration::Manager::GetInstance();
    REQUIRE(TestManager != nullptr);

    SECTION("Signed")
    {
        TestManager->SetValue("Signed", 1);
        REQUIRE(TestManager->Contains("Signed"));
        REQUIRE(TestManager->GetValue("Signed") == 1);
    }

    SECTION("Unsigned")
    {
        TestManager->SetValue("Unsigned", 2u);
        REQUIRE(TestManager->Contains("Unsigned"));
        REQUIRE(TestManager->GetValue("Unsigned") == 2u);
    }

    SECTION("Float")
    {
        TestManager->SetValue("Float", 3.f);
        REQUIRE(TestManager->Contains("Float"));
        REQUIRE(TestManager->GetValue("Float") == 3.f);
    }

    SECTION("Double")
    {
        TestManager->SetValue("Double", 4.0);
        REQUIRE(TestManager->Contains("Double"));
        REQUIRE(TestManager->GetValue("Double") == 4.0);
    }

    SECTION("String")
    {
        TestManager->SetValue("String", std::string("Five"));
        REQUIRE(TestManager->Contains("String"));
        REQUIRE(TestManager->GetValue("String") == "Five");
    }

    SECTION("Null")
    {
        TestManager->SetValue("Null", nullptr);
        REQUIRE_FALSE(TestManager->Contains("Null"));
        REQUIRE_THROWS_AS(TestManager->GetValue("Null") == nullptr, std::exception);
    }
}

TEST_CASE("Removal", "[Configuration]")
{
    const auto TestManager = Configuration::Manager::GetInstance();
    REQUIRE(TestManager != nullptr);

    SECTION("Insert")
    {
        TestManager->SetValue("Testing", 1);
        REQUIRE(TestManager->Contains("Testing"));
    }

    SECTION("Remove")
    {
        TestManager->RemoveValue("Testing");
        REQUIRE_FALSE(TestManager->Contains("Testing"));
    }
}

TEST_CASE("Dump", "[Configuration]")
{
    const auto TestManager = Configuration::Manager::GetInstance();
    REQUIRE(TestManager != nullptr);

    SECTION("Insert Signed")
    {
        TestManager->SetValue("Signed", 1);
        REQUIRE(TestManager->Contains("Signed"));
        REQUIRE(TestManager->GetValue("Signed") == 1);
    }

    SECTION("Insert Float")
    {
        TestManager->SetValue("Float", 2.f);
        REQUIRE(TestManager->Contains("Float"));
        REQUIRE(TestManager->GetValue("Float") == 2.f);
    }

    SECTION("Insert Array")
    {
        const boost::json::array Value { "Item1", "Item2", "Item3" };
        TestManager->SetValue("Array", Value);
        REQUIRE(TestManager->Contains("Array"));
        REQUIRE(TestManager->GetValue("Array").as_array() == Value);
    }

    SECTION("Check Dump Content")
    {
        const std::string DumpContent = TestManager->Dump();
        constexpr std::string_view TestContentRequirement = R"({"Signed":1,"Unsigned":2,"Float":2E0,"Double":4E0,"String":"Five","Array":["Item1","Item2","Item3"]})";
        REQUIRE(DumpContent == TestContentRequirement);
    }
}

TEST_CASE("Save and Load Data", "[Configuration]")
{
    SECTION("Save and Load Valid Data")
    {
        auto TestManager = Configuration::Manager::GetInstance();
        REQUIRE(TestManager != nullptr);

        TestManager->SetValue("Value1", 123);
        TestManager->SetValue("Value2", "TestString");
        TestManager->SetValue("Value3", 3.14);

        const std::string FilePath = ".\\test_data.json";
        TestManager->SaveData(FilePath);

        TestManager.reset();
        REQUIRE(TestManager == nullptr);

        const auto NewManager = Configuration::Manager::GetInstance();
        REQUIRE(NewManager != nullptr);

        NewManager->LoadData(FilePath);

        REQUIRE(NewManager->Contains("Value1"));
        REQUIRE(NewManager->Contains("Value2"));
        REQUIRE(NewManager->Contains("Value3"));

        REQUIRE(NewManager->GetValue("Value1") == 123);
        REQUIRE(NewManager->GetValue("Value2") == "TestString");
        REQUIRE(NewManager->GetValue("Value3") == 3.14);
    }

    SECTION("Load Invalid File")
    {
        const auto TestManager = Configuration::Manager::GetInstance();
        REQUIRE(TestManager != nullptr);

        const std::string FilePath = ".\\non_existent_file.json";
        REQUIRE_THROWS_AS(TestManager->LoadData(FilePath), std::filesystem::filesystem_error);
    }
}
