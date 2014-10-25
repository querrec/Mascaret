
#ifndef MANAGE_MODEL_SERVLET_H
#define MANAGE_MODEL_SERVLET_H 1
#include "httpServer/httpServlet.h"
#include "VEHA/Kernel/Package.h"
#include "HAVE/VirtualHuman.h"

class ManageModelServlet : public HttpServlet
{
public:
	void manageRequest(HttpRequest* req);
private:
	void _printPackage(int level, HttpRequest * req, boost::shared_ptr<VEHA::Package> package, shared_ptr<VirtualHuman> human);
};
extern "C" HttpServlet* createManageModelServlet();
extern "C" void destroyManageModelServlet(HttpServlet* s);

#endif
