
#ifndef MANAGE_ORGANISATIONAL_STRUCTURE_SERVLET_H
#define MANAGE_ORGANISATIONAL_STRUCTURE_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageOrganisationalStructureServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageOrganisationalStructureServlet();
extern "C" void destroyManageOrganisationalStructureServlet(HttpServlet* s);

#endif
