
#ifndef MANAGE_CREATE_USER_MESSAGE_SERVLET_H
#define MANAGE_CREATE_USER_MESSAGE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageCreateUserMessageServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageCreateUserMessageServlet();
extern "C" void destroyManageCreateUserMessageServlet(HttpServlet* s);

#endif
