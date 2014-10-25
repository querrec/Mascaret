
#ifndef MANAGE_ACLMESSAGE_SERVLET_H
#define MANAGE_ACLMESSAGE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageACLMessageServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageACLMessageServlet();
extern "C" void destroyManageACLMessageServlet(HttpServlet* s);

#endif
