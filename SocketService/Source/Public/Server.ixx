// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include "SocketServiceModule.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/function.hpp>

export module SocketService.Server;

import SocketService.Service;

namespace SocketService
{
    export class SOCKETSERVICEMODULE_API Server final : public Service
    {
        class Impl;
        std::unique_ptr<Impl> m_Impl;

    public:
        Server(boost::asio::io_context &, std::string_view, std::uint16_t);
        ~Server() override;

        void Connect(boost::function<void(std::string)> const &) override;
        void Disconnect() override;

        void Post(std::string_view) override;

        [[nodiscard]] bool IsConnected() const override;
    };
} // namespace SocketService
