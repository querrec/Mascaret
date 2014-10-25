
#ifndef MANAGE_EXECUTE_QUERY_SERVLET_H
#define MANAGE_EXECUTE_QUERY_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageExecuteQueryServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageExecuteQueryServlet();
extern "C" void destroyManageExecuteQueryServlet(HttpServlet* s);

#endif
