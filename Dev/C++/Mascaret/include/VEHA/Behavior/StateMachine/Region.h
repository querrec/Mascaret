#ifndef _v2_VEHA_Behavior_StateMachine_Region_H
#define _v2_VEHA_Behavior_StateMachine_Region_H

#include "Tools/veha_plateform.h"

namespace VEHA
{
class Vertex;
class Transition;

class VEHA_API Region
{
public :
	Region();
	virtual ~Region();

	// --- Attributes ---

	// --- Relations ---
	//------ _Transition -----
protected :
	vector< shared_ptr<Transition> >    _transition;
public :
	inline vector< shared_ptr<Transition> > getTransition(void) { return _transition;}
	inline shared_ptr<Transition> getTransition_at(int index) { return _transition[index];}
	inline void addTransition(shared_ptr<Transition> value) { _transition.push_back(value);}
	//------ _vertices -----
protected :
	vector< shared_ptr<Vertex> >    _vertices;
public :
	inline vector< shared_ptr<Vertex> > getVertices(void) { return _vertices;}
	inline shared_ptr<Vertex> getVertices_at(int index) { return _vertices[index];}
	inline void addVertices(shared_ptr<Vertex> value) { _vertices.push_back(value);}
	// --- Operations ---
};

}
#endif
