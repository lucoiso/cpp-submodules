// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "SocketServiceModule.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>

export module SocketService.Service;

import SocketService.IInterface;

namespace SocketService
{
    export class SOCKETSERVICEMODULE_API Service : public IInterface
    {
        std::string   m_Host {};
        std::uint16_t m_Port { 0U };

    public:
        Service(boost::asio::io_context &, std::string_view, std::uint16_t);

        void Connect(boost::function<void(std::string)> const &) override;
    };
} // namespace SocketService
