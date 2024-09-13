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
    Impl(boost::asio::io_context &Context, std::string_view const Host, std::uint16_t const Port)
        : m_Context(Context)
      , m_Host(Host)
      , m_Port(Port)
    {
    }

    void Connect(boost::function<void(std::string)> const &Callback)
    {
        m_Service = std::make_unique<Service>(m_Context, std::data(m_Host), m_Port);
        m_Service->Connect(Callback);
    }

    void Disconnect() const
    {
        m_Service->Disconnect();
    }

    void Post(std::string_view const Data) const
    {
        m_Service->Post(Data);
    }
};

Client::Client(boost::asio::io_context &Context, std::string_view const Host, std::uint16_t const Port)
    : Service(Context, Host, Port)
  , m_Impl(std::make_unique<Impl>(Context, Host, Port))
{
}

Client::~Client() = default;

void Client::Connect(boost::function<void(std::string)> const &Callback)
{
    m_Impl->Connect(Callback);
}

void Client::Disconnect()
{
    m_Impl->Disconnect();
}

void Client::Post(std::string_view const Data)
{
    m_Impl->Post(Data);
}
