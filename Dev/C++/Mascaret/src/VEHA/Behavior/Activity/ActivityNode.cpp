#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ObjectNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/Activity.h"
namespace VEHA
{

ActivityNode::ActivityNode(const string& name, const string& kind) : Element(name), _kind(kind), _running(false)
{
}

ActivityNode::~ActivityNode()
{
}

vector< shared_ptr<ObjectNode> > ActivityNode::getOutgoingObjectNode(void)
{
	vector< shared_ptr<ObjectNode> > nodes;
	vector< shared_ptr<ActivityEdge> > edges = getOutgoing();
	for (size_t iE = 0; iE < edges.size() ; iE ++)
	{
		shared_ptr<ActivityNode> node = edges[iE]->getTarget();
		if (node && node->getKind() == "object")
			nodes.push_back(shared_dynamic_cast<ObjectNode>(node));

	}
	return nodes;
}

vector< shared_ptr<ObjectNode> > ActivityNode::getIncomingObjectNode(void)
{
	vector< shared_ptr<ObjectNode> > nodes;
	vector< shared_ptr<ActivityEdge> > edges = getIncoming();
	for (size_t iE = 0; iE < edges.size() ; iE ++)
	{
		shared_ptr<ActivityNode> node = edges[iE]->getSource();
		if (node && node->getKind() == "object")
			nodes.push_back(shared_dynamic_cast<ObjectNode>(node));
	}
	return nodes;
}

vector< shared_ptr<ActionNode> > ActivityNode::getOutgoingActionNode(void)
{
	vector< shared_ptr<ActionNode> > nodes;
	vector< shared_ptr<ActivityEdge> > edges = getOutgoing();
        cerr << " EDGES : " << edges.size() << endl;
	for (size_t iE = 0; iE < edges.size() ; iE ++)
	{
		shared_ptr<ActivityNode> node = edges[iE]->getTarget();
        cout << " Target : " << node->getFullName() << " kind: " << node->getKind() << endl;

        vector< shared_ptr<ActivityPartition> > toto = node->getPartition();
		if (node && node->getKind() == "action") nodes.push_back(shared_dynamic_cast<ActionNode>(node));
		else if (node && node->getKind() == "loop") nodes.push_back(shared_dynamic_cast<ActionNode>(node));
		else
		{
			vector< shared_ptr<ActionNode> > nodesTmp = node->getOutgoingActionNode();
			for (size_t iN = 0; iN < nodesTmp.size(); iN++)
			{
				nodes.push_back(nodesTmp[iN]);
			}
		}
	}
	return nodes;
}
vector<shared_ptr<ActionNode> > ActivityNode::getPossibleOutgoingActionNode()
{
	vector< shared_ptr<ActionNode> > nodes;
	vector< shared_ptr<ActivityEdge> > edges = getOutgoing();
	for (size_t iE = 0; iE < edges.size() ; iE ++)
	{
		shared_ptr<ActivityNode> node = edges[iE]->getTarget();
		if (node && node->getKind() == "action") nodes.push_back(shared_dynamic_cast<ActionNode>(node));
		else
		{
			vector< shared_ptr<ActionNode> > nodesTmp = node->getPossibleOutgoingActionNode();
			for (size_t iN = 0; iN < nodesTmp.size(); iN++)
			{
				nodes.push_back(nodesTmp[iN]);
			}
		}
	}
	return nodes;
}
vector< shared_ptr<ActionNode> > ActivityNode::getOutgoingActionNodeByPartition(const string& partitionName)
{
	vector< shared_ptr<ActionNode> > nodes;
	vector< shared_ptr<ActivityEdge> > edges = getOutgoing();
	for (size_t iE = 0; iE < edges.size() ; iE ++)
	{
		shared_ptr<ActivityNode> node = edges[iE]->getTarget();
		if (node && node->getKind() == "action")
		{
			if (node->getPartition()[0]->getName() == partitionName)
				nodes.push_back(shared_dynamic_cast<ActionNode>(node));
		}
		else
		{
			vector< shared_ptr<ActionNode> > nodesTmp = node->getOutgoingActionNode();
			for (size_t iN = 0; iN < nodesTmp.size(); iN++)
			{
				nodes.push_back(nodesTmp[iN]);
			}
		}
	}
	return nodes;
}


vector< shared_ptr<ActionNode> > ActivityNode::getIncomingActionNode(void)
{
	vector< shared_ptr<ActionNode> > nodes;
	vector< shared_ptr<ActivityEdge> > edges = getIncoming();
	for (size_t iE = 0; iE < edges.size() ; iE ++)
	{
		shared_ptr<ActivityNode> node = edges[iE]->getTarget();
		if (node && node->getKind() == "action") nodes.push_back(shared_dynamic_cast<ActionNode>(node));
		else
		{
			vector< shared_ptr<ActionNode> > nodesTmp = node->getIncomingActionNode();
			for (size_t iN = 0; iN < nodesTmp.size(); iN++)
			{
				nodes.push_back(nodesTmp[iN]);
			}
		}
	}
	return nodes;
}
vector< shared_ptr<ActionNode> > ActivityNode::getIncomingActionNodeByPartition(string partitionName)
{
	vector< shared_ptr<ActionNode> > nodes;
	vector< shared_ptr<ActivityEdge> > edges = getIncoming();
	for (size_t iE = 0; iE < edges.size() ; iE ++)
	{
		shared_ptr<ActivityNode> node = edges[iE]->getTarget();
		if (node && node->getKind() == "action")
		{
			if (node->getPartition()[0]->getName() == partitionName)
				nodes.push_back(shared_dynamic_cast<ActionNode>(node));
		}
		else
		{
			vector< shared_ptr<ActionNode> > nodesTmp = node->getIncomingActionNode();
			for (size_t iN = 0; iN < nodesTmp.size(); iN++)
			{
				nodes.push_back(nodesTmp[iN]);
			}
		}
	}
	return nodes;
}

vector< shared_ptr<ActivityEdge> > ActivityNode::getOutgoingActionNodeEdges(void)
{
	vector< shared_ptr<ActivityEdge> > actionNodeEdges;
	vector< shared_ptr<ActivityEdge> > edges = getOutgoing();
        cerr << " EDGES : " << edges.size() << endl;
	for (size_t iE = 0; iE < edges.size() ; iE ++)
	{
		shared_ptr<ActivityNode> node = edges[iE]->getTarget();
        cout << " Target : " << node->getName() << endl;

        vector< shared_ptr<ActivityPartition> > toto = node->getPartition();
		if (node && node->getKind() == "action")
		{
			actionNodeEdges.push_back(edges[iE]);
		}
		else if (node && node->getKind() == "loop")
		{
			actionNodeEdges.push_back(edges[iE]);
		}
		else
		{
			vector< shared_ptr<ActivityEdge> > nodesTmp = node->getOutgoingActionNodeEdges();
			for (size_t iN = 0; iN < nodesTmp.size(); iN++)
			{
				actionNodeEdges.push_back(nodesTmp[iN]);
			}
		}
	}
	return actionNodeEdges;
}

vector<shared_ptr<ActivityEdge> > ActivityNode::getOutgoingControlFlowEdges()
{
	vector< shared_ptr<ActivityEdge> > allOutgoing = getOutgoing();
	cerr << "Size: " << allOutgoing.size() << endl;
	vector< shared_ptr<ActivityEdge> > controlFlows;
	for(size_t i = 0; i < allOutgoing.size(); ++i)
	{
		if (allOutgoing[i]->getKind() == "ControlFlow")
			controlFlows.push_back(allOutgoing[i]);
	}
	return controlFlows;
}
}
