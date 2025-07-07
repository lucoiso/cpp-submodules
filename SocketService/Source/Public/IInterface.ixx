// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "SocketServiceModule.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>
#include <string>

export module SocketService.IInterface;

namespace SocketService
{
    export class SOCKETSERVICEMODULE_API IInterface
    {
    protected:
        bool m_IsConnected = false;

        std::string m_ReadData {};
        std::string m_WriteData {};

        boost::asio::io_context &    m_Context;
        boost::asio::ip::tcp::socket m_Socket;

        boost::function<void(std::string)> m_Callback {};

    public:
        virtual  ~IInterface() = default;
        explicit IInterface(boost::asio::io_context &);
        IInterface(boost::asio::io_context &Context, boost::asio::ip::tcp::socket);

        virtual void Connect(boost::function<void(std::string)> const &);
        virtual void Disconnect();

        virtual bool IsConnected() const;

        virtual void Post(std::string_view);

    protected:
        virtual void DoRead();
        virtual void DoClose();

        virtual void ConnectionCallback(boost::system::error_code const &);

        virtual void ReadCallback(boost::system::error_code const &, std::size_t);

        virtual void PostCallback();
        virtual void WriteCallback(boost::system::error_code const &, std::size_t);
    };
} // namespace SocketService
