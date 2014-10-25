#ifndef _v2_VEHA_Behavior_Common_PythonBehaviorExecution_H
#define _v2_VEHA_Behavior_Common_PythonBehaviorExecution_H
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include <Python.h>
using namespace std;

namespace VEHA
{
class Behavior;
class TimeExpression;
class InstanceSpecification;
class OpaquePythonBehavior;

class VEHA_API PythonBehaviorExecution: public BehaviorExecution
{
public :
	PythonBehaviorExecution(shared_ptr<OpaquePythonBehavior>  behavior,shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~PythonBehaviorExecution();
	//------ specification -----
	virtual double execute(double dt);
protected:
	PyObject * __main__,*_scope;
};

}
#endif
