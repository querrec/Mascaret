
#ifndef MANAGE_CHANGE_ATTRIBUTE_SERVLET_H
#define MANAGE_CHANGE_ATTRIBUTE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageChangeAttributeServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageChangeAttributeServlet();
extern "C" void destroyManageChangeAttributeServlet(HttpServlet* s);

#endif
