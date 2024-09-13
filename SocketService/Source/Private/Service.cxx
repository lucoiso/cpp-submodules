// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind/bind.hpp>
#include <boost/log/trivial.hpp>

module SocketService.Service;

using namespace SocketService;

Service::Service(boost::asio::io_context &Context, std::string_view const Host, std::uint16_t const Port) : IInterface(Context), m_Host(Host), m_Port(Port)
{
}

void Service::Connect(boost::function<void(std::string)> const &Callback)
{
    IInterface::Connect(Callback);

    boost::asio::ip::tcp::resolver                     Resolver(m_Context);
    boost::asio::ip::tcp::resolver::results_type const Results = Resolver.resolve(m_Host, std::to_string(m_Port));

    auto const AsyncCallback = boost::bind(&Service::ConnectionCallback, this, boost::placeholders::_1);
    boost::asio::async_connect(m_Socket, Results, AsyncCallback);
}
