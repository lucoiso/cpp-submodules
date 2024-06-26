// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <boost/asio/ip/tcp.hpp>

module SocketService.Client;

using namespace SocketService;

class Client::Impl
{
    boost::asio::io_context &m_Context;
    std::shared_ptr<Service> m_Service {};

    std::string   m_Host {};
    std::uint16_t m_Port { 0U };

public:
    Impl(boost::asio::io_context &Context, const std::string_view Host, const std::uint16_t Port)
        : m_Context(Context)
      , m_Host(Host)
      , m_Port(Port)
    {
    }

    void Connect(const boost::function<void(std::string)> &Callback)
    {
        m_Service = std::make_unique<Service>(m_Context, std::data(m_Host), m_Port);
        m_Service->Connect(Callback);
    }

    void Disconnect() const
    {
        m_Service->Disconnect();
    }

    void Post(const std::string_view Data) const
    {
        m_Service->Post(Data);
    }
};

Client::Client(boost::asio::io_context &Context, const std::string_view Host, const std::uint16_t Port)
    : Service(Context, Host, Port)
  , m_Impl(std::make_unique<Impl>(Context, Host, Port))
{
}

Client::~Client() = default;

void Client::Connect(const boost::function<void(std::string)> &Callback)
{
    m_Impl->Connect(Callback);
}

void Client::Disconnect()
{
    m_Impl->Disconnect();
}

void Client::Post(const std::string_view Data)
{
    m_Impl->Post(Data);
}
