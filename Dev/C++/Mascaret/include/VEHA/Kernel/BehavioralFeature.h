#ifndef _v2_VEHA_Kernel_BehavioralFeature_H
#define _v2_VEHA_Kernel_BehavioralFeature_H
#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Feature.h"

namespace VEHA
{
class Behavior;
class InstanceSpecification;

class VEHA_API BehavioralFeature : public Feature
{
public :
	BehavioralFeature(const string& name,shared_ptr<Behavior> behavior);
	virtual ~BehavioralFeature();
	// --- Attributes ---
	// --- Relations ---
protected :
	shared_ptr<Behavior> _method;
public :
	inline shared_ptr<Behavior> getMethod(){return _method;}
	inline void setMethod(shared_ptr<Behavior> value) {_method = value;}

	// --- Operations ---
};

}
#endif
