
#ifndef MANAGE_CENTER_VIEW_SERVLET_H
#define MANAGE_CENTER_VIEW_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageCenterViewServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageCenterViewServlet();
extern "C" void destroyManageCenterViewServlet(HttpServlet* s);

#endif
