
#ifndef MANAGE_AGENT_SERVLET_H
#define MANAGE_AGENT_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageAgentServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageAgentServlet();
extern "C" void destroyManageAgentServlet(HttpServlet* s);

#endif
