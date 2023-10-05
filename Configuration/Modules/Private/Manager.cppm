// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>

module Configuration.Manager;

import <filesystem>;
import <fstream>;
import <string_view>;
import <stdexcept>;

using namespace Configuration;

Manager& Manager::Get()
{
    static Manager Instance {};
    return Instance;
}

boost::json::value const& Manager::GetValue(std::string_view const Key) const
{
    return m_Data.at(Key);
}

void Manager::RemoveValue(std::string_view const Key)
{
    m_Data.erase(Key);
}

bool Manager::Contains(std::string_view const Key) const
{
    return m_Data.contains(Key);
}

std::string Manager::Dump() const
{
    return serialize(m_Data);
}

void Manager::SaveData(std::string_view const Path) const
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

    FileStream << Dump();
    FileStream.close();
}

void Manager::LoadData(std::string_view const Path)
{
    std::filesystem::path const Destination(Path);
    if (!exists(Destination))
    {
        throw std::filesystem::filesystem_error("File not found", std::filesystem::path(Path), std::make_error_code(std::errc::no_such_file_or_directory));
    }

    std::ifstream FileStream(Destination);
    if (!FileStream.is_open())
    {
        throw std::filesystem::filesystem_error("Failed to open file", std::filesystem::path(Path), std::make_error_code(std::errc::text_file_busy));
    }

    std::stringstream FileContent;
    FileContent << FileStream.rdbuf();
    FileStream.close();

    for (boost::json::value const JsonContent = boost::json::parse(FileContent);
         auto const& [Key, Value]: JsonContent.get_object())
    {
        m_Data.insert_or_assign(Key, Value);
    }
}