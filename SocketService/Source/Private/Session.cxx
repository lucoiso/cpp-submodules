// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>
#include <easy/profiler.h>

module SocketService.Session;

using namespace SocketService;

Session::Session(boost::asio::io_context &                     Context,
                 boost::asio::ip::tcp::socket                  Socket,
                 const boost::function<void(const Session *)> &DisconnectCallback)
    : IInterface(Context, std::move(Socket))
  , m_DisconnectCallback(DisconnectCallback)
{
}

void Session::Connect(const boost::function<void(std::string)> &Callback)
{
    EASY_FUNCTION(profiler::colors::Green);

    IInterface::Connect(Callback);
    DoRead();
}

void Session::ReadCallback(const boost::system::error_code &Error, const std::size_t BytesTransferred)
{
    EASY_FUNCTION(profiler::colors::Green);

    IInterface::ReadCallback(Error, BytesTransferred);

    if (Error && Error == boost::asio::error::eof || Error == boost::asio::error::connection_reset)
    {
        m_DisconnectCallback(this);
    }
}
