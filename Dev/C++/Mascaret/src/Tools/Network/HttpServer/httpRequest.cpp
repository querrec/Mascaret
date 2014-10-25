#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "httpServer/httpRequest.h"


using std::string;
using std::vector;
using std::istringstream;
using std::map;
#include "iostream"
#include "fstream"

/// TODO : Create a service class for this
#define SPC_BASE16_TO_10(x) (((x) >= '0' && (x) <= '9') ? ((x) - '0') : \
                             (toupper((x)) - 'A' + 10))

char *spc_decode_url(const char *url, size_t *nbytes) {
  char       *out, *ptr;
  const char *c;

  if (!(out = ptr = strdup(url))) return 0;
  for (c = url;  *c;  c++) {
	if(*c == '+') *ptr++=' ';
    else if (*c != '%' || !isxdigit(c[1]) || !isxdigit(c[2])) *ptr++ = *c;
    else {
      *ptr++ = (SPC_BASE16_TO_10(c[1]) * 16) + (SPC_BASE16_TO_10(c[2]));
      c += 2;
    }
  }
  *ptr = 0;
  if (nbytes) *nbytes = (ptr - out); /* does not include null byte */
  return out;
}

HttpRequest::HttpRequest() : _session(NULL),_contentLength(0), _socket(NULL)
{
	_response=new HttpResponse();
	_headers["Content-Length"]="0";

}
//HttpRequest::HttpRequest(const string& method,const string& hostname,const string& requestURI) : _contentLength(0),_session(NULL),_content(""),_method(method),_requestURI(requestURI), _socket(NULL)
//{
//	_response=new HttpResponse();
//	_headers["Content-Length"]="0";
//}
HttpRequest::~HttpRequest()
{
	delete _response;
}
HttpRequest* HttpRequest::createRequestFromHeader(const std::string& request)
{
	HttpRequest* req=new HttpRequest();
	istringstream iss(request);
	iss >> req->_method >> req->_requestURI;
	string httpVersion;
	iss >> httpVersion;
	string line;
	bool first=true;
	while(true)
	{
		char buf[1024];
		iss.getline(buf,1024);
		line=buf;
		boost::trim(line);
		if(!line.length() && !first)
			break;
		first=false;
		size_t index=line.find(':');
		if(index != string::npos)
		{
			string name=line.substr(0,index);
			boost::trim(name);
			string value=line.substr(index+1);
			boost::trim(value);
			req->_headers[name]=value;
			if(name == "Content-Length")
			{
				istringstream iss(value);
				int contentLength;
				iss >> contentLength;
				req->_contentLength=contentLength;
			}
		}
	}

	return req;
}
HttpRequest* HttpRequest::createRequestFromSocket(boost::asio::ip::tcp::socket* socket)
{
	std::string tooMuch;
	std::string header=readHeader(socket, tooMuch);

	HttpRequest* req=HttpRequest::createRequestFromHeader(header);
	int length=req->getContentLength();
	char* buf=new char[length+1];
	int nb=tooMuch.length();
	strncpy(buf,tooMuch.c_str(),nb);
	nb+=boost::asio::read(*socket,boost::asio::buffer(buf+nb,length-nb));
	buf[length]='\0';
	if(nb != length)
		std::cerr << "Request content size error: Content-Size: "<< length << ", size read :"<< nb <<std::endl;
	req->_content=buf;
	delete[] buf;
	req->readParameters();
	req->getResponse()->_socket=socket;
	req->_socket=socket;
	return req;
}
std::string HttpRequest::readHeader(boost::asio::ip::tcp::socket* socket, std::string& tooMuch)
{
	int nb=0;
	boost::asio::streambuf buf;
        if (socket == NULL) return"";
	//std::cerr << "socket = " << socket << std::endl;
	std::ofstream file;
	try {
	  nb=boost::asio::read_until(*socket, buf,boost::regex("\r?\n\r?\n"));
    	}
	catch(boost::system::system_error& e){
	  std::cerr << "######################## ERREUR SOCKET EXCEPTION ###########################" << std::endl;
	  std::cerr << "######################## ERREUR SOCKET EXCEPTION ###########################" << std::endl;
	  std::cerr << "######################## ERREUR SOCKET EXCEPTION ###########################" << std::endl;
	  std::cerr << "######################## ERREUR SOCKET EXCEPTION ###########################" << std::endl;
	  std::cerr << "######################## ERREUR SOCKET EXCEPTION ###########################" << std::endl;
	  std::cerr << "######################## ERREUR SOCKET EXCEPTION ###########################" << std::endl;
	  file.open("boostException.txt");
	  if (file.is_open())  
	    {
	     file << "ERROR SOCKET" << std::endl;
	     file.close();
	    } 
	  else 
	    std::cerr << "Error opening file" << std::endl;
        }
	boost::asio::streambuf::const_buffers_type bufs = buf.data();
	std::string header(
			boost::asio::buffers_begin(bufs),
			boost::asio::buffers_begin(bufs) + nb);
	tooMuch=std::string(
			boost::asio::buffers_begin(bufs)+ nb,
			boost::asio::buffers_begin(bufs) + buf.size());
	return header;
}
std::string HttpRequest::getRequestString()
{
	string out = _method+" "+_requestURI;
	if(_method == "GET")
	{
		out+="?" + getParametersUrlEncoded();
	}
	out += " HTTP/1.1\r\n";
	map<string,string>::iterator i = _headers.begin();
	while(i != _headers.end())
	{
		out += i->first + ": " + i->second + "\r\n";
		i++;
	}
	out+="\r\n";
	if(_method == "POST")
	{
		out+=getParametersUrlEncoded();
	}
	return out;
}

string HttpRequest::getParametersUrlEncoded()
{
	string out="";
	map<string,string>::iterator i=_parameters.begin();
	while(i != _parameters.end())
	{
		out+=i->first+"="+i->second;
	}
	return out;
}
HttpSession* HttpRequest::getSession(bool create)
{
	if(_session == NULL && create)
		_session = new HttpSession();
	return _session;
}
void HttpRequest::readParameters()
{
	string urlEncodedParams="";
	if(_method == "POST")
	{
		if(!boost::starts_with(getHeader("Content-Type"),"application/x-www-form-urlencoded"))
		{
			//std::cerr << "POST Request Content-Type=" << getHeader("Content-Type") << std::endl;
			return;
		}
		urlEncodedParams=_content;
	}
	else if(_method == "GET")
	{
		size_t pos = _requestURI.find("?");
		if(pos != string::npos)
		{
			urlEncodedParams=_requestURI.substr(pos+1);
		}
	}
	if(!urlEncodedParams.empty())
	{
		vector<string> params;
		boost::split(params,urlEncodedParams,boost::is_any_of("&"));
		for(vector<string>::iterator i=params.begin();i!=params.end();i++)
		{
			size_t pos = i->find("=");
			if(pos != string::npos)
			{
				size_t nByte=0;
				_parameters[i->substr(0,pos)]=spc_decode_url(i->substr(pos+1).c_str(),&nByte);
			}
			else
			{
				_parameters[*i]="";
			}
		}
	}
}

