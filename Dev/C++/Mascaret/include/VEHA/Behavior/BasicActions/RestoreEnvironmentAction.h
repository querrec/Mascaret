/*
 * SetInRedAction.h
 *
 *  Created on: 18 nov. 2010
 *      Author: soler
 */

#ifndef RestoreEnvironmentAction_H_
#define RestoreEnvironmentAction_H_

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{
class Entity;
class EntityClass;
class Signal;

class VEHA_API RestoreEnvironmentAction : public Action
{
public :
	RestoreEnvironmentAction();
	virtual ~RestoreEnvironmentAction();

protected :

public :

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);

};

}


#endif /* SETINREDACTION_H_ */
