// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#pragma once

#include "ConfigurationModule.h"
#include <memory>
#include <string_view>
#include <boost/json/value.hpp>
#include <boost/json/value_from.hpp>
#include <boost/json/array.hpp>
#include <concepts>

template <typename T>
concept SerializableToJson = requires(const T &value) { { boost::json::value_from(value) }; };

namespace Configuration
{
    class CONFIGURATIONMODULE_API Manager
    {
    public:
        Manager();
        ~Manager();

        [[nodiscard]] static std::shared_ptr<Manager> GetInstance();

        [[nodiscard]] const boost::json::value &GetValue(const std::string_view Key) const;

        template <typename T>
            requires SerializableToJson<T>
        constexpr inline void SetValue(const std::string_view Key, const T &Value)
        {
            if constexpr (std::is_pointer<T>::value || std::is_null_pointer<T>::value)
            {
                if (Value == nullptr)
                {
                    return;
                }
            }

            m_Data.insert_or_assign(Key, Value);
        }

        void RemoveValue(const std::string_view Key);

        bool Contains(const std::string_view Key) const;

        [[nodiscard]] boost::json::value GetData() const;
        [[nodiscard]] std::string Dump() const;

        void SaveData(const std::string_view Path) const;
        void LoadData(const std::string_view Path);

    private:
        static std::shared_ptr<Manager> m_Instance;
        boost::json::object m_Data;
    };
}
