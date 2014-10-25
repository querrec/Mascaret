
#ifndef MANAGE_ACTION_SERVLET_H
#define MANAGE_ACTION_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageActionServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageActionServlet();
extern "C" void destroyManageActionServlet(HttpServlet* s);

#endif
