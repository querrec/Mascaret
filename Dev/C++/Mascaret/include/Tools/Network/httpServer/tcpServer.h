#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "httpServer_plateform.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp> 
#include <boost/asio.hpp>


#include "tcpConnectionFactory.h"

class HttpConnection;
class HttpRequest;

class HTTPSERVER_API TcpServer : private boost::noncopyable
{
	public:
		TcpServer(int port,TcpConnectionFactory* factory);
		virtual ~TcpServer();
		void start();
		void stop();

		TcpConnectionFactory* getConnectionFactory();
		void setConnectionFactory(TcpConnectionFactory* factory);
		int getThreadPoolSize();
		void setThreadPoolSize(int threadPoolSize);
		boost::asio::io_service& getIoService();

	protected:
		virtual boost::shared_ptr<TcpConnection> createConnection();

	private:
		
		void startAccept();
		void handleAccept(boost::shared_ptr<TcpConnection> connection, const boost::system::error_code& e);
		void createServer();
		boost::shared_ptr<boost::thread> _threadStart;
		int _port;

		boost::asio::io_service _io_service;
		boost::asio::ip::tcp::acceptor _acceptor;
		TcpConnectionFactory* _connectionFactory;
		std::size_t _threadPoolSize;
		
};
inline TcpConnectionFactory* TcpServer::getConnectionFactory()
{
	return _connectionFactory;
}
inline void TcpServer::setConnectionFactory(TcpConnectionFactory* factory)
{
	_connectionFactory=factory;
}
inline boost::asio::io_service& TcpServer::getIoService()
{
	return _io_service;
}
inline int TcpServer::getThreadPoolSize()
{
	return _threadPoolSize;
}
inline void TcpServer::setThreadPoolSize(int threadPoolSize)
{
	_threadPoolSize=threadPoolSize;
}

#endif
