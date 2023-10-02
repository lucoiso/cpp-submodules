// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "ConfigurationModule.h"
#include <boost/json/object.hpp>
#include <filesystem>
#include <fstream>
#include <string_view>

export module Configuration.Manager;

namespace Configuration
{
    export class CONFIGURATIONMODULE_API Manager
    {
        boost::json::object m_Data;

    public:
        static Manager& Get();

        [[nodiscard]] boost::json::value const& GetValue(std::string_view Key) const;
        void RemoveValue(std::string_view Key);

        [[nodiscard]] bool Contains(std::string_view Key) const;
        [[nodiscard]] std::string Dump() const;

        void SaveData(std::string_view Path) const;
        void LoadData(std::string_view Path);
    };
}// namespace Configuration