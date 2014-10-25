
#ifndef MANAGE_AGENTKBHEAD_SERVLET_H
#define MANAGE_AGENTKBHEAD_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageAgentKBHeadServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageAgentKBHeadServlet();
extern "C" void destroyManageAgentKBHeadServlet(HttpServlet* s);

#endif
