
#ifndef MANAGE_SCENARIO_SERVLET_H
#define MANAGE_SCENARIO_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageScenarioServlet : public HttpServlet
{
public:
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageScenarioServlet();
extern "C" void destroyManageScenarioServlet(HttpServlet* s);

#endif
