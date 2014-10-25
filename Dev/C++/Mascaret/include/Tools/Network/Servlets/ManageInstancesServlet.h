
#ifndef MANAGE_INSTANCES_SERVLET_H
#define MANAGE_INSTANCES_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageInstancesServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageInstancesServlet();
extern "C" void destroyManageInstancesServlet(HttpServlet* s);

#endif
