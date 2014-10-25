
#ifndef MANAGE_FIPASLTEST_SERVLET_H
#define MANAGE_FIPASLTEST_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageFIPASLTestServlet : public HttpServlet
{
	void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageFIPASLTestServlet();
extern "C" void destroyManageFIPASLTestServlet(HttpServlet* s);

#endif
