#include "VEHA/Behavior/Activity/ActivityExecution.h"

namespace VEHA
{

ActivityExecution::ActivityExecution(const shared_ptr<Activity> act) :
		_activity(act)
{
	_toExecuteNodes = act->getInitialNode()->getOutgoingActionNode();
}

ActivityExecution::~ActivityExecution()
{
}

vector<shared_ptr<ActionNode> > ActivityExecution::getNextActions(const string& roleName)
{
	vector<shared_ptr<ActionNode> > toExec;
	updateNodes();
	for(size_t i=0;i<_toExecuteNodes.size();i++)
	{
		if(_toExecuteNodes[i]->getPartition_at(0)->getName() == roleName)
		{
			toExec.push_back(_toExecuteNodes[i]);
		}
	}
	return toExec;
}
string clean(const string& toClean)
{
	string returned="";
	for(size_t i=0;i<toClean.size();i++)
	{
		if(toClean[i] != '/' && toClean[i] != '(' && toClean[i] != ')' && toClean[i] != '^')
			returned+=toClean[i];
	}
	return returned;
}

void ActivityExecution::printState()
{
	cerr << "Noeud a executer:"<<endl;
	vector<shared_ptr<ActionNode> >::iterator it=_toExecuteNodes.begin();
	while(it != _toExecuteNodes.end())
	{
		cerr << "--> "<<(*it)->getName() << " par "<< (*it)->getPartition_at(0)->getName() << endl;
		it++;
	}
	cerr << "Noeud en cours d'execution:"<<endl;
	it=_runningNodes.begin();
	while(it != _runningNodes.end())
	{
		cerr << "--> " << (*it)->getName() << " par "<< (*it)->getPartition_at(0)->getName() << endl;
		it++;
	}
	cerr << "Evenement running recu:"<<endl;
	vector<pair<string,string> >::iterator it2=_actionsRunning.begin();
	while(it2 != _actionsRunning.end())
	{
		cerr << "--> "<<it2->first << " running " << it2->second << endl;
		it2++;
	}
	cerr << "Evenement done recu:"<<endl;
	it2=_actionsDone.begin();
	while(it2 != _actionsDone.end())
	{
		cerr << "--> "<<it2->first << " have done "<<it2->second << endl;
		it2++;
	}

}
template <typename T> inline
void
StlVectorFastErase(vector<T> & vect,
		unsigned int index)
{
	vect[index]=vect.back();
	vect.pop_back();
}
void ActivityExecution::updateNodes()
{
	for(size_t i=0;i<_actionsRunning.size();)
	{
		bool increase=true;
		for(size_t j=0;j<_toExecuteNodes.size();)
		{
			if(_actionsRunning[i].first == _toExecuteNodes[j]->getPartition_at(0)->getName() &&
					_actionsRunning[i].second == clean(_toExecuteNodes[j]->getName()))
			{
				//cerr << _toExecuteNodes[j]->getName() << " en cours d'execution"<<endl;
				_runningNodes.push_back(_toExecuteNodes[j]);
				StlVectorFastErase(_toExecuteNodes,j);
				StlVectorFastErase(_actionsRunning,i);
				increase=false;
				break;
			}
			else
			{
				j++;
			}
		}
		if(increase)
			i++;
	}

	for(size_t i=0;i<_actionsDone.size();)
	{
		bool increase=true;
		for(size_t j=0;j<_runningNodes.size();)
		{
			if(_actionsDone[i].first == _runningNodes[j]->getPartition_at(0)->getName() &&
					_actionsDone[i].second == clean(_runningNodes[j]->getName()))
			{
				//cerr << _runningNodes[j]->getName() << " terminé"<<endl;
				//cerr << "find next of "<< _runningNodes[j]->getName() <<endl;
				vector< shared_ptr < ActionNode> > next = _runningNodes[j]->getOutgoingActionNode();
				for (size_t iN = 0; iN < next.size() ; iN++)
				{
					//cerr << _runningNodes[j]->getName()<<" nextNode: "<<next[iN]->getName()<<endl;
					_toExecuteNodes.push_back(next[iN]);
				}
				StlVectorFastErase(_runningNodes,j);
				StlVectorFastErase(_actionsDone,i);
				break;
			}
			else
			{
				j++;
			}
		}
		if(increase)
			i++;
	}
}

}
