
#ifndef MANAGE_XOCL_HELPER_REMOVE_SERVLET_H
#define MANAGE_XOCL_HELPER_REMOVE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageXOCLHelperRemoveServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageXOCLHelperRemoveServlet();
extern "C" void destroyManageXOCLHelperRemoveServlet(HttpServlet* s);

#endif
