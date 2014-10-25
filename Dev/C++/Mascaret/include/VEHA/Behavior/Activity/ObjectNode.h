#ifndef _v2_VEHA_Behavior_Activity_ObjectNode_H
#define _v2_VEHA_Behavior_Activity_ObjectNode_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/ValueSpecification.h"

namespace VEHA
{
class VEHA_API ObjectNode : public ActivityNode
{
public :
	ObjectNode(const string& name);
	virtual ~ObjectNode();

	inline bool isInParticularState(void) {return _inParticularState;}
	inline void setInParticularState(bool value) {_inParticularState = value;}

	inline string getState(void) {return _state;}
	inline void setState(string value) {_state = value;}

	inline shared_ptr<Classifier> getResourceType(void) {return _resourceType;}
	inline void setResourceType(shared_ptr<Classifier> value) {_resourceType = value;}

	inline shared_ptr<ValueSpecification> getResourceValue(void) {return _value;}
	inline void setResourceValue(shared_ptr<ValueSpecification> v) {_value = v;}

protected:
	bool _inParticularState;
	string _state;
	shared_ptr<Classifier> _resourceType;
	shared_ptr<ValueSpecification> _value;
};

}
#endif
