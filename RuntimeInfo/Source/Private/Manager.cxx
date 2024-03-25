// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <format>
#include <mutex>
#include <regex>
#include <vector>

module RuntimeInfo.Manager;

using namespace RuntimeInfo;

void Manager::InsertCallstack(std::source_location&& Location)
{
    std::lock_guard lock(m_CallstackMutex);

    m_Callstack.insert(std::cbegin(m_Callstack), std::move(Location));

    if (static_cast<std::uint8_t>(std::size(m_Callstack)) >= m_CallstackLimit)
    {
        m_Callstack.pop_back();
    }
}

Manager& Manager::Get()
{
    static Manager Instance;
    return Instance;
}

void Manager::PushCallstack(std::source_location Location)
{
    if (!m_Active)
    {
        return;
    }

    InsertCallstack(std::move(Location));
}

std::string ExtractFunctionName(std::string const& FunctionName)
{
    std::regex const Regex(R"(\b([a-zA-Z_][a-zA-Z0-9_]*)\b(?=\s*\())");

    if (std::smatch Match;
        std::regex_search(FunctionName, Match, Regex))
    {
        return Match.str();
    }

    return {};
}

ScopedCounter Manager::PushCallstackWithCounter(std::source_location Location)
{
    std::string const FunctionName = ExtractFunctionName(Location.function_name());
    InsertCallstack(std::move(Location));
    return ScopedCounter{ FunctionName };
}

bool Manager::IsActive() const
{
    return m_Active;
}

void Manager::SetActive(bool const Value)
{
    m_Active = Value;
}

void Manager::PopCallstack()
{
    std::lock_guard lock(m_CallstackMutex);

    if (!std::empty(m_Callstack))
    {
        m_Callstack.pop_back();
    }
}

void Manager::Reset()
{
    std::lock_guard lock(m_CallstackMutex);

    m_Callstack.clear();
}

void Manager::SetCallstackLimit(std::uint8_t const Limit)
{
    if (Limit == 0U)
    {
        return;
    }

    std::lock_guard lock(m_CallstackMutex);

    if (m_CallstackLimit != Limit)
    {
        m_CallstackLimit = Limit;
        m_Callstack.resize(m_CallstackLimit);
    }
}

std::vector<std::source_location> const& Manager::GetCallstack() const
{
    return m_Callstack;
}

std::uint8_t Manager::GetCallstackLimit() const
{
    return m_CallstackLimit;
}
