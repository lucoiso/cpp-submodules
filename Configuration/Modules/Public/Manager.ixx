// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <boost/json/object.hpp>

export module Configuration.Manager;

export import <string_view>;

namespace Configuration
{
    boost::json::object g_Data;

    export [[nodiscard]] boost::json::value const& GetValue(std::string_view const&);

    export template<typename T>
    constexpr void SetValue(std::string_view const Key, T&& Value)
    {
        if constexpr (std::is_pointer_v<T> || std::is_null_pointer_v<T>)
        {
            if (Value == nullptr)
            {
                return;
            }
        }

        g_Data.insert_or_assign(Key, Value);
    }

    export void RemoveValue(std::string_view const&);
    export void ClearData();

    export [[nodiscard]] bool Contains(std::string_view const&);
    export [[nodiscard]] bool IsEmpty();

    export [[nodiscard]] std::string const Dump();

    export [[nodiscard]] bool SaveData(std::string_view const&);
    export [[nodiscard]] bool LoadData(std::string_view const&);
}// namespace Configuration