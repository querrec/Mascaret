#include "VEHA/Kernel/expressionParser.h"
#include "VEHA/Kernel/Slot.h"

using namespace VEHA::OclParser;
using namespace VEHA;
int main()
{

	shared_ptr<Class> cl(new Class("cl1"));
	shared_ptr<Property> p(new Property("prop",cl,cl));
	p->setMaximumNumber(1);
	cl->addAttribute(p);
	shared_ptr<InstanceSpecification> is(new InstanceSpecification("toto", cl));
	shared_ptr<InstanceSpecification> is2(new InstanceSpecification("toto2", cl));
	is->getProperty("prop").addValue(InstanceValue(is2));
	Context ctx;
	ctx["self"]=is;
	string all;
	string str;
	while (getline(cin, str))
	{
		if (str.empty() || str[0] == 'q' || str[0] == 'Q')
			break;
		all+=" "+str;
	}
	cout << all << endl;

	ExpressionTree et= parseExpression(all) ;

	cout<<"Value:"<<evalExpression(et,ctx) << endl;


	//#if defined(BOOST_SPIRIT_DUMP_PARSETREE_AS_XML)
	//      std::map<parser_id, std::string> rule_names;
	//            rule_names[oclExpressions.id()] = "oclExpression";
	//
	////            rule_names[] = "factor";
	////            rule_names[termID] = "term";
	////            rule_names[expressionID] = "expression";
	//            tree_to_xml(cout, info.trees, str.c_str(), rule_names);
	//#endif
	//		cout << "parsing succeeded\n";
	//

}



