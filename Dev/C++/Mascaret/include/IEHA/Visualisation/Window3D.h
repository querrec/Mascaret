/*
 * Light.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef WINDOW3D_H_
#define WINDOW3D_H_

#include "IEHA/Visualisation/Renderer.h"
#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Kernel/Environment.h"
#include "IEHA/Interaction/Peripheric.h"
#include "IEHA/Interaction/Cursor.h"

#include <boost/signal.hpp>
#include <boost/bind.hpp>

//FRED
namespace BEHAVE
{
class ProcedureExecution;
}
using namespace BEHAVE;
//END FRED

namespace VEHA
{
class Action;
}
using namespace VEHA;


namespace IEHA
{

typedef struct _SigCallFunc
{ 
   void (BehaviorExecution::*function)(shared_ptr<VEHA::Entity>, string, map<string, shared_ptr<ValueSpecification> >);
   BehaviorExecution* behavior;
} SigCallFunc;

class Camera;
class Peripheric;
class Interaction;
class Cursor;
class Navigation;
class VEHA_API Window3D : public Renderer
{
public:
	Window3D();
	virtual ~Window3D();

	virtual bool getDecoration() const=0;
	virtual void setDecoration(bool deco)=0;

	virtual bool getMouseCapture() const=0;
	virtual void setMouseCapture(bool captureMouse)=0;

	virtual void enableSimpleInteractor(bool simpleInteractor)=0;

        virtual void showOperations(shared_ptr<VEHA::Entity> entity,
				    shared_ptr<VEHA::Environment> environment) const=0;

        virtual void showSignals(shared_ptr<VEHA::Entity> entity,
				 shared_ptr<VEHA::Environment> environment) const=0;

        //virtual void sendSignal(shared_ptr<VEHA::Entity> entity, string signalName, map<string, shared_ptr<ValueSpecification> > values);

	template<class T1,class T2>
        boost::signals::connection addCallbackActionDone(shared_ptr<T1> obj, void (T2::*method)(shared_ptr<Action>));
	
	virtual void hide()=0;

  inline shared_ptr<Interaction> getInteraction();
  inline void setInteraction(shared_ptr<Interaction> interaction);

	void addPeripheric(shared_ptr<Peripheric> periph);
	shared_ptr<Peripheric> getPeripheric(const string& periph);
	vector<shared_ptr<Peripheric> > getPeripherics();

	void addCursor(shared_ptr<Cursor> c);
	void removeCursor(shared_ptr<Cursor> c);
	shared_ptr<Cursor> getCursor(const string& name);
	vector<shared_ptr<Cursor> > getCursors();

protected:
	boost::signal<void (shared_ptr<Action>)> _cbActionDone;
	vector<shared_ptr<Peripheric> > _peripherics;
	vector<shared_ptr<Cursor> > _cursors;
	shared_ptr<Interaction> _interaction;

};

template<class T1, class T2>
inline boost::signals::connection Window3D::addCallbackActionDone(shared_ptr<T1> obj,void (T2::*method)(shared_ptr<Action>))
{
        T2 * dummy1=(T1 *)0; // ensure T1 inherits from T2
        (void)dummy1;
        return _cbActionDone.connect(bind(method,obj,_1));
}


inline shared_ptr<Interaction> Window3D::getInteraction()
{
	return _interaction;
}
inline void Window3D::setInteraction(shared_ptr<Interaction> interaction)
{
	_interaction=interaction;
}
inline void Window3D::addPeripheric(shared_ptr<Peripheric> periph)
{
	_peripherics.push_back(periph);
}
inline shared_ptr<Peripheric> Window3D::getPeripheric(const string& periph)
{
	for(size_t i=0;i<_peripherics.size();i++)
	{
		if(_peripherics[i]->getName() == periph)
		{
			return _peripherics[i];
		}
	}
	return shared_ptr<Peripheric>();
}
inline vector<shared_ptr<Peripheric> > Window3D::getPeripherics()
{
	return _peripherics;
}
inline void Window3D::addCursor(shared_ptr<Cursor> cursor)
{
	_cursors.push_back(cursor);
}
inline void Window3D::removeCursor(shared_ptr<Cursor> cursor)
{
	size_t i=0;
	for(;i<_cursors.size();i++)
	{
		if(_cursors[i] == cursor)
		{
			break;
		}
	}
	if(i<_cursors.size())
		_cursors.erase(_cursors.begin()+i);
	
}
inline shared_ptr<Cursor> Window3D::getCursor(const string& name)
{
	for(size_t i=0;i<_cursors.size();i++)
	{
		if(_cursors[i]->getName() == name)
		{
			return _cursors[i];
		}
	}
	return shared_ptr<Cursor>();
}
inline vector<shared_ptr<Cursor> > Window3D::getCursors()
{
	return _cursors;
}
}
#endif /* LIGHT_H_ */
