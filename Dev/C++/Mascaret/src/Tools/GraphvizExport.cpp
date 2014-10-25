#include "Tools/GraphvizExport.h"

#ifdef USE_GRAPHVIZ
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <boost/graph/graphviz.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/device/file.hpp>

/* Exporteur d'activité au format graphviz .dot
 * TODO: Gestion des Loop-Node
 * TODO: Affichage des expressions de garde
 * TODO: Gestion token d'execution (pour voir les actions en cours)
 */

namespace GraphTools
{
using VEHA::Activity;
using VEHA::ActivityEdge;
using VEHA::ActivityNode;

class ActivityNode_property_writer {
public:
	ActivityNode_property_writer(vector< shared_ptr<ActivityNode> > nodes,
			shared_ptr<ActivityNode> currentNode = shared_ptr<ActivityNode>()) :
		_nodes(nodes), _currentNode(currentNode) {}
	template <class VertexOrEdge>
	void operator()(std::ostream& out, const VertexOrEdge& v) const {
		shared_ptr<ActivityNode> node = _nodes[v];

		if(node->getKind() == "initial")
		{
			out << "[label=\"\",shape=point,height=0.35]";
			return;
		}
		if(node->getKind() == "final")
		{
			out << "[label=\"\",shape=doublecircle,style=filled,color=black,height=0.35]";
			return;
		}
		if(node->getKind() == "fork")
		{
			out << "[label=\"\",shape=rect,height=0.1,width=1,style=filled,color=black]";
			return;
		}
		if(node->getKind() == "join")
		{
			out << "[label=\"\",shape=rect,height=0.1,width=1,style=filled,color=black]";
			return;
		}

		out << "[label=\"" << node->getName() << "\"]";
		if(node->getKind() == "action")
			out << "[shape=box,style=rounded]";
		if(node->getKind() == "object")
			out << "[shape=box]";
		if(node->getKind() == "inputPin")
			out << "[shape=box]";

		if(node == _currentNode)
			out << "[color=red]";

		//cerr << " kind: " << _nodes[v]->getKind() << endl;
	}
private:
	vector< shared_ptr<ActivityNode> > _nodes;
	shared_ptr<ActivityNode> _currentNode;
};

class ActivityEdge_property_writer {
public:
	ActivityEdge_property_writer(vector< shared_ptr<ActivityEdge> > edges) : _edges(edges) {}
	template <class VertexOrEdge>
	void operator()(std::ostream& out, const VertexOrEdge& v) const {
	}
private:
	vector< shared_ptr<ActivityEdge> > _edges;
};


void GraphVizActivity::exporting(std::ostream& out, shared_ptr<Activity> activity, shared_ptr<ActivityNode> activeNode)
{
	using namespace boost;

	shared_ptr<ActivityNode> initial = activity->getInitialNode();
	vector< shared_ptr<ActivityNode> > nodes = activity->getNode();
	vector< shared_ptr<ActivityEdge> > edges = activity->getEdge();

	typedef pair<int,int> Edge;

	typedef adjacency_list< vecS, vecS, directedS,
			property< vertex_color_t, default_color_type >,
			property< edge_weight_t, int >
	> Graph;

	vector<Edge> edges_g;

	cout << "nb nodes: " << nodes.size() << endl;
	cout << "nb edges: " << edges.size() << endl;

	for(size_t i = 0; i < edges.size(); i++)
	{
		shared_ptr<ActivityEdge> edge = edges[i];
		shared_ptr<ActivityNode> source;
		shared_ptr<ActivityNode> target;
		int s_id;
		int t_id;

		for(size_t j = 0; j < nodes.size(); j++)
		{
			shared_ptr<ActivityNode> node = nodes[j];
			if(node == edge->getSource())
			{
				source = node;
				s_id = j;
			}
			if(node == edge->getTarget())
			{
				target = node;
				t_id = j;
			}
		}

		if(!target)
		{
			target = edge->getTarget();
			cerr << " Edge: " << edge->getName() << " target not found "
					<< target->getFullName()
					<< " type " << target->getKind() << endl;
			t_id = nodes.size();
			nodes.push_back(target);
		}

		if(source && target)
			edges_g.push_back(Edge(s_id, t_id));
	}

	Graph graph(edges_g.begin(), edges_g.end(), nodes.size());

//	graph_property<Graph, graph_graph_attribute_t>::type& graphAttr = get_property(graph, graph_graph_attribute);
//	graphAttr["name"] = activity->getName();

	string filename = activity->getName() + ".dot";

	std::map<string,string> graph_attr, vertex_attr, edge_attr;
	graph_attr["name"] = activity->getName();

	//write_graphviz(filename, graph, ActivityNode_label_property_writer(nodes));
	write_graphviz(out, graph,
			ActivityNode_property_writer(nodes, activeNode),
			default_writer(),
			make_graph_attributes_writer(graph_attr, vertex_attr,edge_attr));
			//); //, ActivityEdge_property_writer(edges));
			//make_graph_attributes_writer(graph_attr, vertex_attr,edge_attr));
}

string GraphVizExport::generate_svg(const string & dot)
{
	using std::ofstream;

	// Generate a tempory filename
	char filename[L_tmpnam];
	tmpnam(filename);

	// Write tempory file with dot file data
	ofstream dotfile;
	dotfile.open(filename);
	dotfile << dot << endl;
	dotfile.close();

	// Execute external graphviz program
	string cmd("dot -Tsvg ");
	cmd.append(filename);
	FILE * svgstream = popen(cmd.c_str(), "r");

	// Get SVG data from pipe
	string svg;
	char c;
	while((c = fgetc(svgstream)) != EOF)
	{
		svg.push_back(c);
	}

	// Close pipe and remove tempory file
	pclose(svgstream);
	remove(filename);

	cout << dot << endl;

	return svg;
}
}
#else
namespace GraphTools
{
using VEHA::Activity;
using VEHA::ActivityNode;

void GraphVizActivity::exporting(std::ostream& out, shared_ptr<Activity> activity, shared_ptr<ActivityNode> activeNode)
{
}

string GraphVizExport::generate_svg(const string & dot)
{
	return "";
}
}
#endif
