#include "Tools/filesystem.h"
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include "httpServer/httpServer.h"
#include "httpServer/httpConnectionFactory.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
using std::string;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::filebuf;
using std::ifstream;
using std::ios;
using std::vector;
using std::map;

DECLARE_NAMESPACE_FILESYSTEM;

#define RESTART_SYSCALL(result,syscall) \
    do { (result)=(syscall); } \
    while(((result)<0)&&(errno==EINTR));

#ifdef _WIN32
static char error_buffer[65535];
static string GetErrorMessage()
{
	DWORD errCode=GetLastError();

	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, errCode,
	                          MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
	                          error_buffer, sizeof(error_buffer), NULL );
	return string(error_buffer);
}
#endif


HttpServer::HttpServer(int port) : TcpServer(port,new HttpConnectionFactory()), _resourcesDir("web"), _port(port)
{
}

HttpServer::HttpServer(int port, const string& resourcesDir) : TcpServer(port,new HttpConnectionFactory()), _resourcesDir(resourcesDir)
{
}

HttpServer::~HttpServer()
{
}

bool HttpServer::addServlet(const string& url, const string& className,const string& library)
{
	HttpServlet * serv=createHttpServlet(className,library);
	if(serv != NULL)
	{
		_servlets[url]=serv;
		return true;
	}
	return false;
}

HttpServlet* HttpServer::createHttpServlet(const string& className,const string& library)
{
	map<string,void*>::iterator it=_libs.find(library);
	void* h = NULL;
	if(it == _libs.end())
	{
		string library2=library;
#ifdef _WIN32
		library2="lib"+library+".dll";
		h=(void*)LoadLibrary(library2.c_str());
#else
		library2="lib"+library+".so";
		h=dlopen(library2.c_str(),RTLD_LAZY);
#endif
		if(h == NULL)
		{
#ifdef _WIN32

			cerr << "Impossible de charger la bibliothèque " << library2 << ": " << GetErrorMessage() << endl;
#else
			cerr << "Impossible de charger la bibliothèque " << library2 << ": " << dlerror() << endl;
#endif
			return NULL;
		}
	}
	else
	{
		h=it->second;
	}
	string funcName="create";
	funcName+=className;
#ifdef _WIN32
	create_servlet_t* cs=(create_servlet_t*)GetProcAddress((HINSTANCE)h,funcName.c_str());
#else
	create_servlet_t* cs=(create_servlet_t*)dlsym(h,funcName.c_str());
#endif
	if(cs == NULL)
	{
#ifdef _WIN32
		cerr << "Impossible de charger le symbole " << funcName << " de la bibliothèque "<< library << ": " << GetErrorMessage() << endl;
#else
		cerr << "Impossible de charger le symbole " << funcName << " de la bibliothèque "<< library << ": " << dlerror() << endl;
#endif
		return NULL;
	}
	return cs();
}
boost::shared_ptr<TcpConnection> HttpServer::createConnection()
{
	return ((HttpConnectionFactory*)getConnectionFactory())->create(getIoService(),this);
}

void
HttpServer::manageRequest(HttpRequest * req)
{
	if(req->getMethod() != "GET" &&
	   req->getMethod() != "POST")
	{
		req->getResponse()->setStatus(400);
		string output="<html><body>\nBad Request: "+req->getMethod()+" "+req->getRequestURI()+"</body></html>\n";
		req->getResponse()->write(output);
		req->getResponse()->flushBuffer();
	}
	else
	{
		size_t pos;
		string filename=_resourcesDir+req->getRequestURI();

		pos=filename.find('?');
		if(pos)
		{
			filename=filename.substr(0,pos);
		}
		fs::path p(filename);
		if(fs::is_directory(p))
		{
			filename+="/index.html";
			p=fs::path(filename);
		}
		ifstream fin;
		fin.open(filename.c_str(),ios::in | ios::binary);
		if(fin.fail())
		{
			string url=req->getRequestURI();
			size_t pos;
			if((pos=url.find('?')) != string::npos)
				url=url.substr(0,pos);
			HttpServlet* servlet = getServlet(url);
			if(!servlet)
			{
				req->getResponse()->setStatus(404);
				req->getResponse()->write("<html><head><title>Error 404</title></head><body><h1>Error 404</h1><p>"+req->getRequestURI()+" not found</p></body></html>");
				req->getResponse()->flushBuffer();
			}
			else
			{
				try
				{
					servlet->manageRequest(req);
					req->getResponse()->flushBuffer();
				}
				catch(...)
				{
					req->getResponse()->setStatus(500);
					req->getResponse()->write("<html><head><title>Error 500</title></head><body><h1>Error 500</h1><p>Internal error occurs</p></body></html>");
					req->getResponse()->flushBuffer();
				}
			}
		}
		else
		{
			fs::path p=filename;
			#if BOOST_FILESYSTEM_VERSION == 2
			string ext=p.extension();
			#else
			string ext=p.extension().string();
			#endif

			string contentType = getExtensionTypeMime(ext);
			ostringstream contentLength;
			long size=fs::file_size(p);
			contentLength << size;
			req->getResponse()->setHeader("Content-Type",contentType);
			req->getResponse()->setHeader("Content-Length",contentLength.str());
			req->getResponse()->flushBuffer();
			filebuf *pbuf=fin.rdbuf();
			char *buffer=new char[size];
			pbuf->sgetn (buffer,size);
			try
			{
				boost::asio::write(*req->getResponse()->socket(),boost::asio::buffer((void*)buffer,size));
			}
			catch (...)
			{
				cerr << "Something went wrong with 'boost::asio::write(*req->getResponse()->socket(),boost::asio::buffer((void*)buffer,size));'" << endl;
			}
			delete[] buffer;
			fin.close();
		}
	}
}


