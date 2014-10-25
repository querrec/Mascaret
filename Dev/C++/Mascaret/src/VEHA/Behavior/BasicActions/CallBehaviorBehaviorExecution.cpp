#include "VEHA/Behavior/BasicActions/CallBehaviorBehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Operation.h"
#include <boost/bind.hpp>

#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/RoleAssignement.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ActivityParameterNode.h"

using BEHAVE::AID;
using BEHAVE::ACLMessage;
using BEHAVE::OrganisationalEntity;
using BEHAVE::OrganisationalStructure;
using BEHAVE::Procedure;
using BEHAVE::RoleAssignement;

namespace VEHA
{

CallBehaviorBehaviorExecution::CallBehaviorBehaviorExecution(shared_ptr<CallBehaviorAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action), _started(false), _lastCheckedMailIndex(0)
{
	//_behaviorExecution=_action->getBehavior()->createBehaviorExecution(getHost(),parameters);

}

CallBehaviorBehaviorExecution::~CallBehaviorBehaviorExecution()
{
}
void CallBehaviorBehaviorExecution::stop()
{
	BehaviorExecution::stop();
	//_behaviorExecution->stop();
}
void CallBehaviorBehaviorExecution::restart()
{
	BehaviorExecution::restart();
	//_behaviorExecution->restart();
}
void CallBehaviorBehaviorExecution::pause()
{
	BehaviorExecution::pause();
	//_behaviorExecution->pause();
}

double CallBehaviorBehaviorExecution::execute(double dt)
{
	//return _behaviorExecution->execute();
	if (!_started)
	{
		assert(_action);
		assert(_action->getBehavior());
		string procName = _action->getBehavior()->getName();
	
		vector <shared_ptr<OrganisationalEntity> > orgs = MascaretApplication::getInstance()->getAgentPlateform()->getOrganisationalEntity();
		
		bool found = false;
		
		for (size_t iO=0; iO<orgs.size(); ++iO)
		{
			shared_ptr<OrganisationalEntity> org = orgs[iO];
		
			shared_ptr<OrganisationalStructure> struc = org->getStructure();
			vector < shared_ptr<Procedure> > procs = struc->getProcedures();
		
			shared_ptr<Procedure> proc;
			
			int i = 0;
		
			while (!found && (unsigned)i < procs.size())
			{
				if (procs[i]->getName() == procName)
				{
					found = true;
				}
				else
				{
					i++;
				}
			}
	
			if (found)
			{
				proc = procs[i];
			}
			else
			{
				continue; //try next organisation
			}

			shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();

			vector<shared_ptr<Parameter> > parameters ;
			vector< shared_ptr<ActivityNode> > nodes  = proc->getActivity()->getNode();
			for (unsigned int iNode = 0; iNode < nodes.size(); iNode ++)
			{
				if (nodes[iNode]->getKind() == "parameter")
				{
					parameters.push_back((shared_dynamic_cast<ActivityParameterNode>(nodes[iNode]))->getParameter());
				}
			}

			Parameters param;
			string paramString = "";
	
			for (size_t i=parameters.size();i--;)
			{
				//TODO TODO parameters
				/*
				string strVal=req->getParameter(string("_")+parameters[i]->getName());
				for (unsigned int iC = 0; iC < strVal.size(); iC++)
				{
					if (strVal[iC] == '+') strVal[iC] = ' ';
				}
		
				cerr << " ----- > " << strVal << endl;
			
				param[parameters[i]->getName()]=parameters[i]->getType()->createValueFromString(strVal);
				paramString += " :" + parameters[i]->getName() + " " + strVal;
				*/
			}
			
			/*
			//set timestamp //TODO later...
			struct timeval tv;
			struct timezone tz;
			gettimeofday(&tv,&tz);
			double currentTime=((double)tv.tv_sec)+(((double)tv.tv_usec)*1e-6);
			_startTimestamp = currentTime;
			*/
			
			vector< shared_ptr<RoleAssignement> > assignements = org->getRoleAssignements();
			for (size_t iA = 0; iA < assignements.size() ; iA++)
			{
				shared_ptr<ACLMessage> procMsg = shared_ptr<ACLMessage>(new ACLMessage(BEHAVE::REQUEST));
				procMsg->addReceiver(assignements[iA]->getAgent());

				//procMsg->setSender(_agent->getAID());
				cout<<"CallBehaviorBehaviorExecution SENDING MESSAGE TO " << assignements[iA]->getAgent()->getName() << endl;
				shared_ptr<AID> agentAID = MascaretApplication::getInstance()->agent->getAID();
				procMsg->setSender(agentAID);
				string content ="((action ";
				content += assignements[iA]->getAgent()->getName();
				content += " ";
				content += "(" + proc->getName();
				content += paramString;
				content += ")))";
				cout <<"\t\t\t"<< content << endl;
				procMsg->setContent(content);
				
				//procMsg->setTimestamp(_startTimestamp);
				
				MascaretApplication::getInstance()->getAgentPlateform()->sendMessage(procMsg);
				
				
				//build message to receive
				string msgToReceive ="((done (action ";
				msgToReceive += assignements[iA]->getAgent()->getName();
				msgToReceive += " ";
				msgToReceive += "(" + proc->getName();
				//msgToReceive += paramString; //no params right now TODO TODO
				msgToReceive += "))))";
				
				_mustReceiveTheseMessagesToFinish.push_back(msgToReceive);
			}
		}
		
		if (found)
		{
			_started = true;
			return 0.1;
		}
		else
		{
			return 0.0;
		}
	}
	else
	{
		shared_ptr<Agent> agent = MascaretApplication::getInstance()->agent;
		
		while (agent->receive()) {;}
		
		vector<shared_ptr<ACLMessage> > messages = agent->getChecked();
		
		for (size_t i=_lastCheckedMailIndex; i<messages.size(); ++i)
		{
			for (size_t j=0; j<_mustReceiveTheseMessagesToFinish.size(); ++j)
			{
				if (messages[i]->getContent() == _mustReceiveTheseMessagesToFinish[j])
				{
					_mustReceiveTheseMessagesToFinish.erase(_mustReceiveTheseMessagesToFinish.begin() + j);
					break;
				}
			}
		}
		_lastCheckedMailIndex = messages.size();
		
		if (_mustReceiveTheseMessagesToFinish.size())
		{
			//still have msgs to receive
			return 0.1;
		}
	}
	
	return 0.0;
}

} // Namespace
