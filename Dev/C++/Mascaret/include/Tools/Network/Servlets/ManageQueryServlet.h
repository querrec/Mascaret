
#ifndef MANAGE_QUERY_SERVLET_H
#define MANAGE_QUERY_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageQueryServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageQueryServlet();
extern "C" void destroyManageQueryServlet(HttpServlet* s);

#endif
