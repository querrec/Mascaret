#ifndef __GRAPHVIZ_EXPORT__
#define __GRAPHVIZ_EXPORT__

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Activity/Activity.h"

namespace GraphTools
{
class GraphVizExport
{
public:
	static string generate_svg(const string& dot);
};

class GraphVizActivity
{
public:
	static void exporting(std::ostream& out, shared_ptr<VEHA::Activity> activity,
			shared_ptr<VEHA::ActivityNode> activeNode = shared_ptr<VEHA::ActivityNode>());
};
}

#endif /* #ifndef __GRAPHVIZ_EXPORT__ */
