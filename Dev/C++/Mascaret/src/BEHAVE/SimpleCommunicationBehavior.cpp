#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/Slot.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityParameterNode.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "BEHAVE/OneShotBehaviorExecution.h"
#include "BEHAVE/CyclicBehaviorExecution.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentBehavior.h"
#include "BEHAVE/AgentPlateform.h"
#include "BEHAVE/RoleAssignement.h"
#include "BEHAVE/Ressource.h"
#include "BEHAVE/RessourceAssignement.h"
#include "BEHAVE/AID.h"
#include "BEHAVE/ACLMessage.h"
#include "BEHAVE/fipaSLParser.h"
#include "BEHAVE/SimpleCommunicationBehavior.h"
#include "BEHAVE/ProceduralBehavior.h"
#include "BEHAVE/ActionListenerBehavior.h"
#include "BEHAVE/ProcedureExecution.h"
#include "HAVE/VirtualHuman.h"
#include "Tools/Loaders/XmlParser.h"
//#include "MATS/Poseidon/LearningSession.h"
#include <boost/bind.hpp>
#include <iostream>
#include <sstream>

#include "VEHA/Behavior/BasicActions/CallOperationAction.h"

using std::stringstream;
using namespace VEHA;

namespace BEHAVE
{

SimpleCommunicationBehavior::SimpleCommunicationBehavior(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host,const Parameters& p)
: CyclicBehaviorExecution(specif, host, p),
	_callbackOnEntitiesAdded(false)
{
}

void SimpleCommunicationBehavior::action ()
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	shared_ptr<ACLMessage> msg = agt->receive();

	if (msg)
	{
		if (msg->getPerformative() == REQUEST)
		{
			_manageRequest(msg);
		}
		else if (msg->getPerformative() == INFORM)
		{
			_manageInform(msg);
		}
		else if (msg->getPerformative() == QUERY_REF)
		{
			_manageQueryRef(msg);
		}
	}
	
	if (agt->getMessageQueue().size() == 0)
	{
		pause();
	}
}

void SimpleCommunicationBehavior::_manageRequestAction(shared_ptr<ACLMessage> msg)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	char act [155];
	
	string content = msg->getContent();

	FIPASLParserResult result = parseFipaSLExpression(content.c_str());
	
	if (result.success)
	{
		if (result.isAction)
		{
			bool done = false;
			shared_ptr<AgentBehavior> behavior = agt->getBehaviorByName(result.action.actionName);
		
			if (behavior)
			{
				cerr << "Found behavior : " << behavior->getName() << endl;
				Parameters params;
				if(result.action.paramName.size())
				{
					for(size_t i=0;i<result.action.paramName.size();i++)
					{
						params[result.action.paramName[i]] = shared_ptr<ValueSpecification>(new LiteralString(result.action.paramValue[i]));
					}
				}
				else
				{
					for(size_t i=0;i<result.action.paramName.size();i++)
					{
						params[string("param"+(i+'0'))] = shared_ptr<ValueSpecification>(new LiteralString(result.action.paramValue[i]));
					}
				}
		
				BehaviorScheduler::getInstance()->executeBehavior(behavior,agt,params);
				shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(AGREE));
				reponse->setContent(msg->getContent());
				reponse->addReceiver(msg->getSender());
				agt->send(reponse);
				done=true;
			}
		
			shared_ptr<Class> classifier = agt->getClassifier();
		
			shared_ptr<Operation> operation;
			map< string,shared_ptr<Operation> > operations;
		
			if (classifier)
			{
				operations = classifier->getOperations();
			}
		
			map< string,shared_ptr<Operation> >::iterator itOp;
		
			if (classifier)
			{
				itOp = operations.find(result.action.actionName);
			}
		
			if (classifier && itOp != operations.end())
			{
				operation = itOp->second;
			}
		
			if (operation)
			{
				Parameters param;
				vector<shared_ptr<Parameter> > parameters = operation->getParameter();
				vector<string> paramValue = result.action.paramValue;
				vector<string> paramName = result.action.paramName;
			
				if (parameters.size() == paramValue.size())
				{
					for (size_t i=parameters.size();i--;)
					{
						// Pour tous les parametres issus de l'operation
						string parameterName = parameters[i]->getName();

						// Cherche l'indice de ce parameter dans paramsName
						int unsigned indice = 0; bool found=false;
						while (!found && indice<paramName.size())
						{
							if (paramName[indice] == parameterName)
							{
								found = true;
							}
							else
							{
								indice++;
							}
						}
					
						if (!found)
						{
							cerr << "[SimpleCommunicationBehavior Warning] Parsing error. Parameters parsing error for operation "
								<< operation->getFullName() << endl;
						}
						else
						{
							string strVal=paramValue[indice];
							param[parameters[i]->getName()]=parameters[i]->getType()->createValueFromString(strVal);
						}
					}
					
					vector<shared_ptr<ActionNode> > possibleTranslation = _translateOperationToActions(operation);
					
					shared_ptr<AgentBehaviorExecution> abe = agt->getBehaviorExecutionByName("ProceduralBehavior"); //hasslich... ja...
					shared_ptr<ProceduralBehavior> pbe  = shared_dynamic_cast<ProceduralBehavior> (abe);
					
					if (possibleTranslation.size() && pbe)
					{
						//add actionNode in ProceduralBehavior
						
						pbe->pushActionToDo(possibleTranslation[0]);
					}
					else
					{
						//call method
						
						shared_ptr<BehaviorExecution> be = BehaviorScheduler::getInstance()->executeBehavior(operation->getMethod(),agt,param);
				
						if (be)
						{
							be->addCallbackOnBehaviorStop(bind(&SimpleCommunicationBehavior::_onBehaviorStop,this,_1));
							
							_requestedAction[be->getSpecification()->getName()].push_back(msg->getSender());
						}
					}
					
					
					shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(AGREE));
					reponse->setContent(msg->getContent());
					reponse->addReceiver(msg->getSender());
					agt->send(reponse);
				}
				else
				{
					shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(NOT_UNDERSTOOD));
					string contentRep (act);
					contentRep += content;
					reponse->setContent(contentRep);
					reponse->addReceiver(msg->getSender());
					agt->send(reponse);
				}
			}
			else
			{
				string procName = result.action.actionName;
				bool found = false;
				shared_ptr<OrganisationalEntity> askedOrg;
				shared_ptr<Procedure> askedProc;
				shared_ptr<Role> askedRole;
				vector<shared_ptr<OrganisationalEntity> > orgs = agt->getAgentPlateform()->getOrganisationalEntity();
			
				for (size_t iOrg = 0; iOrg < orgs.size(); iOrg++)
				{
					vector< shared_ptr<RoleAssignement> > assigns = orgs[iOrg]->getRoleAssignements();
				
					for (size_t iAss = 0; iAss < assigns.size();iAss++)
					{
						if (assigns[iAss]->getAgent()->toString() == agt->getAID()->toString())
						{
							shared_ptr<OrganisationalStructure> os = orgs[iOrg]->getStructure();
							vector < shared_ptr<Procedure> > procs = os->getProcedures();
						
							for (size_t iP = 0; iP < procs.size(); iP++)
							{
								if (procs[iP]->getName() == procName)
								{
									askedProc = procs[iP];
									askedOrg = orgs[iOrg];
									askedRole = assigns[iAss]->getRole();
									found = true;
								}
							}
						}
					}
				}

				if (found)
				{
					shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(AGREE));
					reponse->setContent(msg->getContent());
					reponse->addReceiver(msg->getSender());
					agt->send(reponse);

					// Recherche du comportement procedural
					// UGGLY
					shared_ptr<AgentBehaviorExecution> behavior = agt->getBehaviorExecutionByName("ProceduralBehavior");

					shared_ptr<AgentBehaviorExecution> behavior2 = agt->getBehaviorExecutionByName("ActionListenerBehavior");
					if (behavior)
					{
						shared_ptr<ProceduralBehavior> procBehave  = shared_dynamic_cast<ProceduralBehavior> (behavior);
						
						Parameters procParams;
						for (size_t i=0;i<result.action.paramName.size();i++)
						{
							procParams[result.action.paramName[i]]=boost::make_shared<LiteralString>(result.action.paramValue[i]);
						}
						
						shared_ptr<Activity> act = askedProc->getActivity();
						vector< shared_ptr<ActivityNode> > nodes  = act->getNode();
					
						procBehave->pushProcedureToDo(askedProc, askedOrg, askedRole, procParams);
						procBehave->restart();

						shared_ptr<ACLMessage> reponse = make_shared<ACLMessage>(AGREE);
						reponse->setContent(msg->getContent());
						reponse->addReceiver(msg->getSender());
						agt->send(reponse);
					}
					else if (behavior2)
					{
						shared_ptr<ActionListenerBehavior> procBehave  = shared_dynamic_cast<ActionListenerBehavior> (behavior2);

						procBehave->pushProcedureToDo(askedProc, askedOrg, askedRole, Parameters());
						procBehave->restart();

						shared_ptr<ACLMessage> reponse = make_shared<ACLMessage>(AGREE);
						reponse->setContent(msg->getContent());
						reponse->addReceiver(msg->getSender());
						agt->send(reponse);
					}
					else
					{
						if (!done)
						{
							shared_ptr<ACLMessage> reponse = make_shared<ACLMessage>(NOT_UNDERSTOOD);
							string contentRep (act);
							contentRep += result.action.actionName;
							reponse->setContent(contentRep);
							reponse->addReceiver(msg->getSender());
							agt->send(reponse);
						}
					}
				}
				else
				{
					if (!done)
					{
						shared_ptr<ACLMessage> reponse = make_shared<ACLMessage>(NOT_UNDERSTOOD);
						string contentRep (act);
						contentRep += result.action.actionName;
						reponse->setContent(contentRep);
						reponse->addReceiver(msg->getSender());
						agt->send(reponse);
					}
				}
			}
		}
		else
		{
			// Proposition Non encore traite....
		}
	}
	else
	{
		cerr << "[SimpleCommunicationBehavior Warning] Parsing error. Message content parsing error: " << content << endl;
		
		shared_ptr<ACLMessage> reponse = make_shared<ACLMessage>(NOT_UNDERSTOOD);
		string contentRep (act);
		contentRep += content;
		reponse->setContent(contentRep);
		reponse->addReceiver(msg->getSender());
		agt->send(reponse);
	}
}

void SimpleCommunicationBehavior::_manageRequest(shared_ptr<ACLMessage> msg)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	//char act [155];
	//char actName [155];
	//char paramV [155];
	string content = msg->getContent();

    //cerr << " Message recu : " << content << endl;

	if (content == "Model") _sendModel(msg);
	else if (content == "Organisation") _sendOrganisation(msg);
	else if (content == "Actions") _sendActions(msg);
	else if (content == "Entities") _sendEntities(msg);  // Renvoi les attributs et etats de chaque entités appartenant  a la procedure et abonne aux messages de modifications d'attributs
	else
	{
	   stringstream str (content);
           string first;
	   str >> first;
           if (first == "STOP") 
	   {
		string procName;
		str >> procName;
		shared_ptr<ProceduralBehavior> be=(shared_dynamic_cast<ProceduralBehavior>(agt->getBehaviorExecutionByName("ProceduralBehavior")));
                if (procName =="ALL")
		   if (be) be->stopAll();
		else
	  	   if (be) be->stop(procName);
	   }
	   else _manageRequestAction(msg);
        }
}

void SimpleCommunicationBehavior::_manageInform(shared_ptr<ACLMessage> msg)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	string content = msg->getContent();
	FIPASLParserResult result=parseFipaSLExpression(content.c_str());
	if (result.isAction)
	{
		if (result.success)
		{
			shared_ptr<AgentBehaviorExecution> be=agt->getBehaviorExecutionByName("ProceduralBehavior");
			if (be)
			{
				shared_ptr<ProceduralBehavior> pbe=shared_dynamic_cast<ProceduralBehavior>(be);
				shared_ptr<AID> aid=shared_ptr<AID>(new AID(result.action.performer,agt->getAID()->getPlateformName(),agt->getAID()->getPlateformPort()));
				if (result.isDone)
				{
					aid=shared_ptr<AID>(new AID(result.action.performer,agt->getAID()->getPlateformName(),agt->getAID()->getPlateformPort()));
					pbe->onActionDone(aid, result.action.actionName);
				}
				else
				{
					pbe->onActionRunning(aid, result.action.actionName);
				}
			}
		}
	}
	else
	{
		cerr << result.variable << " of entity : " << result.entity << " == " << result.value << endl;
		shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
		shared_ptr<KnowledgeBase> kb = agt->getKnowledgeBase();
		shared_ptr<Environment> env = kb->getEnvironment();
		if (env)
		{
			shared_ptr<InstanceSpecification> entity;
	        	entity=env->getInstance(result.entity);
			if (entity)
			{
				shared_ptr<Slot> variable;
				variable = entity->getProperty(result.variable);
				if(variable)
				{
					variable->addValueFromString(result.value);
				}
				else
				{
					shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(NOT_UNDERSTOOD));
					reponse->setContent(content);
					reponse->addReceiver(msg->getSender());
					agt->send(reponse);
				}
			}
			else
			{
				shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(NOT_UNDERSTOOD));
				reponse->setContent(content);
				reponse->addReceiver(msg->getSender());
				agt->send(reponse);
			}
		}
		else
		{
			shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(NOT_UNDERSTOOD));
			reponse->setContent(content);
			reponse->addReceiver(msg->getSender());
			agt->send(reponse);
		}
	}
	if(msg->getXMLContent())
	{
		shared_ptr<XmlParser> newParser=shared_ptr<XmlParser>(new XmlParser());
		newParser->createFile("parser");
		shared_ptr<XmlNode> newRoot_node = newParser->getRoot();
		newRoot_node->addChild(msg->getXMLContent(), false);
		cerr << newParser->writeString()<<endl;
	}
}
void SimpleCommunicationBehavior::_manageQueryRef(shared_ptr<ACLMessage> msg)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	string content = msg->getContent();
	FIPASLParserResult result=parseFipaSLExpression(content.c_str());
	if (result.isAction)
	{
		shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(NOT_UNDERSTOOD));
		reponse->setContent(content);
		reponse->addReceiver(msg->getSender());
		agt->send(reponse);
	}
	else
	{
		cerr << result.variable << " of entity : " << result.entity << endl;
		shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
		shared_ptr<KnowledgeBase> kb = agt->getKnowledgeBase();
		shared_ptr<Environment> env = kb->getEnvironment();
		if (env)
		{
			shared_ptr<InstanceSpecification> entity;
	        	entity=env->getInstance(result.entity);
			if (entity)
			{
				shared_ptr<Slot> variable;
				variable = entity->getProperty(result.variable);
				if(variable)
				{
					string value = variable->getValue()->getStringFromValue();
					shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(INFORM));
					string content = "((=?"+result.entity+"."+result.variable+" "+value+"))";
                       	                reponse->setContent(content);
                       	                reponse->addReceiver(msg->getSender());
                       	                agt->send(reponse);
				}
				else
				{
					shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(NOT_UNDERSTOOD));
					reponse->setContent(content);
					reponse->addReceiver(msg->getSender());
					agt->send(reponse);
				}
			}
			else
			{
				shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(NOT_UNDERSTOOD));
				reponse->setContent(content);
				reponse->addReceiver(msg->getSender());
				agt->send(reponse);
			}
		}
		else
		{
			shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(NOT_UNDERSTOOD));
			reponse->setContent(content);
			reponse->addReceiver(msg->getSender());
			agt->send(reponse);
		}
	}
}

void SimpleCommunicationBehavior::_sendActions(shared_ptr<ACLMessage> msg)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	shared_ptr<AgentPlateform> plateform = agt->getAgentPlateform();
	
	const vector< pair<string , shared_ptr<AID> > >& agentsToInform=plateform->getAgentToInform();
	for(size_t i=0;i<agentsToInform.size();i++)
	{
		// Don't send if already registered
		if (
			agentsToInform[i].first == msg->getConversationID() &&
			agentsToInform[i].second->getName() == msg->getSender()->getName() &&
			agentsToInform[i].second->getPlateformName() == msg->getSender()->getPlateformName() &&
			agentsToInform[i].second->getPlateformPort() == msg->getSender()->getPlateformPort()
			)
		{
			return;
		}
	}
	
	plateform->addAgentToInform(msg->getSender(),msg->getConversationID());
}

void SimpleCommunicationBehavior::_sendOrganisation(shared_ptr<ACLMessage> msg)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	shared_ptr<AgentPlateform> plateform = agt->getAgentPlateform();
	vector<shared_ptr<OrganisationalEntity> > orgs = plateform->getOrganisationalEntity();

	shared_ptr<XmlParser> parser = shared_ptr<XmlParser>(new XmlParser());
	parser->createFile("Organisations");
	shared_ptr<XmlNode> root = parser->getRoot();

	for (size_t i = 0; i < orgs.size() ; i++)
	{
		shared_ptr<XmlNode> child (new XmlNode("Organisation"));
		string label = "name";
		string value = orgs[i]->getName();
		child->setProperty(label, value);

		cout<<"Organisation i : "<<orgs[i]->getName()<<endl;

		shared_ptr<XmlNode> childStruct (new XmlNode("Structure"));
		label = "name";
		value = orgs[i]->getStructure()->getName();
		childStruct->setProperty(label, value);
		child->addChild(childStruct);

		shared_ptr<XmlNode> childRoleAss (new XmlNode("RoleAssignements"));
		vector< shared_ptr<RoleAssignement> > roAssignements = orgs[i]->getRoleAssignements();
		for (size_t iRA = 0; iRA < roAssignements.size(); iRA ++)
		{
			shared_ptr<XmlNode> childAssign (new XmlNode("Assignement"));
			shared_ptr<XmlNode> childRole (new XmlNode("Role"));
			label = "name";
			value = roAssignements[iRA]->getRole()->getName();
			childRole->setProperty(label, value);
			childAssign->addChild(childRole);
			shared_ptr<XmlNode> childAgent (new XmlNode("Agent"));
			label = "name";
			value = roAssignements[iRA]->getAgent()->getName();
			childAgent->setProperty(label, value);
			label = "aid";
			value = roAssignements[iRA]->getAgent()->toString();
			childAgent->setProperty(label, value);
			childAssign->addChild(childAgent);
			childRoleAss->addChild(childAssign);
		}
		child->addChild(childRoleAss);

		shared_ptr<XmlNode> childRessourceAss (new XmlNode("RessourceAssignements"));
		vector< shared_ptr<RessourceAssignement> > reAssignements = orgs[i]->getRessourceAssignements();
		for (size_t iRA = 0; iRA < reAssignements.size(); iRA ++)
		{
			cout<<"reAssignements i : "<<reAssignements[iRA]->getRessource()<<endl;
			shared_ptr<XmlNode> childAssign (new XmlNode("Assignement"));
			shared_ptr<XmlNode> childRessource (new XmlNode("Ressource"));
			label = "name";
			value = reAssignements[iRA]->getRessource()->getName();
			childRessource->setProperty(label, value);
			childAssign->addChild(childRessource);
			shared_ptr<XmlNode> childEntity (new XmlNode("Entity"));
			label = "name";
			if (reAssignements[iRA]->getEntity())
			{
				value = reAssignements[iRA]->getEntity()->getName();
			}
			childEntity->setProperty(label, value);
			childAssign->addChild(childEntity);
			childRessourceAss->addChild(childAssign);
		}
		child->addChild(childRessourceAss);

		root->addChild(child);
	}

	shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(INFORM));
	reponse->setConversationID(msg->getConversationID());
	reponse->setXMLContent(root);
	reponse->addReceiver(msg->getSender());

	agt->send(reponse);
}

void SimpleCommunicationBehavior::_sendModel(shared_ptr<ACLMessage> msg)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));

	shared_ptr<Model> model = MascaretApplication::getInstance()->getModel();

	stringstream os;
	model->toXMI2(os);

	shared_ptr<XmlParser> parser = shared_ptr<XmlParser>(new XmlParser());
	parser->parseString(os.str());
	shared_ptr<XmlNode> root = parser->getRoot();

	shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(INFORM));
	reponse->setConversationID(msg->getConversationID());
	reponse->setXMLContent(root);
	reponse->addReceiver(msg->getSender());
	//cout<<"SENDING MODEL TO :"<<msg->getSender()->getName()<<" ID : "<<msg->getSender()->getPlateformName()<< " NAME : "<<msg->getSender()->getPlateformPort()<<endl;
	agt->send(reponse);
}

void SimpleCommunicationBehavior::_sendEntities(shared_ptr<ACLMessage> msg)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));

	shared_ptr<XmlNode> root=_buildWorldInfo();
	_suscribeEntitiesChanged(msg);
	shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(INFORM));
	reponse->setConversationID(msg->getConversationID());
	reponse->setXMLContent(root);
	reponse->addReceiver(msg->getSender());

	agt->send(reponse);
}

/*void SimpleCommunicationBehavior::_sendLearningSession(shared_ptr<ACLMessage> msg)
{

}*/

void SimpleCommunicationBehavior::_suscribeEntitiesChanged(shared_ptr<ACLMessage> msg)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	for(size_t i=0;i<_requestedSlotChanged.size();i++)
	{
		if (_requestedSlotChanged[i].second->getName() == msg->getSender()->getName())
		{
			return;
		}
	}
	_requestedSlotChanged.push_back(make_pair(msg->getConversationID(),msg->getSender()));
	
	if (!_callbackOnEntitiesAdded)
	{
		shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
		shared_ptr<AgentPlateform> plateform = agt->getAgentPlateform();
		vector<shared_ptr<OrganisationalEntity> > orgs = plateform->getOrganisationalEntity();

		for (size_t i = 0; i < orgs.size() ; i++)
		{
			string value;
			vector< shared_ptr<RessourceAssignement> > reAssignements = orgs[i]->getRessourceAssignements();
			for (size_t iRA = 0; iRA < reAssignements.size(); iRA ++)
			{
				if (reAssignements[iRA]->getEntity())
				value = reAssignements[iRA]->getEntity()->getName();
				
				shared_ptr<InstanceSpecification> instance=env->getInstance(value);
				if (instance)
				{
					const InstanceSpecification::SlotMap& slots=instance->getSlots();
					InstanceSpecification::SlotMap::const_iterator it=slots.begin();
					
					while (it !=slots.end())
					{
						cbSlotValue cb=boost::bind(&SimpleCommunicationBehavior::_onSlotChanged,this,_1,_2);
						instance->addCallbackOnAddSlotValue(it->first,cb);
						instance->addCallbackOnRemoveSlotValue(it->first,cb);
						it++;
					}
				}
			}
		}
		
		_callbackOnEntitiesAdded = true;
	}
}

void SimpleCommunicationBehavior::_onBehaviorStop(BehaviorExecution * be)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	string methodName = be->getSpecification()->getName();
	if (_requestedAction.find(methodName) != _requestedAction.end())
	{
		shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(INFORM));
		string content = "inform-done : ";
		content += be->getSpecification()->getName();
		reponse->setContent(content);
		vector < shared_ptr<AID> > aids = _requestedAction[methodName];
		for (size_t i = 0 ; i < aids.size() ; i++)
		{
			reponse->addReceiver(aids[i]);
		}
		agt->send(reponse);
	}
}

shared_ptr<XmlNode> SimpleCommunicationBehavior::_buildWorldInfo()
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
	shared_ptr<AgentPlateform> plateform = agt->getAgentPlateform();
	vector<shared_ptr<OrganisationalEntity> > orgs = plateform->getOrganisationalEntity();

	shared_ptr<XmlParser> parser = shared_ptr<XmlParser>(new XmlParser());
	parser->createFile("Entities");
	shared_ptr<XmlNode> root = parser->getRoot();

	for (size_t i = 0; i < orgs.size() ; i++)
	{
		shared_ptr<XmlNode> child (new XmlNode("OrganisationalEntity"));
		string label = "name";
		string value = orgs[i]->getName();
		child->setProperty(label, value);

		vector< shared_ptr<RessourceAssignement> > reAssignements = orgs[i]->getRessourceAssignements();
		for (size_t iRA = 0; iRA < reAssignements.size(); iRA ++)
		{
			shared_ptr<XmlNode> childEntity (new XmlNode("Entity"));
			label = "name";
			if (reAssignements[iRA]->getEntity())
			{
				value = reAssignements[iRA]->getEntity()->getName();
			}
			childEntity->setProperty(label, value);
			child->addChild(childEntity);
			
			shared_ptr<InstanceSpecification> instance=env->getInstance(value);
			if (instance)
			{
				label="class";
				value=instance->getClassifier()->getFullName();
				childEntity->setProperty(label, value);
				
				const InstanceSpecification::SlotMap& slots=instance->getSlots();
				InstanceSpecification::SlotMap::const_iterator it=slots.begin();
				while (it !=slots.end())
				{
					const map<string, shared_ptr<ValueSpecification> >& values=it->second->getValues();
					map<string, shared_ptr<ValueSpecification> >::const_iterator it2=values.begin();
					while (it2 != values.end())
					{
						shared_ptr<XmlNode> childAttrib (new XmlNode("Attribute"));
						label="name";
						value=it->first;
						childAttrib->setProperty(label,value);
						label="value";
						value=it2->second->getStringFromValue();
						childAttrib->setProperty(label,value);
						childEntity->addChild(childAttrib);
						it2++;
					}
					it++;
				}
			}
		}
		root->addChild(child);
	}
	return root;
}

void SimpleCommunicationBehavior::_onSlotChanged(string, shared_ptr<ValueSpecification>)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	shared_ptr<XmlNode> root=_buildWorldInfo();
	for (size_t i=0;i<_requestedSlotChanged.size();i++)
	{
		shared_ptr<ACLMessage> reponse = shared_ptr<ACLMessage>(new ACLMessage(INFORM));
		reponse->setConversationID(_requestedSlotChanged[i].first);
		reponse->setXMLContent(root);
		reponse->addReceiver(_requestedSlotChanged[i].second);

		agt->send(reponse);
	}
}

//temporary operation->action translation feature
vector<shared_ptr<ActionNode> > SimpleCommunicationBehavior::_translateOperationToActions(shared_ptr<Operation> operation)
{
	vector<shared_ptr<ActionNode> > performedActions;
	
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	shared_ptr<ProceduralBehavior> pb =  (shared_dynamic_cast<ProceduralBehavior>(agt->getBehaviorExecutionByName("ProceduralBehavior")));

	if (pb)
	{
		vector<shared_ptr<ProcedureExecution> > pes = pb->accessRunningProcedures();
		if (pes.size() == 0)
		{
			return performedActions;
		}
	}
	else
	{
		return performedActions;
	}
	
	//-----------------------------------------------------------------------------------//
	//resolve operation and parameters from name
	
	shared_ptr<Class> classifier = agt->getClassifier();
	
	//basic operation to action translator... begin =====================================\
	
	cerr << "========== begin operation to action translation ===========" << endl;
	
	vector<shared_ptr<ProcedureExecution> > pes = pb->accessRunningProcedures();
	
	cerr << "there are " << pes.size() << " running procedures..." << endl;
	
	for (size_t i=0; i<pes.size(); ++i)
	{
		vector<shared_ptr<ActionNode> > actionsToExecute;
		
		//TODO think of a better way to do this
		//----------------------------------------------------------------------------------------------------//
		//first try to see if there is any action that would advance the procedure
		actionsToExecute = pes[i]->getActionToExecuteFor(agt->getAID());
		
		cerr << "nr of actions to execute " << actionsToExecute.size() << endl;
		
		for (size_t j=0; j<actionsToExecute.size(); ++j)
		{
			cerr << "Action " << j << ": " << endl;
			//try to convert to call operation action
			shared_ptr<CallOperationAction> callOp = (shared_dynamic_cast<CallOperationAction>(actionsToExecute[j]->getAction()));
			if (callOp) //if succeeded
			{
				cerr << "CallOperationAction name: '" << actionsToExecute[j]->getName() << "' and operation name: '" << callOp->getOperation()->getName() << "'" << endl;
				cerr << "testing " << callOp->getOperation()->getName() << " vs " << operation->getName() << endl;
				if (callOp->getOperation() == operation)
				{
					cerr << "Found operation->action translation" << endl;
					performedActions.push_back(actionsToExecute[j]);
					break;
				}
			}
			else
			{
				cerr << "<not a CallOperationAction>" << endl;
			}
		}
		
		//----------------------------------------------------------------------------------------------------//
		
		//if no action was found that would advance the procedure, then choose the first matching one from the entire list
		if (performedActions.size() == 0)
		{
			actionsToExecute = pes[i]->getAllActionsFor(agt->getAID());
		
			cerr << "nr of actions to execute " << actionsToExecute.size() << endl;
		
			for (unsigned int j=0; j<actionsToExecute.size(); ++j)
			{
				cerr << "Action " << j << ": " << endl;
				//try to convert to call operation action
				shared_ptr<CallOperationAction> callOp = (shared_dynamic_cast<CallOperationAction>(actionsToExecute[j]->getAction()));
				if (callOp) //if succeeded
				{
					cerr << "CallOperationAction name: '" << actionsToExecute[j]->getName() << "' and operation name: '" << callOp->getOperation()->getName() << "'" << endl;
					cerr << "testing " << callOp->getOperation()->getName() << " vs " << operation->getName() << endl;
					if (callOp->getOperation() == operation)
					{
						cerr << "Found operation->action translation" << endl;
						performedActions.push_back(actionsToExecute[j]);
						break;
					}
				}
				else
				{
					cerr << "<not a CallOperationAction>" << endl;
				}
			}
		}
		//----------------------------------------------------------------------------------------------------//
	}
	
	cerr << "========== end operation to action translation ===========" << endl;
	
	//basic operation to action translator... end   =====================================/
	
	return performedActions;
}

}
