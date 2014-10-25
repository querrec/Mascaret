
#ifndef MANAGE_MESSAGES_ENVOYES_SERVLET_H
#define MANAGE_MESSAGES_ENVOYES_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageMessagesEnvoyesServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageMessagesEnvoyesServlet();
extern "C" void destroyManageMessagesEnvoyesServlet(HttpServlet* s);

#endif
