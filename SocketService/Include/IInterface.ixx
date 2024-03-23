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
        bool m_IsConnected {false};

        std::string m_ReadData {};
        std::string m_WriteData {};

        boost::asio::io_context &m_Context;
        boost::asio::ip::tcp::socket m_Socket;

        boost::function<void(std::string)> m_Callback {};

    public:
        explicit IInterface(boost::asio::io_context &);
        IInterface(boost::asio::io_context &Context, boost::asio::ip::tcp::socket);

        virtual void Connect(const boost::function<void(std::string)> &);
        virtual void Disconnect();

        virtual bool IsConnected() const;

        virtual void Post(std::string_view);

    protected:
        virtual void DoRead();
        virtual void DoClose();

        virtual void ConnectionCallback(const boost::system::error_code &);

        virtual void ReadCallback(const boost::system::error_code &, std::size_t);

        virtual void PostCallback();
        virtual void WriteCallback(const boost::system::error_code &, std::size_t);
    };
} // namespace SocketService
