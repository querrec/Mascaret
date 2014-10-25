#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H 1

#include "httpServer_plateform.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "cookie.h"
#include "httpConnection.h"

class HTTPSERVER_API HttpResponse
{
 	public:
		HttpResponse();
		virtual ~HttpResponse();
		void addCookie(Cookie c);
		void addCookie(const std::string& name, const std::string& value, size_t expires=0);
		void addDateHeader(const std::string& name, time_t t);
		void addHeader(const std::string& name,const std::string& value);
		void addIntHeader(const std::string& name,int value);
		bool containsHeader(const std::string& name);
		std::string encodeURL(const std::string& url);
		std::string encodeRedirectURL(const std::string& url);
		void flushBuffer();
		bool isCommited();
		void reset();
		void sendRedirect(const std::string& location);
		//void setConnection(HttpConnection* conn);
		void setContentType(const std::string& contentType);
		void setDateHeader(const std::string& name, time_t t);
		void setHeader(const std::string& name,const std::string& value);
		void setIntHeader(const std::string& name,int value);
		void setLocale(const std::string& locale);
		void setStatus(int sc);
		boost::asio::ip::tcp::socket* socket();
		void write(long l);
		void write(int i);
		void write(const std::string& content);

		friend class HttpRequest;
	private:
		std::string getMessageFromStatus();
		std::string getFormattedStringResponse();
		
		std::string _buffer;
		std::vector<Cookie> _cookies;
		bool _isCommited;
		std::map<std::string,std::string> _headers;
		int _status;
		std::string locale;
		boost::asio::ip::tcp::socket* _socket;
};

inline void HttpResponse::addCookie(Cookie c)
{
	_cookies.push_back(c);
}
inline void HttpResponse::addCookie(const std::string& name, const std::string& value, size_t expires)
{
	Cookie c(name,value,expires);
	_cookies.push_back(c);
}
inline void HttpResponse::addDateHeader(const std::string& /*name*/, time_t /*t*/)
{
	/// TODO
}
inline void HttpResponse::addHeader(const std::string& name,const std::string& value)
{
	_headers[name]=value;
}
inline void HttpResponse::addIntHeader(const std::string& name,int value)
{
	std::ostringstream valueStr;
	valueStr << value;
	_headers[name]=valueStr.str();
}
inline bool HttpResponse::containsHeader(const std::string& name)
{
	return _headers.find(name) != _headers.end();
}
inline std::string HttpResponse::encodeURL(const std::string& url)
{
	/// TODO
	return url;
}
inline std::string HttpResponse::encodeRedirectURL(const std::string& url)
{
	return url;
}
inline bool HttpResponse::isCommited()
{
	return _isCommited;
}
inline void HttpResponse::sendRedirect(const std::string& /*location*/)
{
	/// TODO
}
inline void HttpResponse::setContentType(const std::string& contentType)
{
	_headers["Content-Type"]=contentType;
}
inline void HttpResponse::setDateHeader(const std::string& /*name*/, time_t /*t*/)
{
	/// TODO
}
inline void HttpResponse::setHeader(const std::string& name,const std::string& value)
{
	_headers[name]=value;
}
inline void HttpResponse::setIntHeader(const std::string& name,int value)
{
	std::ostringstream valueStr;
	valueStr << value;
	_headers[name]=valueStr.str();	
}
inline void HttpResponse::setLocale(const std::string& /*locale*/)
{
	/// TODO
}
inline void HttpResponse::setStatus(int sc)
{
	_status=sc;
}
inline boost::asio::ip::tcp::socket* HttpResponse::socket()
{
	return _socket;
}

#endif
