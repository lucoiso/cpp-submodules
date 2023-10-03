// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#define BOOST_TEST_MODULE CONFIGURATION_TEST_MODULE
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>
#include <boost/test/included/unit_test.hpp>

export module SubmodulesUnitConfig;

import <filesystem>;
import <stdexcept>;

import ConfigurationManager;

BOOST_AUTO_TEST_CASE(InsertData)
{
    Configuration::Manager::Get().SetValue("Signed", 1);
    BOOST_TEST(Configuration::Manager::Get().Contains("Signed"));
    BOOST_TEST(Configuration::Manager::Get().GetValue("Signed") == 1);

    Configuration::Manager::Get().SetValue("Unsigned", 2U);
    BOOST_TEST(Configuration::Manager::Get().Contains("Unsigned"));
    BOOST_TEST(Configuration::Manager::Get().GetValue("Unsigned") == 2U);

    Configuration::Manager::Get().SetValue("Float", 3.F);
    BOOST_TEST(Configuration::Manager::Get().Contains("Float"));
    BOOST_TEST(Configuration::Manager::Get().GetValue("Float") == 3.F);

    Configuration::Manager::Get().SetValue("Double", 4.0);
    BOOST_TEST(Configuration::Manager::Get().Contains("Double"));
    BOOST_TEST(Configuration::Manager::Get().GetValue("Double") == 4.0);

    Configuration::Manager::Get().SetValue("String", std::string("Five"));
    BOOST_TEST(Configuration::Manager::Get().Contains("String"));
    BOOST_TEST(Configuration::Manager::Get().GetValue("String") == "Five");

    Configuration::Manager::Get().SetValue("Null", nullptr);
    BOOST_TEST(!Configuration::Manager::Get().Contains("Null"));
    BOOST_CHECK_THROW(Configuration::Manager::Get().GetValue("Null") == nullptr, std::exception);
}

BOOST_AUTO_TEST_CASE(RemoveData)
{
    Configuration::Manager::Get().SetValue("Testing", 1);
    BOOST_TEST(Configuration::Manager::Get().Contains("Testing"));

    Configuration::Manager::Get().RemoveValue("Testing");
    BOOST_TEST(!Configuration::Manager::Get().Contains("Testing"));
}

BOOST_AUTO_TEST_CASE(DumpConfiguration)
{
    Configuration::Manager::Get().SetValue("Signed", 1);
    BOOST_TEST(Configuration::Manager::Get().Contains("Signed"));
    BOOST_TEST(Configuration::Manager::Get().GetValue("Signed") == 1);

    Configuration::Manager::Get().SetValue("Float", 2.F);
    BOOST_TEST(Configuration::Manager::Get().Contains("Float"));
    BOOST_TEST(Configuration::Manager::Get().GetValue("Float") == 2.F);

    boost::json::array const Value {
            "Item1",
            "Item2",
            "Item3"};

    Configuration::Manager::Get().SetValue("Array", Value);
    BOOST_TEST(Configuration::Manager::Get().Contains("Array"));
    BOOST_TEST(Configuration::Manager::Get().GetValue("Array").as_array() == Value);

    std::string const DumpContent          = Configuration::Manager::Get().Dump();
    constexpr std::string_view TestContent = R"({"Signed":1,"Unsigned":2,"Float":2E0,"Double":4E0,"String":"Five","Array":["Item1","Item2","Item3"]})";
    BOOST_TEST(DumpContent == TestContent);
}

BOOST_AUTO_TEST_CASE(SaveAndLoadData)
{
    std::string const FilePath = R"(.\test_data.json)";

    Configuration::Manager::Get().SetValue("Value1", 123);
    Configuration::Manager::Get().SetValue("Value2", "TestString");
    Configuration::Manager::Get().SetValue("Value3", 3.14);

    Configuration::Manager::Get().SaveData(FilePath);
    Configuration::Manager::Get().LoadData(FilePath);

    BOOST_TEST(Configuration::Manager::Get().Contains("Value1"));
    BOOST_TEST(Configuration::Manager::Get().Contains("Value2"));
    BOOST_TEST(Configuration::Manager::Get().Contains("Value3"));

    BOOST_TEST(Configuration::Manager::Get().GetValue("Value1") == 123);
    BOOST_TEST(Configuration::Manager::Get().GetValue("Value2") == "TestString");
    BOOST_TEST(Configuration::Manager::Get().GetValue("Value3") == 3.14);
}

BOOST_AUTO_TEST_CASE(LoadInvalidData)
{
    std::string const FilePath = R"(.\non_existent_file.json)";
    BOOST_CHECK_THROW(Configuration::Manager::Get().LoadData(FilePath), std::filesystem::filesystem_error);
}

#undef BOOST_TEST_MODULE
