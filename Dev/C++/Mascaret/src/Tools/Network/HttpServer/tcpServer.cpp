#include <iostream>

#include "httpServer/tcpServer.h"
#include "httpServer/tcpConnection.h"

using std::string;

using boost::asio::ip::tcp;
using boost::thread;

TcpServer::TcpServer(int port, TcpConnectionFactory* factory) : _port(port),
		_acceptor(_io_service,tcp::endpoint(tcp::v4(),port)),
		_connectionFactory(factory),
		_threadPoolSize(3)
{
	
}

TcpServer::~TcpServer()
{
	std::cerr << "~TcpServer" << std::endl;
	if(_threadStart)
		stop();
}
void
TcpServer::start()
{
	assert(!_threadStart);
	_threadStart = shared_ptr<thread>(new thread(boost::bind(&TcpServer::createServer, this)));
}

void TcpServer::stop()
{
	assert(_threadStart);
	_io_service.stop();
	_threadStart->join();
}


void TcpServer::createServer()
{
	try
	{
		std::vector<boost::shared_ptr<boost::thread> > threads;
		for (std::size_t i = 0; i < _threadPoolSize; ++i)
		{
			startAccept();
			boost::shared_ptr<boost::thread> thread(new boost::thread(
					boost::bind(&boost::asio::io_service::run, &_io_service)));
			threads.push_back(thread);
		}
		//std::cerr << "joining threads" << std::endl;
		// Wait for all threads in the pool to exit.
		for (std::size_t i = 0; i < threads.size(); ++i)
			threads[i]->join();
		//std::cerr << "threads joined" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

}

void TcpServer::startAccept()
{
	shared_ptr<TcpConnection> connection=createConnection();
	_acceptor.async_accept(connection->socket(),
						boost::bind(&TcpServer::handleAccept, 
						this,
						connection,
						boost::asio::placeholders::error));
}
shared_ptr<TcpConnection> TcpServer::createConnection()
{
	return _connectionFactory->create(_io_service);
}
void TcpServer::handleAccept(shared_ptr<TcpConnection> connection, const boost::system::error_code& e)
{
	if(!e)
	{
		connection->start();
	}
	else
	{
		std::cerr << "----------------> Error accepting" <<std::endl;
	}
	startAccept();
}

