
#ifndef MANAGE_ATTRIBUTE_SERVLET_H
#define MANAGE_ATTRIBUTE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageAttributeServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageAttributeServlet();
extern "C" void destroyManageAttributeServlet(HttpServlet* s);

#endif
