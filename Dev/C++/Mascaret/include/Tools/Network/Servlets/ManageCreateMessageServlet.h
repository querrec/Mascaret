
#ifndef MANAGE_CREATE_MESSAGE_SERVLET_H
#define MANAGE_CREATE_MESSAGE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageCreateMessageServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageCreateMessageServlet();
extern "C" void destroyManageCreateMessageServlet(HttpServlet* s);

#endif
