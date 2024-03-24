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
        boost::function<void(const Session *)> m_DisconnectCallback{};

    public:
        Session(boost::asio::io_context&,
                boost::asio::ip::tcp::socket,
                const boost::function<void(const Session *)>&);

        void Connect(const boost::function<void(std::string)>&) override;

    protected:
        void ReadCallback(const boost::system::error_code&,
                          std::size_t) override;
    };
} // namespace SocketService
