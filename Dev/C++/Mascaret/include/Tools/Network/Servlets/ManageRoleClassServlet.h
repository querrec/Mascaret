
#ifndef MANAGE_ROLE_CLASS_SERVLET_H
#define MANAGE_ROLE_CLASS_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageRoleClassServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageRoleClassServlet();
extern "C" void destroyManageRoleClassServlet(HttpServlet* s);

#endif
