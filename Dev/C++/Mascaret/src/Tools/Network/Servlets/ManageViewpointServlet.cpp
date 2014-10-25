#include "Tools/Network/Servlets/ManageViewpointServlet.h"

#include "IEHA/VRApplication.h"
#include "IEHA/Visualisation/Window3D.h"
#include "IEHA/Visualisation/Camera.h"

using namespace VEHA;
using namespace BEHAVE;

#include <iostream>
using namespace std;

ManageViewpointServlet::~ManageViewpointServlet()
{
}

void ManageViewpointServlet::manageRequest(HttpRequest * req)
{
  string name = req->getParameter("name");

  vector < boost::shared_ptr<IEHA::Window3D> > windows = IEHA::VRApplication::getInstance()->getWindows();

  for (size_t i = 0; i < windows.size(); i++)
  {
    cerr << "ManageViewpointServlet::manageRequest ::: Goto viewpoint: " << name << endl;
    windows[i]->getCamera()->gotoViewPoint(name);
  }

  req->getResponse()->setContentType("text/html");
  req->getResponse()->write("<html>Done.</html>");
  req->getResponse()->flushBuffer();
}
