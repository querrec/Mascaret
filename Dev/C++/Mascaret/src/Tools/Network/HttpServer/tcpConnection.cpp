#include "httpServer/tcpConnection.h"

TcpConnection::TcpConnection(boost::asio::io_service& io_service) : _socket(io_service)
{
}
TcpConnection::~TcpConnection()
{
}
