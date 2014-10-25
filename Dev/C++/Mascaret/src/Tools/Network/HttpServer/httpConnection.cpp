#include <sstream>
#include <boost/algorithm/string.hpp>

#include "httpServer/httpConnection.h"
#include "httpServer/httpRequest.h"
#include "httpServer/httpServer.h"

HttpConnection::HttpConnection(boost::asio::io_service& io_service) : TcpConnection(io_service), _server(NULL)
{
}
HttpConnection::HttpConnection(boost::asio::io_service& io_service, HttpServer* server) : TcpConnection(io_service), _server(server)
{
}
HttpConnection::~HttpConnection()
{
}

void HttpConnection::start()
{
	HttpRequest* req=HttpRequest::createRequestFromSocket(&_socket);
	if(_server != NULL)
	{
		_server->manageRequest(req);
	}
	delete req;
}
