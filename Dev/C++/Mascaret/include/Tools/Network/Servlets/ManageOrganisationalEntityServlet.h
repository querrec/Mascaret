
#ifndef MANAGE_ORGANISATIONAL_ENTITY_SERVLET_H
#define MANAGE_ORGANISATIONAL_ENTITY_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageOrganisationalEntityServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageOrganisationalEntityServlet();
extern "C" void destroyManageOrganisationalEntityServlet(HttpServlet* s);

#endif
