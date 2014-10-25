#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include "httpServer_plateform.h"
#include <boost/bind.hpp>
#include "tcpConnection.h"

class HttpServer;

class HTTPSERVER_API HttpConnection : public TcpConnection
{
	public:
		HttpConnection(boost::asio::io_service& io_service);
		HttpConnection(boost::asio::io_service& io_service,HttpServer* server);
		virtual ~HttpConnection();
		void start();
		//void write(const std::string& content);
		//void write(const void* content,size_t len);
	private:
		//std::string readHeader(std::string& tooMuch);
		HttpServer* _server;
};

#endif
