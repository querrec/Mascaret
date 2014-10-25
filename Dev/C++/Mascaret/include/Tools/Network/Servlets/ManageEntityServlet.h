
#ifndef MANAGE_ENTITY_SERVLET_H
#define MANAGE_ENTITY_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageEntityServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageEntityServlet();
extern "C" void destroyManageEntityServlet(HttpServlet* s);

#endif
