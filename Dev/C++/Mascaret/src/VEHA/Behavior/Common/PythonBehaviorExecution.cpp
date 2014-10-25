
#include "VEHA/Behavior/Common/PythonBehaviorExecution.h"
#include "VEHA/Behavior/Common/OpaquePythonBehavior.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "Tools/utils.h"

#include "boost/python.hpp"

namespace bp = boost::python;

namespace VEHA
{

PythonBehaviorExecution::PythonBehaviorExecution(shared_ptr<OpaquePythonBehavior>  behavior,shared_ptr<InstanceSpecification> host, const Parameters& p)
:BehaviorExecution(behavior,host,p)
{
	PyObject * obj;
	string code=behavior->getCode();
	if (!Py_IsInitialized())
		Py_Initialize();
	string modName("BehaviorExecution");
	cerr<<"modName="<<modName<<endl;
	__main__ = PyModule_New(modName.c_str());
	PyObject * main_dict=PyModule_GetDict(PyImport_AddModule("__main__"));
	PyModule_AddObject(__main__,"__builtins__",PyDict_GetItemString(main_dict,"__builtins__"));
	cerr<<"add ok"<<endl;
	_scope = PyObject_GetAttrString(__main__, "__dict__");
	cerr<<"import vehaV2"<<endl;

	string putPythonPath="import sys\nsys.path.append('";
	putPythonPath+=pythonLibPath+"')\n";
	if (!(obj=PyRun_String(putPythonPath.c_str(),Py_file_input,_scope,_scope)))
		PyErr_Print();
	else
		Py_DECREF(obj);
	if (!(obj=PyRun_String("from vehaV2 import *\n",Py_file_input,_scope,_scope)))
		PyErr_Print();
	else
		Py_DECREF(obj);
	PyErr_Clear();
	bp::dict d=bp::extract<bp::dict>(_scope);
	PyErr_Print();
	d[string("parameters")]=p;
	d[string("host")]=host;
	if(!(obj=PyRun_String(code.c_str(),Py_file_input,_scope,_scope)))
		PyErr_Print();
	else Py_DECREF(obj);
	PyErr_Clear();

}

PythonBehaviorExecution::~PythonBehaviorExecution()
{
	Py_DECREF(__main__);
	Py_DECREF(_scope);
}

double PythonBehaviorExecution::execute(double dt)
{
	cerr<<"exec:"<<endl;
	PyObject * obj;
	double result=-1;
	if (!(obj=PyRun_String("execute()\n",Py_file_input,_scope,_scope)))
	{
		PyErr_Print();
		cerr<<"No result for  execute()"<<endl;
	}
	else
	{
		if (PyFloat_Check(obj))
			cerr<<"Result is not a float object"<<endl;
		else result=PyFloat_AsDouble(obj);
		Py_DECREF(obj);
	}
	PyErr_Clear();
	return result;

}

}
