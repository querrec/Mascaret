#ifndef MANAGE_ENTITY_JSON_SERVLET_H
#define MANAGE_ENTITY_JSON_SERVLET_H 1
#include "httpServer/httpServlet.h"

class ManageEntityJSONServlet: public HttpServlet
{
  void manageRequest(HttpRequest* req);
};
extern "C" HttpServlet* createManageEntityJSONServlet();
extern "C" void destroyManageEntityJSONServlet(HttpServlet* s);

#endif
