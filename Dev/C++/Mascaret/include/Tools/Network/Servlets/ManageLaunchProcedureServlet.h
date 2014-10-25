
#ifndef MANAGE_LAUNCH_PROCEDURE_SERVLET_H
#define MANAGE_LAUNCH_PROCEDURE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageLaunchProcedureServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageLaunchProcedureServlet();
extern "C" void destroyManageLaunchProcedureServlet(HttpServlet* s);

#endif
