
#ifndef MANAGE_CLASS_SERVLET_H
#define MANAGE_CLASS_SERVLET_H 1
#include "httpServer/httpServlet.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/Class.h"

class ManageClassServlet : public HttpServlet
{
public:
	void manageRequest(HttpRequest* req);
private:
	boost::shared_ptr<VEHA::Class> _findClasse(boost::shared_ptr<VEHA::Package> package, const std::string & id);
};
extern "C" HttpServlet* createManageClassServlet();
extern "C" void destroyManageClassServlet(HttpServlet* s);

#endif
