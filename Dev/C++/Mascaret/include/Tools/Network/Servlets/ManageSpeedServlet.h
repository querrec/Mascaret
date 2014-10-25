#ifndef MANAGE_SPEED_SERVLET_H
#define MANAGE_SPEED_SERVLET_H 1

#include "httpServer/httpServlet.h"

class ManageSpeedServlet : public HttpServlet
{
public:
  virtual ~ManageSpeedServlet();
  virtual void manageRequest(HttpRequest* req);
};

extern "C" HttpServlet* createManageSpeedServlet();
extern "C" void destroyManageSpeedServlet(HttpServlet* s);

#endif
