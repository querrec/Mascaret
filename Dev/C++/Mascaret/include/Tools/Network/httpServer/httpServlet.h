#ifndef HTTP_SERVLET_H
#define HTTP_SERVLET_H 1

#include <iostream>
#include "httpRequest.h"
#include "httpResponse.h"


class HttpServlet
{
  public:
    virtual void manageRequest(HttpRequest* request)=0;
    int _port;
    virtual ~HttpServlet(){};
};
typedef HttpServlet* create_servlet_t();
typedef void destroy_servlet_t(HttpServlet*);

#endif
