#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ObjectNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/BasicActions/SetInstanceInRedAction.h"
#include "VEHA/Behavior/BasicActions/SetInstanceTransparencyAction.h"
#include "VEHA/Behavior/BasicActions/GrowUpInstanceAction.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"


namespace VEHA
{

ActionNode::ActionNode(const string& name, const string & kind) : ActivityNode(name,kind), _action()
{
}

ActionNode::~ActionNode()
{
}

Parameters ActionNode::_paramsSendSignal(map<string, shared_ptr<InstanceSpecification> > affectations)
{
	shared_ptr<SendSignalAction> action = shared_dynamic_cast<SendSignalAction>(_action);

//	if (!(shared_dynamic_cast<SendSignalAction>(_action))->getTarget()) // FIXME
	{
		vector< shared_ptr<ActivityEdge> > edges = getOutgoing();
		for (size_t i = 0; i < edges.size() ; i++)
		{
			if (edges[i]->getTarget()->getKind() == "object")
			{
				map<string, shared_ptr<InstanceSpecification> >::iterator it = affectations.find(edges[i]->getTarget()->getName());
				if (it != affectations.end())
				{
					action->setTarget(it->second);
				}
				else
				{
					cerr << "affectation of " << edges[i]->getTarget()->getName() << "not found"<<endl;
					cerr << "affectations:"<<endl;
					map<string, shared_ptr<InstanceSpecification> >::iterator it = affectations.begin();
					while(it != affectations.end())
					{
						cerr << "  "<<it->first<<"="<<it->second->getName()<<endl;
						it++;
					}
				}
			}
		}
	}

	return Parameters();
}

Parameters ActionNode::_paramsCallOperation(map<string, shared_ptr<InstanceSpecification> > affectations)
{
	Parameters param;

	shared_ptr<CallOperationAction> action = shared_dynamic_cast<CallOperationAction>(_action);

	cerr << " Executing Call Operation : " << action->getName() << " " << action->getOperation()->getFullName() << endl;
	if(action->isDynamic())
	{
		return action->getParameters();
	}
	else
	{
		vector<shared_ptr<InputPin> > inputPins = _action->getInputPin() ;
		cerr << " PINS : " << inputPins.size() << endl;
		for (unsigned int i = 0; i < inputPins.size() ; i++)
		{
			shared_ptr<InputPin> pin = inputPins[i];
			vector< shared_ptr<ObjectNode> > nodes = pin->getIncomingObjectNode();

			// Test de validité
			if(!nodes.size()) // TODO: Found a way to test it in ModelLoader.
			{
				cerr << "[ActionNode Warning] No ObjectNode connected to pin "
						<< pin->getName() << " of " << getFullName() << endl;
			}
			if(nodes[0]->getResourceType())
			{
				if(!nodes[0]->getResourceType()->isA(pin->getResourceType())) // TODO: Found a way to test it in ModelLoader.
				{
					cerr << "[ActionNode Warning] Type error for ObjectNode " << nodes[0]->getName()
							<< " connected to pin " << pin->getName()
							<< " of " << getFullName()
							<< " type " << nodes[0]->getResourceType()->getName()
							<< " need type " << pin->getResourceType()->getName()
							<< endl;
				}
			}
			else
			{
				// TODO: Seriously ? This should have never ever happen... Why does this nodes have no type ?
				cerr << "[ActionNode Warning] Type of ObjectNode " << nodes[0]->getName()
						<< " connected to pin " << pin->getName()
						<< " of " << getFullName()
						<< " is unknown" << endl;
			}
			cerr << affectations[nodes[0]->getName()] << endl;
			param[pin->getName()] = make_shared<InstanceValue>(affectations[nodes[0]->getName()]);
		}

#if 0
		/* Code permettant la gestion de l'ancien mode de passage de parametres
		 * Si on ne detecte aucune pin on cherche des ObjectNode directement rélies
		 * en sortie.
		 */
		if(inputPins.size() == 0)
		{
			vector<shared_ptr<ObjectNode> > objects=getOutgoingObjectNode();
			for(size_t i=0;i<objects.size();i++) // TODO: gerer l'ordre et le nombre de parametres
			{
				cerr << "[ActionNode Warning] Passage de parametres par Outgoing Object Node" << endl;
				// On considere que l'ordre des objets affecté au noeud action est le même que l'ordre des paramètres
				// On pourrait ajouté un controle du type...
				cerr << objects[i]->getName() << "=" << affectations[objects[i]->getName()] << endl;
				string paramName = "";
				vector< shared_ptr<Parameter> > opParams = action->getOperation()->getParameter();
				cerr << action->getOperation()->getName() << " PARAMS SIZE : " << opParams.size() << " == " << i << endl;
				if (opParams.size() > i)
				{
#if 0
					paramName=action->getOperation()->getParameter()[i]->getName();
					cerr << paramName << "=" << affectations[objects[i]->getName()]->getName() << endl;
					param[paramName]=shared_ptr<ValueSpecification>(new InstanceValue(affectations[objects[i]->getName()]));
#else
					paramName=action->getOperation()->getParameter()[i]->getName();
					cerr << paramName << "=" << affectations[paramName]->getName() << endl;
					param[paramName]=shared_ptr<ValueSpecification>(new InstanceValue(affectations[paramName]));
#endif
				}
			}
		}
#endif
	}

	return param;
}

Parameters ActionNode::_paramsCallBehavior(map<string, shared_ptr<InstanceSpecification> > affectations)
{
	// TODO: Support parameters CallBehavior.
	cerr << "[ActionNode Warning] Parameters not support on CallBehavior" << endl;
	return Parameters();
}

void ActionNode::start(shared_ptr<InstanceSpecification> host, map<string, shared_ptr<InstanceSpecification> > affectations, bool sync)
{
	if(!_action)
	{
		cerr << "[ActionNode Warning] Le noeud "<< getName() << " n'a pas d'action associée !!!" << endl;
		return;
	}
	Parameters param;
	if (_action->getKind() == "SendSignal")
	{
		param = _paramsSendSignal(affectations);
	}
	else if (_action->getKind() == "CallOperation")
	{
		param = _paramsCallOperation(affectations);
	}
	else if (_action->getKind() == "CallBehavior")
	{
		param = _paramsCallBehavior(affectations);
	}

	{
		cerr << "Action Kind: " << _action->getKind() << endl;
	}
	
	if (getActivity())
	{
		_action->setContext(getActivity()->getContext());
	}

	_currentExecution = BehaviorScheduler::getInstance()->executeBehavior(_action, host, param,sync);
	//_currentExecution=_action->createBehaviorExecution(host,param);
	//_currentExecution->execute(); // a basic action is a one shot action
	//_currentExecution->stop(); //to say the job is finished (because not done by BehaviorScheduler)
}

void ActionNode::stop()
{
   if (_currentExecution) _currentExecution->stop() ;
}

}
