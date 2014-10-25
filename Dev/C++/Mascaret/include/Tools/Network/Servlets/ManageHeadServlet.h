
#ifndef MANAGE_HEAD_SERVLET_H
#define MANAGE_HEAD_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageHeadServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageHeadServlet();
extern "C" void destroyManageHeadServlet(HttpServlet* s);

#endif
