#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/LoopNodeBehaviorExecution.h"
#include "VEHA/Behavior/Activity/LoopNodeBehavior.h"
#include "VEHA/Behavior/Activity/LoopNode.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ActivityBehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include <boost/bind.hpp>


namespace VEHA
{
LoopNodeBehaviorExecution::LoopNodeBehaviorExecution(
		shared_ptr<LoopNodeBehavior> loopNodeBehavior, shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync):BehaviorExecution(loopNodeBehavior,host,p,sync), _stateActivity(0), _loopNodeBehavior(loopNodeBehavior)
{

   cerr << " Loop Node Execution : " << _loopNodeBehavior->getLoopNode()->getName() << endl;

   _loopNode = _loopNodeBehavior->getLoopNode();
   _affectations = _loopNodeBehavior->_affectations;

/*
	_activity = activity;

	vector<shared_ptr<ActivityPartition> > partitions = _activity->getPartition();
	for (size_t i = 0; i < partitions.size(); i++)
	{
		if (partitions[i]->getName() == "this") _affectations["this"] = host;
		else
		{
			string pname = partitions[i]->getName();
			if (p.find(pname) != p.end())
				_affectations[pname] = (shared_dynamic_cast<InstanceValue>(p.find(pname)->second))->getValue();
			else
			{
				map<string,shared_ptr<Slot> > slots = host->getSlots();
				map<string,shared_ptr<Slot> >::iterator it = slots.find(pname);
				if (it != slots.end())
					_affectations[pname] = *host->getProperty(pname)->getValue();
				else
					cerr << "Affectation Partition de " << pname << " impossible ..." << endl;
			}
		}
		vector< shared_ptr<ActivityNode> > nodes = partitions[i]->getNode();
		for (size_t iNode = 0; iNode<nodes.size(); iNode++)
		{
			if (nodes[iNode]->getKind() == "object")
			{
				string pname = nodes[iNode]->getName();
				if (p.find(pname) != p.end())
					_affectations[pname] = (shared_dynamic_cast<InstanceValue>(p.find(pname)->second))->getValue();
				else
				{
					map<string,shared_ptr<Slot> > slots = host->getSlots();
					map<string,shared_ptr<Slot> >::iterator it = slots.find(pname);
					if (it != slots.end())
                                        {
						_affectations[pname] = *host->getProperty(pname)->getValue();
                                        }
					else
						cerr << "Affectation Object de " << pname << " impossible ..." << endl;
				}
			}
			else
			{
				if (nodes[iNode]->getKind() == "action")
				{
					shared_ptr<Action> action = (shared_dynamic_cast<ActionNode>(nodes[iNode]))->getAction();
					if (action->getKind() == "SendSignal")
					{
						shared_ptr<SendSignalAction> sendAct = (shared_dynamic_cast<SendSignalAction>(action));
						string pname = sendAct->getTargetName();
						if(pname != "")
						{
							if (p.find(pname) != p.end())
							{
								_affectations[pname] = (shared_dynamic_cast<InstanceValue>(p.find(pname)->second))->getValue();
								sendAct->setTarget((shared_dynamic_cast<InstanceValue>(p.find(pname)->second))->getValue());
							}
							else
							{
								map<string,shared_ptr<Slot> > slots = host->getSlots();
								map<string,shared_ptr<Slot> >::iterator it = slots.find(pname);
								if (it != slots.end())
								{
									_affectations[pname] = *host->getProperty(pname)->getValue();
									sendAct->setTarget(*host->getProperty(pname)->getValue());
								}
								else
									cerr << "Affectation SendAction de " << pname << " impossible ..." << endl;
							}
						}
					}
				}
			}
		}
	}
*/
}

LoopNodeBehaviorExecution::~LoopNodeBehaviorExecution()
{
}

double LoopNodeBehaviorExecution::execute(double dt)
{

	return 0.0; //Functionality deprecated. New algorithm is in ProcedureExecution.cpp. TODO: Clean this up later...


 //  if (_loopNode->eval() ) return 0;
 //  else return 0.01;

/*
   if (getSync()) 
   {
		shared_ptr<Activity> activity = getActivity();
		shared_ptr<ActivityNode> initial = activity->getInitialNode();
		vector < shared_ptr<ActionNode> > actions ;

		if (initial)
		{
			actions = initial->getOutgoingActionNode();
                }
                else return 0;

                while (actions.size())
                {
			for (size_t iA = 0; iA < actions.size(); iA++)
			{
				if (actions[iA]->getKind() == "action")
				{
					//cerr << "Running : " << actions[iA]->getName() << endl;
					string roleName = actions[iA]->getPartition()[0]->getName();
					(shared_dynamic_cast<ActionNode>(actions[iA]))->start(_affectations[roleName], _affectations, getSync());
				}
			}
			vector< shared_ptr<ActionNode> > next;
			for (size_t i = 0; i<_actions.size(); i++)
			{
				vector<shared_ptr<ActionNode> > tmp = _actions[i]->getOutgoingActionNode();
				for (size_t j = 0; j < tmp.size() ; j++)
					next.push_back(tmp[j]);
			}
                        actions = next;
		}
   }
   else 
*/
   {
	if (_stateActivity == 0)
	{
		//shared_ptr<Activity> activity = getActivity();
		shared_ptr<ActivityNode> initial = _loopNode->getInitialNode();

		if (initial)
		{
			vector < shared_ptr<ActionNode> > actions = initial->getOutgoingActionNode();
			for (size_t iA = 0; iA < actions.size(); iA++)
			{
				if (actions[iA]->getKind() == "action")
				{
					cerr << "Running : " << actions[iA]->getName() << endl;
					string roleName = actions[iA]->getPartition()[0]->getName();
                                        cerr << " RoleName : " << roleName << endl;
					(shared_dynamic_cast<ActionNode>(actions[iA]))->start(_affectations[roleName], _affectations);
				}
				_execs.push_back(actions[iA]);
				_actions.push_back(actions[iA]);
			}
			_stateActivity ++;
		}
	}
	if (_stateActivity == 1)
	{
		if (_execs.size())
		{
			size_t nbAction = _execs.size();
			for (size_t i = 0; i < _execs.size(); i++)
			{
				if (!_execs[i]->isRunning()) nbAction--;
			}
			if (!nbAction) _execs.clear();
		}
		else
		{
			if(_stateActivity == 1)
			{
				vector< shared_ptr<ActionNode> > next;
				for (size_t i = 0; i<_actions.size(); i++)
				{
					vector<shared_ptr<ActionNode> > tmp = _actions[i]->getOutgoingActionNode();
					for (size_t j = 0; j < tmp.size() ; j++)
                                        {
                                                cerr << tmp[i]->getName() << endl;
						next.push_back(tmp[j]);
                                        }
				}
				_actions.clear();
				if (next.size())
				{
					for( size_t iAction = 0; iAction < next.size(); iAction++)
					{
						if (next[iAction]->getKind() == "action")
						{
							cerr << "Running : " << next[iAction]->getName() << endl;
							string roleName = next[iAction]->getPartition()[0]->getName();
							(shared_dynamic_cast<ActionNode>(next[iAction]))->start(_affectations[roleName], _affectations);
						}
						//next[iAction]->start(_affects[next[iAction]->getPartition()->getName()],_affects);
						_execs.push_back(next[iAction]);
						_actions.push_back(next[iAction]);
					}
				}
				else
				{
					_stateActivity ++;
				}
			}
		}
	}
	else
	{
           if (_loopNode->eval() )
           {
		//_affects.clear();
		_execs.clear();
		_actions.clear();
		return 0;
           }
           else
             _stateActivity = 0;
	}
   }
   return 0.01;
}
} // Namespace
