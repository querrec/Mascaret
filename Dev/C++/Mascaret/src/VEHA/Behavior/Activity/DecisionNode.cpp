#include "VEHA/Behavior/Activity/DecisionNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ControlFlow.h"

namespace VEHA
{

DecisionNode::DecisionNode()
:ControlNode("Decision","decision")
{
}

DecisionNode::~DecisionNode()
{
}
vector<shared_ptr<ActionNode> > DecisionNode::getOutgoingActionNode()
{
	vector<shared_ptr<ActionNode> > nodes;
	vector<shared_ptr<ActivityEdge> > edges = getOutgoing();
	for (unsigned int i=0; i<edges.size(); i++)
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
						vector<shared_ptr<ActionNode> > actions=edges[i]->getTarget()->getOutgoingActionNode();
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
			if(edges[i]->getTarget()->getKind() == "action")
			{
				nodes.push_back(shared_dynamic_cast<ActionNode>(edges[i]->getTarget()));
			}
			else
			{
				vector<shared_ptr<ActionNode> > actions=edges[i]->getTarget()->getOutgoingActionNode();
				for(size_t i=0;i<actions.size();i++)
					nodes.push_back(actions[i]);

			}
		}
	}
	return nodes;
}
vector<shared_ptr<ActionNode> > DecisionNode::getOutgoingActionNodeByPartition(const string& partitionName)
{
	vector<shared_ptr<ActionNode> > nodes;
	vector<shared_ptr<ActivityEdge> > edges = getOutgoing();
	for (unsigned int i=0; i<edges.size(); i++)
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
