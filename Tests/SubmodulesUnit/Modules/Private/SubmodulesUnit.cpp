// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#define BOOST_TEST_MODULE Submodules
#include <boost/json/object.hpp>
#include <boost/test/included/unit_test.hpp>

import <filesystem>;
import <stdexcept>;

import Configuration.Manager;
import Timer.Manager;

BOOST_AUTO_TEST_SUITE(Configuration)

BOOST_AUTO_TEST_CASE(InsertData)
{
    Configuration::SetValue("Signed", 1);
    BOOST_TEST(Configuration::Contains("Signed"));
    BOOST_TEST(Configuration::GetValue("Signed") == 1);

    Configuration::SetValue("Unsigned", 2U);
    BOOST_TEST(Configuration::Contains("Unsigned"));
    BOOST_TEST(Configuration::GetValue("Unsigned") == 2U);

    Configuration::SetValue("Float", 3.F);
    BOOST_TEST(Configuration::Contains("Float"));
    BOOST_TEST(Configuration::GetValue("Float") == 3.F);

    Configuration::SetValue("Double", 4.0);
    BOOST_TEST(Configuration::Contains("Double"));
    BOOST_TEST(Configuration::GetValue("Double") == 4.0);

    Configuration::SetValue("String", std::string("Five"));
    BOOST_TEST(Configuration::Contains("String"));
    BOOST_TEST(Configuration::GetValue("String") == "Five");

    Configuration::SetValue("Null", nullptr);
    BOOST_TEST(!Configuration::Contains("Null"));
    BOOST_CHECK_THROW(Configuration::GetValue("Null") == nullptr, std::exception);
}

BOOST_AUTO_TEST_CASE(RemoveData)
{
    Configuration::SetValue("Testing", 1);
    BOOST_TEST(Configuration::Contains("Testing"));

    Configuration::RemoveValue("Testing");
    BOOST_TEST(!Configuration::Contains("Testing"));
}

BOOST_AUTO_TEST_CASE(DumpConfiguration)
{
    Configuration::SetValue("Signed", 1);
    BOOST_TEST(Configuration::Contains("Signed"));
    BOOST_TEST(Configuration::GetValue("Signed") == 1);

    Configuration::SetValue("Float", 2.F);
    BOOST_TEST(Configuration::Contains("Float"));
    BOOST_TEST(Configuration::GetValue("Float") == 2.F);

    boost::json::array const Value {
            "Item1",
            "Item2",
            "Item3"};

    Configuration::SetValue("Array", Value);
    BOOST_TEST(Configuration::Contains("Array"));
    BOOST_TEST(Configuration::GetValue("Array").as_array() == Value);

    std::string const DumpContent          = Configuration::Dump();
    constexpr std::string_view TestContent = R"({"Signed":1,"Unsigned":2,"Float":2E0,"Double":4E0,"String":"Five","Array":["Item1","Item2","Item3"]})";
    BOOST_TEST(DumpContent == TestContent);
}

BOOST_AUTO_TEST_CASE(SaveAndLoadData)
{
    std::string const FilePath = R"(.\test_data.json)";

    Configuration::SetValue("Value1", 123);
    Configuration::SetValue("Value2", "TestString");
    Configuration::SetValue("Value3", 3.14);

    Configuration::SaveData(FilePath);
    Configuration::LoadData(FilePath);

    BOOST_TEST(Configuration::Contains("Value1"));
    BOOST_TEST(Configuration::Contains("Value2"));
    BOOST_TEST(Configuration::Contains("Value3"));

    BOOST_TEST(Configuration::GetValue("Value1") == 123);
    BOOST_TEST(Configuration::GetValue("Value2") == "TestString");
    BOOST_TEST(Configuration::GetValue("Value3") == 3.14);
}

BOOST_AUTO_TEST_CASE(LoadInvalidData)
{
    std::string const FilePath = R"(.\non_existent_file.json)";
    BOOST_CHECK_THROW(Configuration::LoadData(FilePath), std::filesystem::filesystem_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Timer)

BOOST_AUTO_TEST_CASE(PLaceholder)
{
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()