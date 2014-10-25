
#ifndef MANAGE_APPLICATION_SERVLET_H
#define MANAGE_APPLICATION_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageApplicationServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageApplicationServlet();
extern "C" void destroyManageApplicationServlet(HttpServlet* s);

#endif
