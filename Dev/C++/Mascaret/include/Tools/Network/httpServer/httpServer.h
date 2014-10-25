#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "httpServer_plateform.h"

#include <iostream>

#include "httpServer/tcpServer.h"
#include "httpServer/httpRequest.h"
#include "httpServer/httpServlet.h"



class HTTPSERVER_API HttpServer : public TcpServer
{
	public:
		HttpServer(int port);
		HttpServer(int port, const std::string& resourcesDir);
		virtual ~HttpServer();

		inline int getPort() { return _port; };

		HttpServlet* getServlet(const std::string& url);
		bool addServlet(const std::string& url,HttpServlet* servlet);
		bool addServlet(const std::string& url,const std::string& className,const std::string& libray);
		void manageRequest(HttpRequest * req);
		void setExtensionTypeMime(const std::string& ext,const std::string& type_mime);
		std::string getExtensionTypeMime(const std::string& ext);
	protected:
		virtual boost::shared_ptr<TcpConnection> createConnection();
	private:
		HttpServlet* createHttpServlet(const std::string& className,const std::string& library);

		std::map<std::string,std::string> _typesMime;
		std::string _resourcesDir;
		
		std::map<std::string,HttpServlet*> _servlets;
		std::map<std::string,std::string> _servletMapping;
		std::map<std::string,void*> _libs;
		int _port;
};

inline HttpServlet* HttpServer::getServlet(const std::string& url)
{
	std::map<std::string,HttpServlet*>::iterator i=_servlets.find(url);
	if(i != _servlets.end())
		return i->second;
	return NULL;
}
inline bool HttpServer::addServlet(const std::string& url,HttpServlet* servlet)
{
	if(servlet != NULL)
	{
		_servlets[url]=servlet;
		return true;
	}
	return false;
}
inline void HttpServer::setExtensionTypeMime(const std::string& ext,const std::string& type_mime)
{
	_typesMime[ext]=type_mime;
}
inline std::string HttpServer::getExtensionTypeMime(const std::string& ext)
{
	std::map<std::string,std::string>::iterator it=_typesMime.find(ext);
	if(it != _typesMime.end())
		return it->second;
	return "unknown/unknown";
}
#endif
