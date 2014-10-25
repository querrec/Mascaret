/*
 * Interaction.h
 *
 *  Created on: 26 févr. 2011
 *      Author: ping
 */

#ifndef INTERACTION_H_
#define INTERACTION_H_
#include "Tools/veha_plateform.h"
#include <boost/signal.hpp>
#include <boost/bind.hpp>

namespace VEHA
{
class Action;
}
namespace IEHA
{
using VEHA::Action;
class Button;
class Axis;
class Window3D;
class VEHA_API Interaction : public enable_shared_from_this<Interaction>
{
public:

	Interaction(shared_ptr<Window3D> window);
	virtual ~Interaction();

	virtual void addButtonAction(shared_ptr<Button> button,shared_ptr<Action> action, const string& target,bool pressed,const string& cursor="system", double maxdist=-1.0);
	virtual void removeButtonAction(shared_ptr<Button> button);
	virtual void removeAllButtonActions();

	template<class T1,class T2>
	boost::signals::connection addCallbackActionDone(shared_ptr<T1> obj, void (T2::*method)(shared_ptr<Action>));
protected:
	struct ButtonActionInfo
	{
		shared_ptr<Button> button;
		shared_ptr<Action> action;
		bool pressed;
		string target;
		string cursor;
		double maxdist;
	};
	virtual void _onButtonPressed(shared_ptr<Button> button,bool pressed);
	vector<ButtonActionInfo> _buttonActions;
	map<shared_ptr<Button>,boost::signals::connection > _buttonConnections;
	boost::signal<void (shared_ptr<Action>)> _cbActionDone;
  shared_ptr<Window3D> _window;
};
template<class T1, class T2>
inline boost::signals::connection Interaction::addCallbackActionDone(shared_ptr<T1> obj,void (T2::*method)(shared_ptr<Action>))
{
   	T2 * dummy1=(T1 *)0; // ensure T1 inherits from T2
   	(void)dummy1;
   	return _cbActionDone.connect(bind(method,obj,_1));
}
}
#endif /* INTERACTION_H_ */
