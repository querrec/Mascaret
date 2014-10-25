
#ifndef MANAGE_SIGNAL_SERVLET_H
#define MANAGE_SIGNAL_SERVLET_H 1

#include "httpServer/httpServlet.h"

#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include "Tools/veha_plateform.h"

namespace VEHA
{
class Action;
}

using namespace boost;
using namespace VEHA;

class ManageSignalServlet : public HttpServlet
{
public :
	void manageRequest(HttpRequest* req);
	template<class T1,class T2>
        boost::signals::connection addCallbackActionDone(shared_ptr<T1> obj, void (T2::*method)(shared_ptr<Action>));
protected:
boost::signal<void (shared_ptr<Action>)> _cbActionDone;

};


template<class T1, class T2>
inline boost::signals::connection ManageSignalServlet::addCallbackActionDone(shared_ptr<T1> obj,void (T2::*method)(shared_ptr<Action>))
{
        T2 * dummy1=(T1 *)0; // ensure T1 inherits from T2
        (void)dummy1;
        return _cbActionDone.connect(bind(method,obj,_1));
}



extern "C" HttpServlet* createManageSignalServlet();
extern "C" void destroyManageSignalServlet(HttpServlet* s);

#endif
