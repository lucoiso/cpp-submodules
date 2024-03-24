// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <filesystem>
#include <fstream>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>

module Configuration.Manager;

using namespace Configuration;

boost::json::value const& Configuration::GetValue(std::string_view const Key)
{
    return g_Data.at(Key);
}

void Configuration::RemoveValue(std::string_view const Key)
{
    g_Data.erase(Key);
}

void Configuration::ClearData()
{
    g_Data.clear();
}

bool Configuration::Contains(std::string_view const Key)
{
    return g_Data.contains(Key);
}

bool Configuration::IsEmpty()
{
    return g_Data.empty();
}

std::string Configuration::Dump()
{
    return serialize(g_Data);
}

bool Configuration::SaveData(std::string_view const Path)
{
    std::filesystem::path const Destination(Path);
    if (!is_directory(Destination.parent_path()))
    {
        throw std::filesystem::filesystem_error("Directory not found", std::filesystem::path(Path), std::make_error_code(std::errc::no_such_file_or_directory));
    }

    std::ofstream FileStream(Destination, std::ios::trunc);
    if (!FileStream.is_open())
    {
        throw std::filesystem::filesystem_error("Failed to open file", std::filesystem::path(Path), std::make_error_code(std::errc::text_file_busy));
    }

    std::string const Content = Dump();
    FileStream << Content;
    FileStream.close();

    return !Content.empty();
}

bool Configuration::LoadData(std::string_view const Path)
{
    std::filesystem::path const Source(Path);
    if (!exists(Source))
    {
        throw std::filesystem::filesystem_error("File not found", std::filesystem::path(Path), std::make_error_code(std::errc::no_such_file_or_directory));
    }

    std::ifstream FileStream(Source);
    if (!FileStream.is_open())
    {
        throw std::filesystem::filesystem_error("Failed to open file", std::filesystem::path(Path), std::make_error_code(std::errc::text_file_busy));
    }

    std::stringstream FileContent;
    FileContent << FileStream.rdbuf();
    FileStream.close();

    boost::json::value const JsonContent = boost::json::parse(FileContent);
    boost::json::object const& JsonObject = JsonContent.get_object();

    for (auto const& [Key, Value] : JsonObject)
    {
        g_Data.insert_or_assign(Key, Value);
    }

    return !JsonObject.empty();
}
