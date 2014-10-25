
#ifndef MANAGE_SET_VALUE_ATTRIBUTE_SERVLET_H
#define MANAGE_SET_VALUE_ATTRIBUTE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageSetAttributeValueServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageSetAttributeValueServlet();
extern "C" void destroyManageSetAttributeValueServlet(HttpServlet* s);

#endif
