#include "VEHA/Behavior/Common/ActivityBehaviorExecutionHelper.h"

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"

namespace VEHA
{

void ActivityBehaviorExecutionHelper::_buildAffectationsObject(vector<string> & missing, shared_ptr<ActivityNode> node)
{
    // TODO
}

void ActivityBehaviorExecutionHelper::_buildAffectationsAction(vector<string> & missing, shared_ptr<ActivityNode> node)
{
    // TODO
}

void ActivityBehaviorExecutionHelper::_buildAffectations(shared_ptr<InstanceSpecification> host, shared_ptr<Activity> activity)
{
	vector<string> missing;

	cerr << "[ActivityBehaviorExecutionHelper Info] Affectations." << endl;

	/* Affectation de self */
	_affectations["self"] = host;

	/* Affectation des ressources */
	vector<shared_ptr<ActivityPartition> > partitions = activity->getPartition();
	for (size_t i = 0; i < partitions.size(); i++)
	{
		if (partitions[i]->getName() == "this")
		{
			_affectations["this"] = host;
		}
		else
		{
			string pname = partitions[i]->getName();
		}

		vector< shared_ptr<ActivityNode> > nodes = partitions[i]->getNode();
		cerr << " SIZE : " << nodes.size() << endl;
		for (size_t iNode = 0; iNode<nodes.size(); iNode++)
		{
			shared_ptr<ActivityNode> node = nodes[iNode];
			if (node->getKind() == "object")
			{
				_buildAffectationsObject(missing, node);
			}
			else if (node->getKind() == "action")
			{
				_buildAffectationsObject(missing, node);
			}
		}
	}

	if (missing.size())
	{
		_buildAffectationsPostbone(missing);
	}

	cerr << "[ActivityBehaviorExecutionHelper Info] Fin affectations." << endl;

	BehaviorExecution * be = dynamic_cast<BehaviorExecution*>(this);
	activity->setContext(_buildOclContext(be->parameters));
}

OclParser::Context ActivityBehaviorExecutionHelper::_buildOclContext(Parameters params)
{
	/* Construction du contexte des constraintes */
	OclParser::Context context;

	map<string, shared_ptr<InstanceSpecification> >::iterator itAffect;
	for(itAffect=_affectations.begin();itAffect!=_affectations.end();itAffect++)
	{
		context[itAffect->first]=itAffect->second;
	}

	Parameters::iterator itParams = params.begin();
	for(itParams=params.begin();itParams!=params.end();itParams++)
	{
		context[itParams->first]=AnyValue(itParams->second->getStringFromValue());
	}

	return context;
}

}
