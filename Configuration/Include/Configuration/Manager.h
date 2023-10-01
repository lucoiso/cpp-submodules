// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include "ConfigurationModule.h"
#include <boost/json/value.hpp>
#include <boost/json/value_from.hpp>

template<typename T>
concept SerializableToJson = requires(T const& Value) { { boost::json::value_from(Value) }; };

namespace Configuration
{
    class CONFIGURATIONMODULE_API Manager
    {
    public:
        Manager() = default;

        Manager(Manager const& Other)            = delete;
        Manager& operator=(Manager const& Other) = delete;

        ~Manager() = default;

        [[nodiscard]] static Manager& Get();

        [[nodiscard]] boost::json::value const& GetValue(std::string_view Key) const;

        template<typename T>
            requires SerializableToJson<T>
        constexpr void SetValue(std::string_view const Key, T const& Value)
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

    private:
        boost::json::object m_Data;
    };
}// namespace Configuration
