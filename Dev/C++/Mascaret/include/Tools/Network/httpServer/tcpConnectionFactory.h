#ifndef TCPCONNECTION_FACTORY_H
#define TCPCONNECTION_FACTORY_H

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

class TcpConnection;

class TcpConnectionFactory
{
	public:
		virtual boost::shared_ptr<TcpConnection> create(boost::asio::io_service& io_service)=0;
};

#endif
