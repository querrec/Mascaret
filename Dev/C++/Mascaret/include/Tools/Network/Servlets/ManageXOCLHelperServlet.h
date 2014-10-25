
#ifndef MANAGE_XOCL_HELPER_SERVLET_H
#define MANAGE_XOCL_HELPER_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageXOCLHelperServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageXOCLHelperServlet();
extern "C" void destroyManageXOCLHelperServlet(HttpServlet* s);

#endif
