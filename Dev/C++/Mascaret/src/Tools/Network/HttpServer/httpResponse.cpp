#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/time_facet.hpp>

#include "httpServer/httpResponse.h"
#include <sstream>

using std::string;
using std::map;
using std::ostringstream;

HttpResponse::HttpResponse()
{
	_isCommited=false;
	reset();
}
HttpResponse::~HttpResponse()
{
}
void HttpResponse::flushBuffer()
{
	if(!_isCommited)
	{
		_isCommited=true;
                try{
		boost::asio::write(*_socket,boost::asio::buffer(getFormattedStringResponse()));
                }
                catch(...)
		{ return;}

	}
	else
	{
		boost::asio::write(*_socket,boost::asio::buffer(_buffer));
	}
	_buffer="";
}

void HttpResponse::reset()
{
	if(!_isCommited)
	{
		_buffer="";
		_headers.clear();
		_headers["Content-Type"]="text/html";
		_cookies.clear();
		_status=200;
	}
}


string HttpResponse::getFormattedStringResponse()
{
	ostringstream _ostream;
	_ostream << "HTTP/1.1 " << _status << " " << getMessageFromStatus() << "\r\n";
	map<string,string>::iterator i = _headers.begin();
	while(i != _headers.end())
	{
		_ostream << i->first << ": " << i->second << "\r\n";
		i++;
	}
	if(!_cookies.empty())
		_ostream << "Set-Cookie: ";
	for(size_t j=0;j<_cookies.size();j++)
	{
		if(j > 0)
			_ostream << ";";
		_ostream <<_cookies[j].getName() << "=" << _cookies[j].getValue();
		if(_cookies[j].getExpires() != 0)
		{
			using namespace boost::gregorian;
			using namespace boost::posix_time;
			ptime now=microsec_clock::universal_time();
			ptime expires=now + minutes(_cookies[j].getExpires());

			 time_facet* tf = new time_facet("%a, %d-%b-%Y %H:%M:%S GMT");
			 _ostream.imbue(std::locale(std::cout.getloc(), tf));
			_ostream << "; expires= " << expires;
		}
	}
	if(!_cookies.empty())
		_ostream << "\r\n";
	_ostream << "\r\n";
	_ostream << _buffer;
	_isCommited=true;
	return _ostream.str();
}

void HttpResponse::write(long l)
{
	ostringstream valStr;
	valStr << l;
	_buffer+=valStr.str();
}
void HttpResponse::write(int i)
{
	ostringstream valStr;
	valStr << i;
	_buffer+=valStr.str();
}
void HttpResponse::write(const string& content)
{
	_buffer+=content;
}

string HttpResponse::getMessageFromStatus()
{
	switch(_status)
	{
		case 200:
			return "OK";
		case 201:
			return "CREATED";
		case 202:
			return "CREATED";
		case 203:
			return "PARTIAL INFORMATION";
		case 204:
			return "NO RESPONSE";
		case 205:
			return "RESET CONTENT";
		case 206:
			return "PARTIAL CONTENT";
		case 301:
			return "MOVED";
		case 302:
			return "FOUND";
		case 303:
			return "METHOD";
		case 304:
			return "NOT MODIFIED";
		case 400:
			return "BAD REQUEST";
		case 401:
			return "UNAUTHORIZED";
		case 402:
			return "PAYMENT REQUIRED";
		case 403:
			return "FORBIDDEN";
		case 404:
			return "NOT FOUND";
		case 500:
			return "INTERNAL ERROR";
		case 501:
			return "NOT IMPLEMENTED";
		case 502:
			return "BAD GATEWAY";
		case 503:
			return "SERVICE UNAVAILABLE";
		case 504:
			return "GATEWAY TIMEOUT";
	}
	return "UNKNOW";
}
