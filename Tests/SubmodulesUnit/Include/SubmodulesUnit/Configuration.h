// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#define BOOST_TEST_MODULE CONFIGURATION_TEST_MODULE
#include <boost/test/included/unit_test.hpp>

#include <Configuration/Manager.h>
#include <filesystem>

BOOST_AUTO_TEST_CASE(InsertData)
{
    const auto TestManager = Configuration::Manager::GetInstance();
    BOOST_TEST(TestManager != nullptr);

    TestManager->SetValue("Signed", 1);
    BOOST_TEST(TestManager->Contains("Signed"));
    BOOST_TEST(TestManager->GetValue("Signed") == 1);

    TestManager->SetValue("Unsigned", 2u);
    BOOST_TEST(TestManager->Contains("Unsigned"));
    BOOST_TEST(TestManager->GetValue("Unsigned") == 2u);

    TestManager->SetValue("Float", 3.f);
    BOOST_TEST(TestManager->Contains("Float"));
    BOOST_TEST(TestManager->GetValue("Float") == 3.f);

    TestManager->SetValue("Double", 4.0);
    BOOST_TEST(TestManager->Contains("Double"));
    BOOST_TEST(TestManager->GetValue("Double") == 4.0);

    TestManager->SetValue("String", std::string("Five"));
    BOOST_TEST(TestManager->Contains("String"));
    BOOST_TEST(TestManager->GetValue("String") == "Five");

    TestManager->SetValue("Null", nullptr);
    BOOST_TEST(!TestManager->Contains("Null"));
    BOOST_CHECK_THROW(TestManager->GetValue("Null") == nullptr, std::exception);
}

BOOST_AUTO_TEST_CASE(RemoveData)
{
    const auto TestManager = Configuration::Manager::GetInstance();
    BOOST_TEST(TestManager != nullptr);

    TestManager->SetValue("Testing", 1);
    BOOST_TEST(TestManager->Contains("Testing"));

    TestManager->RemoveValue("Testing");
    BOOST_TEST(!TestManager->Contains("Testing"));
}

BOOST_AUTO_TEST_CASE(DumpConfiguration)
{
    const auto TestManager = Configuration::Manager::GetInstance();
    BOOST_TEST(TestManager != nullptr);

    TestManager->SetValue("Signed", 1);
    BOOST_TEST(TestManager->Contains("Signed"));
    BOOST_TEST(TestManager->GetValue("Signed") == 1);

    TestManager->SetValue("Float", 2.f);
    BOOST_TEST(TestManager->Contains("Float"));
    BOOST_TEST(TestManager->GetValue("Float") == 2.f);

    const boost::json::array Value{"Item1", "Item2", "Item3"};
    TestManager->SetValue("Array", Value);
    BOOST_TEST(TestManager->Contains("Array"));
    BOOST_TEST(TestManager->GetValue("Array").as_array() == Value);

    const std::string DumpContent = TestManager->Dump();
    constexpr std::string_view TestContent = R"({"Signed":1,"Unsigned":2,"Float":2E0,"Double":4E0,"String":"Five","Array":["Item1","Item2","Item3"]})";
    BOOST_TEST(DumpContent == TestContent);
}

BOOST_AUTO_TEST_CASE(SaveAndLoadData)
{
    auto TestManager = Configuration::Manager::GetInstance();
    BOOST_TEST(TestManager != nullptr);

    TestManager->SetValue("Value1", 123);
    TestManager->SetValue("Value2", "TestString");
    TestManager->SetValue("Value3", 3.14);

    const std::string FilePath = R"(.\test_data.json)";
    TestManager->SaveData(FilePath);

    TestManager.reset();
    BOOST_TEST(TestManager == nullptr);

    const auto NewManager = Configuration::Manager::GetInstance();
    BOOST_TEST(NewManager != nullptr);

    NewManager->LoadData(FilePath);

    BOOST_TEST(NewManager->Contains("Value1"));
    BOOST_TEST(NewManager->Contains("Value2"));
    BOOST_TEST(NewManager->Contains("Value3"));

    BOOST_TEST(NewManager->GetValue("Value1") == 123);
    BOOST_TEST(NewManager->GetValue("Value2") == "TestString");
    BOOST_TEST(NewManager->GetValue("Value3") == 3.14);
}

BOOST_AUTO_TEST_CASE(LoadInvalidData)
{
    const auto TestManager = Configuration::Manager::GetInstance();
    BOOST_TEST(TestManager != nullptr);

    const std::string FilePath = R"(.\non_existent_file.json)";
    BOOST_CHECK_THROW(TestManager->LoadData(FilePath), std::filesystem::filesystem_error);
}
#undef BOOST_TEST_MODULE