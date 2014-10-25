
#ifndef MANAGE_SEND_MESSAGE_SERVLET_H
#define MANAGE_SEND_MESSAGE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageSendMessageServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageSendMessageServlet();
extern "C" void destroyManageSendMessageServlet(HttpServlet* s);

#endif
