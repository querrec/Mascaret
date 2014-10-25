#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H 1

#include <iostream>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>

#include "httpSession.h"
#include "httpResponse.h"
#include "cookie.h"

class HTTPSERVER_API HttpRequest
{
	public:
		//HttpRequest(const std::string& method,const std::string& hostname, const std::string& requestURI);
		virtual ~HttpRequest();

		//void forward(const std::string& location);
		std::vector<std::string> getAcceptedLocales();
		std::string getContent();
		int getContentLength();
		std::string getContentType();
		std::string getHeader(const std::string& name);
		std::vector<std::string> getHeadersNames();
		std::map<std::string,std::string> getHeaders();
		std::string getLocale();
		std::string getMethod();
		std::string getParameter(const std::string& name);
		std::map<std::string,std::string> getParameters();
		std::string getPathInfo();
		std::string getProtocol();
		std::string getQueryString();
		std::string getRemoteAdr();
		std::string getRequestedSessionId();
		std::string getRequestURI();
		std::string getRequestURL();
		HttpResponse* getResponse();
		std::string getServerName();
		int getServerPort();
		HttpSession* getSession(bool create = false);
		std::string getUserAgent();
		bool hasParameter(const std::string& name);
		bool isSecure();
		std::string getRequestString();

		//void setContent(const std::string& content);
		//void setParameter(const std::string& name,const std::string& value);
		boost::asio::ip::tcp::socket* socket();

		static HttpRequest* createRequestFromSocket(boost::asio::ip::tcp::socket* socket);
	private:
		static HttpRequest* createRequestFromHeader(const std::string& header);
		static std::string readHeader(boost::asio::ip::tcp::socket* socket,std::string& tooMuch);

		HttpRequest();
		std::string getParametersUrlEncoded();
		void readParameters();
		std::map<std::string, std::string> _headers;
		std::string _locale;
		std::map<std::string,std::string> _parameters;
		std::string _queryString;
		std::string _requestURI;
		std::string _sessionId;
		std::map<std::string,Cookie*> cookies;
		int _serverPort;
		HttpSession * _session;
		std::string _content;
		std::string _contentType;
		int _contentLength;
		std::string _method;
		std::string _serverName;
		HttpResponse* _response;
		boost::asio::ip::tcp::socket* _socket;
};

inline std::vector<std::string> HttpRequest::getAcceptedLocales()
{
	std::vector<std::string> locales;
	boost::split(locales,_locale,boost::is_any_of(";"));
	return locales;
}
inline std::string HttpRequest::getContent()
{
	return _content;
}

inline int HttpRequest::getContentLength()
{
	return _contentLength;
}
inline std::string HttpRequest::getContentType()
{
	return _headers["Content-Type"];
}
inline std::string HttpRequest::getHeader(const std::string& name)
{
	return _headers[name];
}
inline std::vector<std::string> HttpRequest::getHeadersNames()
{
	std::vector<std::string> names;
	std::map<std::string,std::string>::iterator i = _headers.begin();
	while(i != _headers.end())
	{
		names.push_back(i->first);
		i++;
	}
	return names;
}
inline std::map<std::string,std::string> HttpRequest::getHeaders()
{
	return _headers;
}
inline std::string HttpRequest::getLocale()
{
	return _locale;
}
inline std::string HttpRequest::getMethod()
{
	return _method;
}
inline std::string HttpRequest::getParameter(const std::string& name)
{
	std::map<std::string,std::string>::iterator i = _parameters.find(name);
	if(i != _parameters.end())
	{
		return i->second;
	}
	return "";
}
inline std::map<std::string,std::string> HttpRequest::getParameters()
{
	return _parameters;
}
inline std::string HttpRequest::getPathInfo()
{
	/// TODO : this is not the path info
	return _requestURI;
}
inline std::string HttpRequest::getProtocol()
{
	return "http";
}
inline std::string HttpRequest::getQueryString()
{
	/// TODO : this is not the query string...
	return getProtocol()+"://"+getServerName()+getRequestURI();
}
inline std::string HttpRequest::getRemoteAdr()
{
	return _socket->remote_endpoint().address().to_string();
}

inline std::string HttpRequest::getRequestedSessionId()
{
	/// TODO
	return "";
}
inline std::string HttpRequest::getRequestURI()
{
	return _requestURI;
}
inline std::string HttpRequest::getRequestURL()
{
	return getProtocol()+"://"+getServerName()+getRequestURI();
}
inline HttpResponse* HttpRequest::getResponse()
{
	return _response;
}
inline std::string HttpRequest::getServerName()
{
	return _serverName;
}
inline int HttpRequest::getServerPort()
{
	return _serverPort;
}
inline std::string HttpRequest::getUserAgent()
{
	std::map<std::string, std::string>::iterator it= _headers.find("User-Agent");
	if(it != _headers.end())
		return it->second;
	return "";
}
inline bool HttpRequest::hasParameter(const std::string& name)
{
	return _parameters.find(name) != _parameters.end();
}
inline bool HttpRequest::isSecure()
{
	return getProtocol() == "https";
}
//inline void HttpRequest::setContent(const std::string& content)
//{
//	_content=content;
//	_contentLength=content.length();
//	_headers["Content-Length"]=""+_contentLength;
//}
//inline void HttpRequest::setParameter(const std::string& name,const std::string& value)
//{
//	_parameters[name]=value;
//}
inline boost::asio::ip::tcp::socket* HttpRequest::socket()
{
	return _socket;
}
#endif
