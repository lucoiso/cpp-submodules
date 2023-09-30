// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#include "Manager.h"
#include <filesystem>
#include <fstream>
#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>

using namespace Configuration;

Manager Manager::g_Instance;

Manager::Manager()  = default;
Manager::~Manager() = default;

Manager& Manager::Get()
{
    return g_Instance;
}

const boost::json::value& Manager::GetValue(const std::string_view Key) const
{
    return m_Data.at(Key);
}

void Manager::RemoveValue(const std::string_view Key)
{
    m_Data.erase(Key);
}

bool Manager::Contains(const std::string_view Key) const
{
    return m_Data.contains(Key);
}

std::string Manager::Dump() const
{
    return serialize(m_Data);
}

void Manager::SaveData(const std::string_view Path) const
{
    const std::filesystem::path Destination(Path);
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

void Manager::LoadData(const std::string_view Path)
{
    const std::filesystem::path Destination(Path);
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

    const boost::json::value JsonContent = boost::json::parse(FileContent);
    for (const auto& [Key, Value] : JsonContent.get_object())
    {
        m_Data.insert_or_assign(Key, Value);
    }
}
