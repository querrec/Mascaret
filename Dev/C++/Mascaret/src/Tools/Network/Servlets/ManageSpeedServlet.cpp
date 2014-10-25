#include "Tools/Network/Servlets/ManageSpeedServlet.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"

using namespace VEHA;
using namespace BEHAVE;

#include <iostream>
using namespace std;

ManageSpeedServlet::~ManageSpeedServlet()
{
}

void ManageSpeedServlet::manageRequest(HttpRequest * req)
{
  string cmd = req->getParameter("action");
  if (cmd == "Plus")
  {
    BehaviorScheduler::getInstance()->setAccelerationFactor(
        min(3., BehaviorScheduler::getInstance()->getAccelerationFactor() + 0.1));
  }
  else if (cmd == "Minus")
  {
    BehaviorScheduler::getInstance()->setAccelerationFactor(
        max(0.1, BehaviorScheduler::getInstance()->getAccelerationFactor() - 0.1));
  }
  else if (cmd == "Default")
  {
    BehaviorScheduler::getInstance()->setAccelerationFactor(1.);
  }
  else if (cmd == "Play")
  {
    BehaviorScheduler::getInstance()->setSuspended(false);
  }
  else if (cmd == "Pause")
  {
    BehaviorScheduler::getInstance()->setSuspended(true);
  }

  double time = BehaviorScheduler::getInstance()->getElapsedVirtualTime();
  int nbSeconds = ((int) time) % 60;
  int nbMinutes = ((int) time) / 60;

  ostringstream oss;

  req->getResponse()->setContentType("text/json");
  oss << "{";
  oss << "\"time\":" << time << ",";
  oss << "\"time_str\":\"";
  oss << nbMinutes << ":";
  if (nbSeconds < 10)
  {
    oss << "0";
  }
  oss << nbSeconds;
  oss << "\",";
  oss << "\"paused\": \"" << (BehaviorScheduler::getInstance()->isSuspended() ?
      "pause" : "play") << "\",";
  oss << "\"speed\":" << BehaviorScheduler::getInstance()->getAccelerationFactor() << ",";
  oss << "\"speed_percent\":"
      << (int) (100. * BehaviorScheduler::getInstance()->getAccelerationFactor());
  oss << "}";
  req->getResponse()->write(oss.str());
  req->getResponse()->flushBuffer();
}

extern "C" HttpServlet* createManageSpeedServlet()
{
	return new ManageSpeedServlet();
}
extern "C" void destroyManageSpeedServlet(HttpServlet* s)
{
	delete s;
}
