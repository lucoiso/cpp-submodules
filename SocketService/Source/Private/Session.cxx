// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>

module SocketService.Session;

using namespace SocketService;

Session::Session(boost::asio::io_context& Context,
                 boost::asio::ip::tcp::socket Socket,
                 const boost::function<void(const Session *)>& DisconnectCallback)
    : IInterface(Context, std::move(Socket))
    , m_DisconnectCallback(DisconnectCallback)
{
}

void Session::Connect(const boost::function<void(std::string)>& Callback)
{
    IInterface::Connect(Callback);
    DoRead();
}

void Session::ReadCallback(const boost::system::error_code& Error,
                           const std::size_t BytesTransferred)
{
    IInterface::ReadCallback(Error, BytesTransferred);

    if (Error && Error == boost::asio::error::eof || Error == boost::asio::error::connection_reset)
    {
        m_DisconnectCallback(this);
    }
}
