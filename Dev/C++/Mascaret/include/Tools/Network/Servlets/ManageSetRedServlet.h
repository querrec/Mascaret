
#ifndef MANAGE_SET_RED_SERVLET_H
#define MANAGE_SET_RED_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageSetRedServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageSetRedServlet();
extern "C" void destroyManageSetRedServlet(HttpServlet* s);

#endif
