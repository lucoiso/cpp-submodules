// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <chrono>
#include <boost/log/trivial.hpp>

module RuntimeInfo.ExecutionCounter;

using namespace RuntimeInfo;

ScopedCounter::ScopedCounter(std::string_view const Identifier)
    : m_Identifier(Identifier)
  , m_StartPoint(std::chrono::high_resolution_clock::now())
{
}

ScopedCounter::~ScopedCounter()
{
    BOOST_LOG_TRIVIAL(info) << "[" << m_Identifier << "]:" << std::chrono::duration<
                               double, std::milli>(std::chrono::high_resolution_clock::now() - m_StartPoint);
}
