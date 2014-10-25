#include "VEHA/Behavior/Activity/ChoiceNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ControlFlow.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/Activity.h"

namespace VEHA
{
size_t ChoiceNode::choiceNodeCount = 1;

ChoiceNode::ChoiceNode() : ControlNode("Choice","choice")
{
	string name="Choice";
	name+=choiceNodeCount++ + 0x30;
	setName(name);
}

ChoiceNode::~ChoiceNode()
{
}
vector<shared_ptr<ActionNode> > ChoiceNode::getPossibleOutgoingActionNode()
{
	vector<shared_ptr<ActionNode> > nodes;
	vector<shared_ptr<ActivityEdge> > edges = getOutgoing();
	for (size_t i=0; i<edges.size(); i++)
	{
		if (edges[i]->getGuard())
		{
			bool eval=edges[i]->getGuard()->eval(_activity.lock()->getContext());
			if(eval)
			{
				if(edges[i]->getTarget()->getKind() == "action")
				{
					nodes.push_back(shared_dynamic_cast<ActionNode>(edges[i]->getTarget()));
				}
				else
				{
					vector<shared_ptr<ActionNode> > actions=edges[i]->getTarget()->getPossibleOutgoingActionNode();
					for(size_t i=0;i<actions.size();i++)
					{
						nodes.push_back(actions[i]);
					}
				}
			}
		}
		else
		{
			if(edges[i]->getTarget()->getKind() == "action")
			{
				nodes.push_back(shared_dynamic_cast<ActionNode>(edges[i]->getTarget()));
			}
			else
			{
				vector<shared_ptr<ActionNode> > actions=edges[i]->getTarget()->getPossibleOutgoingActionNode();
				for(size_t i=0;i<actions.size();i++)
				{
					nodes.push_back(actions[i]);
				}
			}
		}
	}
	return nodes;
}
vector<shared_ptr<ActionNode> > ChoiceNode::getOutgoingActionNode()
{
	vector<shared_ptr<ActionNode> > nodes;
	vector<shared_ptr<ActivityEdge> > edges = getOutgoing();
	for (size_t i=0; i<edges.size(); i++)
	{
		if (edges[i]->getGuard())
		{
			cerr << "eval "<<shared_dynamic_cast<Expression>(edges[i]->getGuard()->getSpecification())->getExpression()<<endl;
			bool eval=edges[i]->getGuard()->eval(_activity.lock()->getContext());
			if(eval)
			{
				if(edges[i]->getTarget()->getKind() == "action")
				{
					nodes.push_back(shared_dynamic_cast<ActionNode>(edges[i]->getTarget()));
					break;
				}
				else
				{
					vector<shared_ptr<ActionNode> > actions=edges[i]->getTarget()->getOutgoingActionNode();
					for(size_t i=0;i<actions.size();i++)
					{
						nodes.push_back(actions[i]);
					}
					if(actions.size())
						break;
				}
			}
		}
		else
		{
			if(edges[i]->getTarget()->getKind() == "action")
			{
				nodes.push_back(shared_dynamic_cast<ActionNode>(edges[i]->getTarget()));
				break;
			}
			else
			{
				vector<shared_ptr<ActionNode> > actions=edges[i]->getTarget()->getOutgoingActionNode();
				for(size_t i=0;i<actions.size();i++)
				{
					nodes.push_back(actions[i]);
				}
				if(actions.size())
					break;
			}
		}
	}
	return nodes;
}
vector<shared_ptr<ActionNode> > ChoiceNode::getOutgoingActionNodeByPartition(const string& partitionName)
{
	vector<shared_ptr<ActionNode> > nodes;
	vector<shared_ptr<ActivityEdge> > edges = getOutgoing();
	for (size_t i=0; i<edges.size(); i++)
	{
		if(edges[i]->getTarget()->getPartition()[0]->getName() == partitionName)
		{
			if (edges[i]->getGuard())
			{
				bool eval;
				if (edges[i]->getGuard()->getSpecification()->getBoolValue(eval))
				{
					if(eval)
					{
						if(edges[i]->getTarget()->getKind() == "action")
						{
							nodes.push_back(shared_dynamic_cast<ActionNode>(edges[i]->getTarget()));
						}
						else
						{
							vector<shared_ptr<ActionNode> > actions=edges[i]->getTarget()->getOutgoingActionNodeByPartition(partitionName);
							for(size_t i=0;i<actions.size();i++)
								nodes.push_back(actions[i]);
						}
					}
				}
				else
				{
					cerr << "FAILED eval Expression!!!"<< endl;
				}
			}
			else
			{
				if(edges[i]->getTarget()->getPartition()[0]->getName() == partitionName)
				{
					if(edges[i]->getTarget()->getKind() == "action")
					{
						nodes.push_back(shared_dynamic_cast<ActionNode>(edges[i]->getTarget()));
					}
					else
					{
						vector<shared_ptr<ActionNode> > actions=edges[i]->getTarget()->getOutgoingActionNodeByPartition(partitionName);
						for(size_t i=0;i<actions.size();i++)
							nodes.push_back(actions[i]);
					}
				}
			}
		}
	}
	return nodes;
}

}
