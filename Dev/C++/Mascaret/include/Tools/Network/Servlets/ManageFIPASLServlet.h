
#ifndef MANAGE_FIPASL_SERVLET_H
#define MANAGE_FIPASL_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageFIPASLServlet : public HttpServlet
{
public:
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageFIPASLServlet();
extern "C" void destroyManageFIPASLServlet(HttpServlet* s);

#endif
