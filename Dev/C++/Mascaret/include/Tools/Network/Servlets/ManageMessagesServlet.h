
#ifndef MANAGE_MESSAGES_SERVLET_H
#define MANAGE_MESSAGES_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageMessagesServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageMessagesServlet();
extern "C" void destroyManageMessagesServlet(HttpServlet* s);

#endif
