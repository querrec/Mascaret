
#ifndef MANAGE_CHANGE_GEOMETRY_SERVLET_H
#define MANAGE_CHANGE_GEOMETRY_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageChangeGeometryServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageChangeGeometryServlet();
extern "C" void destroyManageChangeGeometryServlet(HttpServlet* s);

#endif
