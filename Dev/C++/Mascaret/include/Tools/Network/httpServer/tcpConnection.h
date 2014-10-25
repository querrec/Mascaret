#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "httpServer_plateform.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp> 
#include <boost/asio.hpp>



class HTTPSERVER_API TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
	public:
		TcpConnection(boost::asio::io_service& io_service);
		virtual ~TcpConnection();

		inline boost::asio::ip::tcp::socket& socket()
		{
			return _socket;
		}
		virtual void start()=0;

	protected:
		boost::asio::ip::tcp::socket _socket;
};

#endif
