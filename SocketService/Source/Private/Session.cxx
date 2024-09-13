// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>

module SocketService.Session;

using namespace SocketService;

Session::Session(boost::asio::io_context &                     Context,
                 boost::asio::ip::tcp::socket                  Socket, boost::function<void(Session const *)> const &DisconnectCallback)
    : IInterface(Context, std::move(Socket))
  , m_DisconnectCallback(DisconnectCallback)
{
}

void Session::Connect(boost::function<void(std::string)> const &Callback)
{
IInterface::Connect(Callback);
    DoRead();
}

void Session::ReadCallback(boost::system::error_code const &Error, std::size_t const BytesTransferred)
{
IInterface::ReadCallback(Error, BytesTransferred);

    if (Error && Error == boost::asio::error::eof || Error == boost::asio::error::connection_reset)
    {
        m_DisconnectCallback(this);
    }
}
