
#ifndef MANAGE_AGENTKB_SERVLET_H
#define MANAGE_AGENTKB_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageAgentKnowledgeBaseServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageAgentKnowledgeBaseServlet();
extern "C" void destroyManageAgentKnowledgeBaseServlet(HttpServlet* s);

#endif
