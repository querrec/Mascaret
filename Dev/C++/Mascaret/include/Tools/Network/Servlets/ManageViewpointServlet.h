#ifndef MANAGE_VIEWPOINT_SERVLET_H
#define MANAGE_VIEWPOINT_SERVLET_H 1

#include "httpServer/httpServlet.h"

class ManageViewpointServlet: public HttpServlet
{
public:
  virtual ~ManageViewpointServlet();
  virtual void manageRequest(HttpRequest* req);
};

#endif
