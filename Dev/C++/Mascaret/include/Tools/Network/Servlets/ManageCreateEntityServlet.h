
#ifndef MANAGE_CREATE_ENTITY_SERVLET_H
#define MANAGE_CREATE_ENTITY_SERVLET_H 1
#include "httpServer/httpServlet.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/Class.h"

class ManageCreateEntityServlet : public HttpServlet
{
public:
	void manageRequest(HttpRequest* req);
private:
	boost::shared_ptr<VEHA::Class> _findClasse(boost::shared_ptr<VEHA::Package> package, const std::string & id);
};
extern "C" HttpServlet* createManageCreateEntityServlet();
extern "C" void destroyManageCreateEntityServlet(HttpServlet* s);

#endif
