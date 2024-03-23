// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include <filesystem>
#include <boost/json/object.hpp>
#include <catch2/catch_test_macros.hpp>

import Configuration.Manager;

TEST_CASE("InsertData", "[Configuration]")
{
    Configuration::ClearData();
    REQUIRE(Configuration::IsEmpty());

    Configuration::SetValue("Signed", 1);
    REQUIRE(Configuration::Contains("Signed"));
    REQUIRE(Configuration::GetValue("Signed") == 1);

    Configuration::SetValue("Unsigned", 2U);
    REQUIRE(Configuration::Contains("Unsigned"));
    REQUIRE(Configuration::GetValue("Unsigned") == 2U);

    Configuration::SetValue("Float", 3.F);
    REQUIRE(Configuration::Contains("Float"));
    REQUIRE(Configuration::GetValue("Float") == 3.F);

    Configuration::SetValue("Double", 4.0);
    REQUIRE(Configuration::Contains("Double"));
    REQUIRE(Configuration::GetValue("Double") == 4.0);

    Configuration::SetValue("String", std::string("Five"));
    REQUIRE(Configuration::Contains("String"));
    REQUIRE(Configuration::GetValue("String") == "Five");

    Configuration::SetValue("Null", nullptr);
    REQUIRE(!Configuration::Contains("Null"));
    REQUIRE_THROWS_AS(Configuration::GetValue("Null") == nullptr, std::exception);
}

TEST_CASE("RemoveData", "[Configuration]")
{
    Configuration::ClearData();
    REQUIRE(Configuration::IsEmpty());

    Configuration::SetValue("Testing", 1);
    REQUIRE(Configuration::Contains("Testing"));

    Configuration::RemoveValue("Testing");
    REQUIRE(!Configuration::Contains("Testing"));
}

TEST_CASE("DumpContent", "[Configuration]")
{
    Configuration::ClearData();
    REQUIRE(Configuration::IsEmpty());

    Configuration::SetValue("Signed", 1);
    REQUIRE(Configuration::Contains("Signed"));
    REQUIRE(Configuration::GetValue("Signed") == 1);

    Configuration::SetValue("Float", 2.F);
    REQUIRE(Configuration::Contains("Float"));
    REQUIRE(Configuration::GetValue("Float") == 2.F);

    boost::json::array const Value {
            "Item1",
            "Item2",
            "Item3"};

    Configuration::SetValue("Array", Value);
    REQUIRE(Configuration::Contains("Array"));
    REQUIRE(Configuration::GetValue("Array").as_array() == Value);

    std::string const DumpContent = Configuration::Dump();
    std::string const TestContent = R"({"Signed":1,"Float":2E0,"Array":["Item1","Item2","Item3"]})";
    REQUIRE(DumpContent == TestContent);
}

TEST_CASE("SaveAndLoadData", "[Configuration]")
{
    Configuration::ClearData();
    REQUIRE(Configuration::IsEmpty());

    std::string const FilePath = R"(.\test_data.json)";

    Configuration::SetValue("Value1", 123);
    Configuration::SetValue("Value2", "TestString");
    Configuration::SetValue("Value3", 3.14);

    REQUIRE(Configuration::SaveData(FilePath));
    REQUIRE(Configuration::LoadData(FilePath));

    REQUIRE(Configuration::Contains("Value1"));
    REQUIRE(Configuration::Contains("Value2"));
    REQUIRE(Configuration::Contains("Value3"));

    REQUIRE(Configuration::GetValue("Value1") == 123);
    REQUIRE(Configuration::GetValue("Value2") == "TestString");
    REQUIRE(Configuration::GetValue("Value3") == 3.14);
}

TEST_CASE("LoadInvalidData", "[Configuration]")
{
    Configuration::ClearData();
    REQUIRE(Configuration::IsEmpty());

    std::string const FilePath = R"(.\non_existent_file.json)";
    REQUIRE_THROWS_AS(Configuration::LoadData(FilePath), std::filesystem::filesystem_error);
}