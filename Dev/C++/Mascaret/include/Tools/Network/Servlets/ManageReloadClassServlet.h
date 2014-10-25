
#ifndef MANAGE_RELOADCLASS_SERVLET_H
#define MANAGE_RELOADCLASS_SERVLET_H 1
#include "httpServer/httpServlet.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/Class.h"

class ManageReloadClassServlet : public HttpServlet
{
public:
	void manageRequest(HttpRequest* req);
private:
	boost::shared_ptr<VEHA::Class> _findClasse(boost::shared_ptr<VEHA::Package> package, const std::string & id);
};
extern "C" HttpServlet* createManageReloadClassServlet();
extern "C" void destroyManageReloadClassServlet(HttpServlet* s);

#endif
