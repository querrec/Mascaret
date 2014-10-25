
#ifndef MANAGE_INDEX_SERVLET_H
#define MANAGE_INDEX_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageIndexServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageIndexServlet();
extern "C" void destroyManageIndexServlet(HttpServlet* s);

#endif
