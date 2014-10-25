#include "Tools/Network/Servlets/ManageConnectServlet.h"
//#include <iostream>
#include <sstream>

void ManageConnectServlet::manageRequest(HttpRequest* req)
{
  std::ostringstream address;
  address << "portablerq:" << _port << "/MASCARET";

	req->getResponse()->write(address.str());
	std::cerr <<  "CONNECTION TO " << address.str() << "........" << std::endl;
}
extern "C" HttpServlet* createManageConnectServlet(int port)
{
  HttpServlet* svlt = new ManageConnectServlet();
  svlt->_port = port;
	return svlt;
}
extern "C" void destroyManageConnectServlet(HttpServlet* s)
{
	delete s;
}
