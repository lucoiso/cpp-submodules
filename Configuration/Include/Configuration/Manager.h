// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include "ConfigurationModule.h"
#include <boost/json/value.hpp>
#include <boost/json/value_from.hpp>

template <typename T>concept SerializableToJson = requires(const T& Value) { { boost::json::value_from(Value) }; };

namespace Configuration
{
    class CONFIGURATIONMODULE_API Manager
    {
    public:
        Manager();
        ~Manager();

        [[nodiscard]] static Manager& Get();

        [[nodiscard]] const boost::json::value& GetValue(std::string_view Key) const;

        template <typename T>
            requires SerializableToJson<T>
        constexpr void SetValue(const std::string_view Key, const T& Value)
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

        bool Contains(std::string_view Key) const;

        [[nodiscard]] boost::json::value GetData() const;
        [[nodiscard]] std::string        Dump() const;

        void SaveData(std::string_view Path) const;
        void LoadData(std::string_view Path);

    private:
        static Manager      g_Instance;
        boost::json::object m_Data;
    };
}
