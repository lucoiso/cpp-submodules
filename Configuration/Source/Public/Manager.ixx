// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <string>
#include <boost/json/object.hpp>
#include "ConfigurationModule.hpp"

export module Configuration.Manager;

namespace Configuration
{
    export CONFIGURATIONMODULE_API [[nodiscard]] boost::json::object &GetMutableData();

    export CONFIGURATIONMODULE_API [[nodiscard]] boost::json::object const &GetData();

    export CONFIGURATIONMODULE_API [[nodiscard]] boost::json::value const &GetValue(std::string_view);

    export template <typename T>
    CONFIGURATIONMODULE_API constexpr void SetValue(std::string_view const Key, T &&Value)
    {
        if constexpr (std::is_pointer_v<T> || std::is_null_pointer_v<T>)
        {
            if (Value == nullptr)
            {
                return;
            }
        }

        GetMutableData().insert_or_assign(Key, Value);
    }

    export CONFIGURATIONMODULE_API void RemoveValue(std::string_view);

    export CONFIGURATIONMODULE_API void ClearData();

    export CONFIGURATIONMODULE_API [[nodiscard]] bool Contains(std::string_view);

    export CONFIGURATIONMODULE_API [[nodiscard]] bool IsEmpty();

    export CONFIGURATIONMODULE_API [[nodiscard]] std::string Dump();

    export CONFIGURATIONMODULE_API [[nodiscard]] bool SaveData(std::string_view);

    export CONFIGURATIONMODULE_API [[nodiscard]] bool LoadData(std::string_view);
} // namespace Configuration
