#ifndef _v2_VEHA_Behavior_Activity_LoopNode_H
#define _v2_VEHA_Behavior_Activity_LoopNode_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Expression.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"


namespace VEHA
{
class VEHA_API LoopNode : public ActionNode
{
public :
	LoopNode(const string& name, const string& kind = "loop");
	virtual ~LoopNode();

protected :
	vector< shared_ptr<ActivityNode> >  _node;
	vector< shared_ptr<ActivityEdge> >  _edges;

	shared_ptr<ActivityNode> _initial;

    shared_ptr<Expression> _condition;
    
    bool _testedFirst;

public :
	inline vector< shared_ptr<ActivityNode> > getNode(void) { return _node;}
	inline shared_ptr<ActivityNode> getNode_at(int index) { return _node[index];}
	inline void addNode(shared_ptr<ActivityNode> value)
	{
		_node.push_back(value);
		value->setActivity(getActivity());
	}

	inline vector< shared_ptr<ActivityEdge> > getEdge(void) { return _edges;}
	inline shared_ptr<ActivityEdge> getEdge_at(int index) { return _edges[index];}
	inline void addEdge(shared_ptr<ActivityEdge> value)
	{
		_edges.push_back(value);
		value->setActivity(getActivity());
	}
	
	inline void setTestedFirst(bool tf) { _testedFirst = tf; }
	inline bool getTestedFirst() { return _testedFirst; }
	
	inline void setCondition(shared_ptr<Expression> condition) {_condition = condition;}
	inline shared_ptr<Expression> getCondition(void) { return _condition;}

	inline void setInitialNode(shared_ptr<ActivityNode> initial) {_initial = initial;}
	inline shared_ptr<ActivityNode> getInitialNode(void) { return _initial;}

    virtual void start(shared_ptr<InstanceSpecification> host, map < string, shared_ptr<InstanceSpecification> > affectations, bool sync = false);
    void stop(void);
    virtual inline bool isRunning()
    {
       return false;
    }

    bool eval(void);
};

}
#endif
