
#ifndef MANAGE_AGENTS_SERVLET_H
#define MANAGE_AGENTS_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageAgentsServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageAgentsServlet();
extern "C" void destroyManageAgentsServlet(HttpServlet* s);

#endif
