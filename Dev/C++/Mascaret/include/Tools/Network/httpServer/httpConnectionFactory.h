#ifndef HTTPCONNECTION_FACTORY_H
#define HTTPCONNECTION_FACTORY_H

#include "httpServer_plateform.h"
#include "httpConnection.h"
#include "tcpConnectionFactory.h"
#include "httpServer.h"

class HTTPSERVER_API HttpConnectionFactory : public TcpConnectionFactory
{
	public:
		virtual boost::shared_ptr<TcpConnection> create(boost::asio::io_service& io_service);
		virtual boost::shared_ptr<TcpConnection> create(boost::asio::io_service& io_service, HttpServer* server);

};

inline boost::shared_ptr<TcpConnection> HttpConnectionFactory::create(boost::asio::io_service& io_service)
{
	return boost::shared_ptr<TcpConnection>(new HttpConnection(io_service));
}
inline boost::shared_ptr<TcpConnection> HttpConnectionFactory::create(boost::asio::io_service& io_service, HttpServer* server)
{
	return boost::shared_ptr<TcpConnection>(new HttpConnection(io_service,server));
}

#endif
