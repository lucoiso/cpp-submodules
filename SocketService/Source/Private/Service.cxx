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

Service::Service(boost::asio::io_context &Context, const std::string_view Host, const std::uint16_t Port) : IInterface(Context), m_Host(Host), m_Port(Port)
{
}

void Service::Connect(const boost::function<void(std::string)> &Callback)
{
    IInterface::Connect(Callback);

    boost::asio::ip::tcp::resolver                     Resolver(m_Context);
    const boost::asio::ip::tcp::resolver::results_type Results = Resolver.resolve(m_Host, std::to_string(m_Port));

    const auto AsyncCallback = boost::bind(&Service::ConnectionCallback, this, boost::placeholders::_1);
    boost::asio::async_connect(m_Socket, Results, AsyncCallback);
}
