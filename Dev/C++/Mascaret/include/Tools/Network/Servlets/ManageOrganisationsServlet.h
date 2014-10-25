
#ifndef MANAGE_ORGANISATIONS_SERVLET_H
#define MANAGE_ORGANISATIONS_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageOrganisationsServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageOrganisationsServlet();
extern "C" void destroyManageOrganisationsServlet(HttpServlet* s);

#endif
