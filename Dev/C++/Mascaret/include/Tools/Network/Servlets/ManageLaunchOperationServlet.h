
#ifndef MANAGE_LAUNCH_OPERATION_SERVLET_H
#define MANAGE_LAUNCH_OPERATION_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageLaunchOperationServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageLaunchOperationServlet();
extern "C" void destroyManageLaunchOperationServlet(HttpServlet* s);

#endif
