#ifndef _v2_VEHA_Behavior_BasicActions_CreateEntityAction_H
#define _v2_VEHA_Behavior_BasicActions_CreateEntityAction_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{
class Entity;
class EntityClass;
class Signal;

class VEHA_API CreateEntityAction : public Action
{
public :
	CreateEntityAction();
	virtual ~CreateEntityAction();

protected :
	shared_ptr<EntityClass>    _classifier;
	string    _name;
public :
	inline shared_ptr<EntityClass> getClassifier(void) { return _classifier;}
	inline void setClassifier(shared_ptr<EntityClass> classifier) { _classifier = classifier;}

	inline string getName(void) { return _name;}
	inline void setName(string name) { _name = name;}

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&);

};

}
#endif
