
#ifndef MANAGE_CONNECT_SERVLET_H
#define MANAGE_CONNECT_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageConnectServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageConnectServlet(int port);
extern "C" void destroyManageConnectServlet(HttpServlet* s);

#endif
