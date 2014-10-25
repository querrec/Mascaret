
#ifndef MANAGE_SEND_USER_MESSAGECLASS_SERVLET_H
#define MANAGE_SEND_USER_MESSAGE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageSendUserMessageServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageSendUserMessageServlet();
extern "C" void destroyManageSendUserMessageServlet(HttpServlet* s);

#endif
