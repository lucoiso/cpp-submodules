// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

module;

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind/bind.hpp>
#include <boost/log/trivial.hpp>

module SocketService.IInterface;

using namespace SocketService;

IInterface::IInterface(boost::asio::io_context &Context)
    : m_Context(Context)
  , m_Socket(Context)
{
}

IInterface::IInterface(boost::asio::io_context &Context, boost::asio::ip::tcp::socket Socket)
    : m_Context(Context)
  , m_Socket(std::move(Socket))
{
}

void IInterface::DoRead()
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Read requested";
    auto Callback = boost::bind(&IInterface::ReadCallback, this, boost::placeholders::_1, boost::placeholders::_2);
    boost::asio::async_read_until(m_Socket, boost::asio::dynamic_buffer(m_ReadData), '\n', Callback);
}

void IInterface::Connect(boost::function<void(std::string)> const &Callback)
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Connect requested";

    m_Callback = Callback;
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Starting Connection";
}

void IInterface::Disconnect()
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Disconnect requested";

    m_IsConnected = false;
    boost::asio::post(m_Context, boost::bind(&IInterface::DoClose, this));
}

bool IInterface::IsConnected() const
{
    return m_IsConnected;
}

void IInterface::Post(std::string_view const Data)
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Post requested with data: " << Data;

    m_WriteData = std::format("{}\n", Data);
    boost::asio::post(m_Context, boost::bind(&IInterface::PostCallback, this));
}

void IInterface::DoClose()
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Close requested";

    m_IsConnected = false;

    if (m_Socket.is_open())
    {
        boost::system::error_code Error;
        [[maybe_unused]] auto     Discard = m_Socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, Error);
        m_Socket.close();
    }
}

void IInterface::ConnectionCallback(boost::system::error_code const &Error)
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Connection callback reached";

    if (!Error)
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Connection established";

        m_IsConnected = true;
        DoRead();
    }
    else
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Connection Failed";

        DoClose();
    }
}

void IInterface::ReadCallback(boost::system::error_code const &Error, [[maybe_unused]] std::size_t BytesTransferred)
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Read callback reached";

    if (Error && Error != boost::asio::error::eof && Error != boost::asio::error::operation_aborted && Error.category() !=
        boost::asio::error::system_category)
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - An error has occurred: " << Error.message() << " (" << Error << ")";
    }
    else
    {
        if (!m_ReadData.empty())
        {
            std::_Erase_remove(m_ReadData, '\n');
            m_Callback(m_ReadData);

            BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Received Data: '" << m_ReadData << "'";
            m_ReadData.clear();
        }

        if (!Error)
        {
            DoRead();
        }
        else
        {
            DoClose();
        }
    }
}

void IInterface::PostCallback()
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Post callback reached";

    auto const Callback = boost::bind(&IInterface::WriteCallback, this, boost::placeholders::_1, boost::placeholders::_2);
    boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteData.data(), m_WriteData.length()), Callback);
}

void IInterface::WriteCallback(boost::system::error_code const &Error, std::size_t BytesTransferred)
{
    BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Write callback reached";

    if (Error && Error != boost::asio::error::eof)
    {
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - An error has occurred: " << Error.message() << " (" << Error << ")";
    }
    else
    {
        std::erase(m_WriteData, '\n');
        BOOST_LOG_TRIVIAL(debug) << "[" << __func__ << "]: " << " - Data sent: '" << m_WriteData << "' Bytes transferred: '" << BytesTransferred <<  "'";
        m_WriteData.clear();
    }
}
