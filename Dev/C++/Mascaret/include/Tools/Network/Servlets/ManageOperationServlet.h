
#ifndef MANAGE_OPERATION_SERVLET_H
#define MANAGE_OPERATION_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageOperationServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageOperationServlet();
extern "C" void destroyManageOperationServlet(HttpServlet* s);

#endif
