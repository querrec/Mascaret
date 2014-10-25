/*
 * SetInRedAction.h
 *
 *  Created on: 18 nov. 2010
 *      Author: soler
 */

#ifndef SETINREDACTION_H_
#define SETINREDACTION_H_

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{
class Entity;
class EntityClass;
class Signal;

class VEHA_API SetInstanceInRedAction : public Action
{
public :
	SetInstanceInRedAction();
	virtual ~SetInstanceInRedAction();

protected :
	shared_ptr<InstanceSpecification>  _target;
public :
	inline shared_ptr<InstanceSpecification> getTarget() { return _target;}
	inline void setTarget(shared_ptr<InstanceSpecification> target) { _target=target;}

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);

};

}


#endif /* SETINREDACTION_H_ */
