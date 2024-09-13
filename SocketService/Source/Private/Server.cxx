// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <queue>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind/bind.hpp>
#include <boost/log/trivial.hpp>

module SocketService.Server;

import SocketService.Session;

using namespace SocketService;

class Server::Impl
{
    bool m_IsConnected {};

    boost::asio::io_context &m_Context;

    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_Acceptor {};
    std::list<std::unique_ptr<Session>>             m_Connections {};

    std::queue<std::string> m_MessagesQueue {};

    boost::function<void(std::string)> m_Callback {};

public:
    Impl(boost::asio::io_context &Context, std::string_view const Host, std::uint16_t const Port)
        : m_Context(Context)
      , m_Acceptor(std::make_unique<boost::asio::ip::tcp::acceptor>(Context,
                                                                    boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(std::data(Host)),
                                                                                                   Port)))
    {
    }

    void Connect(boost::function<void(std::string)> const &Callback)
    {
        m_IsConnected = true;
        m_Callback    = Callback;

        AcceptConnection();
    }

    void Disconnect()
    {
        try
        {
            m_IsConnected = false;
            m_Acceptor->close();
            m_Connections.clear();

            while (!m_MessagesQueue.empty())
            {
                m_MessagesQueue.pop();
            }
        }
        catch (std::exception const &Exception)
        {
            BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: " << " - An error has occurred: " << Exception.what();
        }
    }

    void Post(std::string_view const Data)
    {
        if (m_Connections.empty())
        {
            m_MessagesQueue.emplace(Data);
        }
        else
        {
            for (std::unique_ptr<Session> const &ConnectionIterator : m_Connections)
            {
                ConnectionIterator->Post(Data);
            }
        }
    }

    [[nodiscard]] bool IsConnected() const
    {
        return m_IsConnected;
    }

private:
    void AcceptConnection()
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Accepting connection";

        if (m_IsConnected)
        {
            auto const Callback = std::bind(&Impl::AcceptCallback, this, std::placeholders::_1, std::placeholders::_2);
            m_Acceptor->async_accept(Callback);
        }
    }

    void AcceptCallback(boost::system::error_code const &Error, boost::asio::ip::tcp::socket Socket)
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Accept callback reached";

        if (Error)
        {
            if (Error != boost::asio::error::operation_aborted)
            {
                BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - An error has occurred: " << Error.message() << " (" << Error << ")";
            }

            Disconnect();
        }
        else
        {
            BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Accepting new Session on: " << Socket.remote_endpoint().address() << ":" <<
 Socket.remote_endpoint().port();

            auto const DisconnectCallback = boost::bind(&Impl::OnClientDisconnected, this, boost::placeholders::_1);
            m_Connections.push_back(std::make_unique<Session>(m_Context, std::move(Socket), DisconnectCallback));
            m_Connections.back()->Connect(m_Callback);

            while (!m_MessagesQueue.empty())
            {
                std::string const DequeuedMessage = m_MessagesQueue.front();
                BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Sending pending message to new connection: " << DequeuedMessage;

                m_Connections.back()->Post(std::data(DequeuedMessage));
                m_MessagesQueue.pop();
            }
        }

        AcceptConnection();
    }

    void OnClientDisconnected(Session const *const Session)
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - A client was disconnected.";

        std::_Erase_remove_if(m_Connections,
                              [Session](std::unique_ptr<SocketService::Session> const &ConnectionIterator)
                              {
                                  return ConnectionIterator.get() == Session;
                              });
    }
};

Server::Server(boost::asio::io_context &Context, std::string_view const Host, std::uint16_t const Port)
    : Service(Context, Host, Port)
  , m_Impl(std::make_unique<Impl>(Context, Host, Port))
{
}

Server::~Server() = default;

void Server::Connect(boost::function<void(std::string)> const &Callback)
{
    m_Impl->Connect(Callback);
}

void Server::Disconnect()
{
    m_Impl->Disconnect();
}

void Server::Post(std::string_view const Data)
{
    m_Impl->Post(Data);
}

bool Server::IsConnected() const
{
    return m_Impl->IsConnected();
}
