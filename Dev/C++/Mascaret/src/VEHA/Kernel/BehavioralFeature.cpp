#include "VEHA/Kernel/BehavioralFeature.h"

namespace VEHA
{

BehavioralFeature::BehavioralFeature(const string& name,shared_ptr<Behavior> behavior)
:Feature(name),_method(behavior)
{
}

BehavioralFeature::~BehavioralFeature()
{
}

}
