
#ifndef MANAGE_MESSAGES_RECUS_SERVLET_H
#define MANAGE_MESSAGES_RECUS_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageMessagesRecusServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageMessagesRecusServlet();
extern "C" void destroyManageMessagesRecusServlet(HttpServlet* s);

#endif
