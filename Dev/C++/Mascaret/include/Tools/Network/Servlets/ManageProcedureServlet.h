
#ifndef MANAGE_PROCEDURE_SERVLET_H
#define MANAGE_PROCEDURE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageProcedureServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageProcedureServlet();
extern "C" void destroyManageProcedureServlet(HttpServlet* s);

#endif
