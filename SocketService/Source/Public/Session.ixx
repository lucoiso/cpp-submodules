// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "SocketServiceModule.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>

export module SocketService.Session;

import SocketService.IInterface;

namespace SocketService
{
    export class SOCKETSERVICEMODULE_API Session : public IInterface
    {
        boost::function<void(Session const *)> m_DisconnectCallback {};

    public:
        Session(boost::asio::io_context &, boost::asio::ip::tcp::socket, boost::function<void(Session const *)> const &);

        void Connect(boost::function<void(std::string)> const &) override;

    protected:
        void ReadCallback(boost::system::error_code const &, std::size_t) override;
    };
} // namespace SocketService
