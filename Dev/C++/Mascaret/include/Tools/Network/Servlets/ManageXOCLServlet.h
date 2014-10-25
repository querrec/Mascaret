
#ifndef MANAGE_XOCL_SERVLET_H
#define MANAGE_XOCL_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageXOCLServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageXOCLServlet();
extern "C" void destroyManageXOCLServlet(HttpServlet* s);

#endif
