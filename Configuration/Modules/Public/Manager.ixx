// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "ConfigurationModule.h"
#include <boost/json/object.hpp>

export module ConfigurationManager;

import <string_view>;

export namespace Configuration
{
    class CONFIGURATIONMODULE_API Manager
    {
        boost::json::object m_Data;

    public:
        static Manager& Get();

        [[nodiscard]] boost::json::value const& GetValue(std::string_view Key) const;

        template<typename T>
        constexpr void SetValue(std::string_view const Key, T&& Value)
        {
            if constexpr (std::is_pointer_v<T> || std::is_null_pointer_v<T>)
            {
                if (Value == nullptr)
                {
                    return;
                }
            }

            m_Data.insert_or_assign(Key, Value);
        }

        void RemoveValue(std::string_view Key);

        [[nodiscard]] bool Contains(std::string_view Key) const;
        [[nodiscard]] std::string Dump() const;

        void SaveData(std::string_view Path) const;
        void LoadData(std::string_view Path);
    };
}// namespace Configuration