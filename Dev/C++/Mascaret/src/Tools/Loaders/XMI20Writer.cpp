
#include "Tools/Loaders/XMI20Writer.h"
#include "Tools/IDCalculator.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Kernel/PrimitiveType.h"

#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/InitialNode.h"
#include "VEHA/Behavior/Activity/FinalNode.h"
#include "VEHA/Behavior/Activity/ActionNode.h"

#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"

#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include <fstream>
using std::string;
using std::ofstream;
using std::ostream;

using namespace VEHA;

XMI20Writer::XMI20Writer()
{
}
XMI20Writer::~XMI20Writer()
{
}
void XMI20Writer::writeModel(shared_ptr<Model> m,const string& filename)
{
	ofstream out(filename.c_str());
	writeModel(m,out);
}
void XMI20Writer::writeModel(shared_ptr<Model> m,ostream& out)
{
	currentIndentation=2;
	currentCommentNumber=5;

	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	out << "<xmi:XMI xmi:version=\"2.1\" xmlns:uml=\"http://schema.omg.org/spec/UML/2.1\" xmlns:xmi=\"http://schema.omg.org/spec/XMI/2.1\" xmlns:Mascaret=\"http:///schemas/Mascaret/1\" xmlns:VEHA=\"http:///schemas/VEHA/1\" xmlns:BEHAVE=\"http:///schemas/BEHAVE/1\" xmlns:HAVE=\"http:///schemas/HAVE/1\">" << endl;
	out	<< "<xmi:Documentation exporter=\"Bouml\" exporterVersion=\"1.11\"/>"<<endl;
	out << "\t<uml:Model xmi:type=\"uml:Model\" xmi:id=\"themodel\" name=\"" << m->getUrl() << "\">" << endl;
	out << "\t\t<profileApplication xmi:type=\"uml:ProfileApplication\" xmi:id=\"BOUML_PRFA_0x1f467_22\">" << endl;
	out << "\t\t\t<appliedProfile xmi:idref=\"BOUML_0x1f467_22\"/>" << endl;
	out << "\t\t</profileApplication>" << endl;
	out << "\t\t<profileApplication xmi:type=\"uml:ProfileApplication\" xmi:id=\"BOUML_PRFA_0x1f4e7_22\">" << endl;
	out << "\t\t\t<appliedProfile xmi:idref=\"BOUML_0x1f4e7_22\"/>" << endl;
	out << "\t\t</profileApplication>" << endl;
	out << "\t\t<profileApplication xmi:type=\"uml:ProfileApplication\" xmi:id=\"BOUML_PRFA_0x1f567_22\">" << endl;
	out << "\t\t\t<appliedProfile xmi:idref=\"BOUML_0x1f567_22\"/>" << endl;
	out << "\t\t</profileApplication>" << endl;
	out << "\t\t<profileApplication xmi:type=\"uml:ProfileApplication\" xmi:id=\"BOUML_PRFA_0x1f5e7_22\">" << endl;
	out << "\t\t\t<appliedProfile xmi:idref=\"BOUML_0x1f5e7_22\"/>" << endl;
	out << "\t\t</profileApplication>" << endl;
	writePackage(out, m->getPackage(),true);
	out << "\t\t<packagedElement xmi:type=\"uml:DataType\" xmi:id=\"BOUML_datatype_2\" name=\"boolean\"/>" << endl;
	out << "\t\t<packagedElement xmi:type=\"uml:DataType\" xmi:id=\"BOUML_datatype_0\" name=\"double\"/>" << endl;
	out << "\t\t<packagedElement xmi:type=\"uml:DataType\" xmi:id=\"BOUML_datatype_1\" name=\"integer\"/>" << endl;
	out << "\t</uml:Model>" << endl;
	//out << "\t<VEHA:Entity xmi:id=\"BOUML_STELT_0x1f88c_4\" base_Element=\"BOUML_0x1f88c_4\"/>" << endl;
	out << "</xmi:XMI>" << endl;
}

void XMI20Writer::writePackage(ostream& out, shared_ptr<Package> p,bool initialPackage)
{
	string indent=buildIndent();
	currentIndentation++;
	setId(p);
	out << indent << "<packagedElement xmi:type=\"uml:Package\" xmi:id=\"" << p->getId() << "\" name =\""<<  p->getName() << "\">" << endl;
	out << indent << "\t<ownedComment xmi:type=\"uml:Comment\" xmi:id=\"COMMENT_1\" body=\"" << p->getDescription() << "\"/>"<< endl;
	if(initialPackage)
		writeMascaretProfile(out);
	const vector< shared_ptr<Package> >& packages=p->getPackages();
	for(size_t i=0;i<packages.size();i++)
		writePackage(out,packages[i]);
	const vector< shared_ptr<Class> >& classes=p->getClasses();
	for(size_t i=0;i<classes.size();i++)
		writeClass(out,classes[i]);
	for(map<string,shared_ptr<Activity> >::iterator i=_activities.begin();i!=_activities.end();i++)
	{
		writeActivity(out,i->second,i->first);
	}
	_activities.clear();
	for(map<string,shared_ptr<StateMachine> >::iterator i=_stateMachines.begin();i!=_stateMachines.end();i++)
	{
		writeStateMachine(out,i->second,i->first);
	}
	_stateMachines.clear();
	out << indent << "</packagedElement>" << endl;
	currentIndentation--;
}

void XMI20Writer::writeClass(ostream& out, shared_ptr<Class> c)
{
	string indent=buildIndent();
	currentIndentation++;
	setId(c);
	out << indent << "<packagedElement xmi:type=\"uml:Class\" name=\"" << c->getName() << "\" xmi:id=\"" << c->getId() << "\" visibility=\"package\">" << endl;
	for(size_t i=0;i<c->getParents().size();i++)
	{
		setId(c->getParents()[i]);
		out << indent << "\t<generalization xmi:type=\"uml:Generalization\" xmi:id=\"" << c->getParents()[i]->getId() << "0\" general=\"" << c->getParents()[i]->getId() << "\"/>" << endl;
	}
	const map<string, shared_ptr<Property> >& attributes=c->getAttributes();
	for(map<string, shared_ptr<Property> >::const_iterator i=attributes.begin();i!=attributes.end();i++)
	{
		writeAttribute(out,i->second);
	}
	const map<string, shared_ptr<Operation> >& operations=c->getOperations();
	for(map<string, shared_ptr<Operation> >::const_iterator i=operations.begin();i!=operations.end();i++)
	{
		writeOperation(out,i->second);
	}
	const map<string,shared_ptr<Behavior> >& behaviors=c->getOwnedBehavior();
	for(map<string,shared_ptr<Behavior> >::const_iterator i=behaviors.begin();i!=behaviors.end();i++)
	{
		shared_ptr<StateMachine> sm=shared_dynamic_cast<StateMachine>(i->second);
		if(sm)
			_stateMachines[c->getId()]=sm;
	}
	out << indent << "</packagedElement>" << endl;
	currentIndentation--;
}

void XMI20Writer::writeAttribute(ostream& out, shared_ptr<Property> p)
{
	string indent=buildIndent();
	if(shared_dynamic_cast<PrimitiveType>(p->getType()))
	{
		string typeName=p->getType()->getName();
		string typeId;
		if(typeName == "real") // TODO: le type double ne dois pas etre correcte. A verifier
		{
			typeId="BOUML_datatype_0";
		}
		else if(typeName == "integer")
		{
			typeId="BOUML_datatype_1";
		}
		else if(typeName == "bool")
		{
			typeId="BOUML_datatype_2";
		}

		out << indent << "<ownedAttribute xmi:type=\"uml:Property\" name=\""<< p->getName() <<"\" xmi:id=\""<<"\" visibility=\"public\">" << endl;
		out << indent << "\t<type xmi:type=\"uml:Class\" xmi:idref=\"" << typeId << "\"/>" << endl;
		if(p->getDefaultValue())
			out << indent << "<defaultValue xmi:type=\"uml:LiteralString\" xmi:id=\"\" value=\"" << p->getDefaultValue()->getStringFromValue() << "\"/>" << endl;
		out << indent << "</ownedAttribute>" << endl;
	}
	else
	{
		string typeName=p->getType()->getName();
		setId(p->getType());
		out << indent << "<ownedAttribute xmi:type=\"uml:Property\" name=\""<< p->getName() <<"\" xmi:id=\""<<"\" visibility=\"public\">" << endl;
		out << indent << "\t<type xmi:type=\"uml:Class\" xmi:idref=\"" << p->getType()->getId() << "\"/>" << endl;
		out << indent << "\t<lowerValue xmi:type=\"uml:LiteralString\" xmi:id=\"BOUML_MULTIPLICITY_L_0x1fb8c_0\" value=\"" << p->getMinimumNumber() << "\"/>" << endl;
		out << indent << "\t<upperValue xmi:type=\"uml:LiteralString\" xmi:id=\"BOUML_MULTIPLICITY_U_0x1fb8c_0\" value=\"" << p->getMaximumNumber() << "\"/>" << endl;
		if(p->getDefaultValue())
			out << indent << "<defaultValue xmi:type=\"uml:LiteralString\" xmi:id=\"\" value=\"" << p->getDefaultValue()->getStringFromValue() << "\"/>" << endl;
		out << indent << "</ownedAttribute>" << endl;
	}
}
void XMI20Writer::writeOperation(ostream& out, shared_ptr<Operation> op)
{
	string indent=buildIndent();
	currentIndentation++;
	setId(op);
	out << indent << "<ownedOperation xmi:type=\"uml:Operation\" name=\"" << op->getName() << "\" xmi:id=\"" << op->getId() << "\" visibility=\"public\" isAbstract=\"false\">" << endl;
	for(size_t i=0;i<op->getParameter().size();i++)
		writeParameter(out,op->getParameter()[i]);
	out << indent << "</ownedOperation>" << endl;
	shared_ptr<Activity> activity=shared_dynamic_cast<Activity>(op->getMethod());
	if(activity)
		_activities[op->getId()]=activity;
	currentIndentation--;
}
void XMI20Writer::writeParameter(ostream& out, shared_ptr<Parameter> p)
{
	string indent=buildIndent();
	setId(p);
	out << indent << "<ownedParameter name=\"" << p->getName() << "\" xmi:direction=\"" << p->getDirection() << "\"" << endl;
	setId(p->getType());
	out << indent << "\t<type xmi:type=\"uml:Class\" xmi:id=\""<< p->getType()->getId() << "\"/>" << endl;
	out << indent << "</ownedParameter>" << endl;
}
void XMI20Writer::writeActivity(ostream& out,shared_ptr<Activity> act,const string& opId)
{
	string indent=buildIndent();
	setId(act);
	currentIndentation++;
	currentIndentation++;
	vector<shared_ptr<ActivityNode> > nodesDone;
	out << indent << "<packagedElement xmi:type=\"uml:Activity\" xmi:id=\"" << act->getId() << "\" name=\"" << act->getName() <<"\" specification=\"" << opId <<"\">" << endl;
	vector<shared_ptr<ActivityPartition> > parts=act->getPartition();
	for(size_t i=0;i<parts.size();i++)
	{
		setId(parts[i]);
		out << indent << "\t<group xmi:type=\"uml:ActivityPartition\" name=\"" << act->getName() << "\" xmi:id=\"" << parts[i]->getId() << "\" represents=\"" << (parts[i]->getRole()?parts[i]->getRole()->getId():"") << "\">" << endl;
		vector<shared_ptr<ActivityNode> > nodes=parts[i]->getNode();
		for(size_t j=0;j<nodes.size();j++)
		{
			writeActivityNode(out,nodes[j]);
			nodesDone.push_back(nodes[j]);
		}
		out << indent << "\t</group>" << endl;
	}
	currentIndentation--;
	vector<shared_ptr<ActivityNode> > nodes=act->getNode();
	for(size_t i=0;i<nodes.size();i++)
	{
		bool found=false;
		for(size_t j=0;j<nodesDone.size();j++)
		{
			if(nodesDone[j]==nodes[i])
			{
				found=true;
				break;
			}
		}
		if(!found)
			writeActivityNode(out,nodes[i]);
	}
	out << indent << "</packagedElement>" << endl;

	currentIndentation--;
}
void XMI20Writer::writeActivityNode(ostream& out,shared_ptr<ActivityNode> node)
{
	string indent=buildIndent();
	setId(node);
	if(shared_dynamic_cast<InitialNode>(node))
	{
		out << indent << "<node xmi:type=\"uml:InitialNode\" xmi:id=\"" << node->getId() << "\">" << endl;
	}
	else if(shared_dynamic_cast<FinalNode>(node))
	{
		out << indent << "<node xmi:type=\"uml:FinalNode\" xmi:id=\"" << node->getId() << "\">" << endl;
	}
	else if(shared_dynamic_cast<ActionNode>(node))
	{
		shared_ptr<ActionNode> actionNode=shared_dynamic_cast<ActionNode>(node);
		if(shared_dynamic_cast<SendSignalAction>(actionNode->getAction()))
		{
			out << indent << "<node xmi:type=\"uml:SendSignalAction\" xmi:id=\"" << node->getId() << "\">" << endl;
		}
		else if(shared_dynamic_cast<CallOperationAction>(actionNode->getAction()))
		{
			out << indent << "<node xmi:type=\"uml:CallOperationAction\" xmi:id=\"" << node->getId() << "\">" << endl;
		}
	}
	out << indent << "</node>" << endl;
}
void XMI20Writer::writeStateMachine(ostream& out,shared_ptr<StateMachine> sm,const string& classifierId)
{
	out << "<packagedElement xmi:type=\"uml:StateMachine\" xmi:id=\"" << sm->getId() << "\" name=\"" << sm->getName() <<"\" specification=\"" << classifierId <<"\">" << endl;
	out << "</packagedElement>" << endl;
}

void XMI20Writer::writeMascaretProfile(ostream& out)
{
	string indent=buildIndent();
	out << indent << "<packagedElement xmi:type=\"uml:Profile\" xmi:id=\"BOUML_0x1f467_22\" name =\"Mascaret\">" << endl;
	out << indent << "\t<packagedElement xmi:type=\"uml:Profile\" xmi:id=\"BOUML_0x1f4e7_22\" name =\"VEHA\">" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"Entity\" xmi:id=\"BOUML_0x1f467_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"BuiltIn\" xmi:id=\"BOUML_0x2c267_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t</packagedElement>" << endl;
	out << indent << "\t<packagedElement xmi:type=\"uml:Profile\" xmi:id=\"BOUML_0x1f567_22\" name=\"BEHAVE\">" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Enumeration\" name=\"Language\" xmi:id=\"BOUML_0x2b8e7_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t\t<ownedLiteral xmi:type=\"uml:EnumerationLiteral\" name=\"Soar\" xmi:id=\"BOUML_0x1fb67_1\">" << endl;
	out << indent << "\t\t\t</ownedLiteral>" << endl;
	out << indent << "\t\t\t<ownedLiteral xmi:type=\"uml:EnumerationLiteral\" name=\"Prolog\" xmi:id=\"BOUML_0x1fbe7_1\">" << endl;
	out << indent << "\t\t\t</ownedLiteral>" << endl;
	out << indent << "\t\t\t<ownedLiteral xmi:type=\"uml:EnumerationLiteral\" name=\"Clips\" xmi:id=\"BOUML_0x1fc67_1\">" << endl;
	out << indent << "\t\t\t</ownedLiteral>" << endl;
	out << indent << "\t\t\t<ownedLiteral xmi:type=\"uml:EnumerationLiteral\" name=\"Cpp\" xmi:id=\"BOUML_0x1fce7_1\">" << endl;
	out << indent << "\t\t\t</ownedLiteral>" << endl;
	out << indent << "\t\t\t<ownedLiteral xmi:type=\"uml:EnumerationLiteral\" name=\"Python\" xmi:id=\"BOUML_0x1fd67_1\">" << endl;
	out << indent << "\t\t\t</ownedLiteral>" << endl;
	out << indent << "\t\t\t<ownedLiteral xmi:type=\"uml:EnumerationLiteral\" name=\"Other\" xmi:id=\"BOUML_0x1fde7_1\">" << endl;
	out << indent << "\t\t\t</ownedLiteral>" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"Agent\" xmi:id=\"BOUML_0x2a467_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"EmboddiedAgent\" xmi:id=\"BOUML_0x2a567_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t\t<generalization xmi:type=\"uml:Generalization\" xmi:id=\"BOUML_0x1f667_0\" general=\"BOUML_0x2a467_4\"/>" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"DecisionBehavior\" xmi:id=\"BOUML_0x2a6e7_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t\t<ownedAttribute xmi:type=\"uml:Property\" name=\"language\" xmi:id=\"BOUML_0x1fae7_1\" visibility=\"protected\" isReadOnly=\"true\">" << endl;
	out << indent << "\t\t\t\t<type xmi:type=\"uml:Class\" xmi:idref=\"BOUML_0x2b8e7_4\"/>" << endl;
	out << indent << "\t\t\t\t<defaultValue xmi:type=\"uml:LiteralString\" xmi:id=\"BOUML_VALUE_0x1fae7_1\" value=\"Other\"/>" << endl;
	out << indent << "\t\t\t</ownedAttribute>" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"PerceptFilter\" xmi:id=\"BOUML_0x2a767_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t\t<ownedAttribute xmi:type=\"uml:Property\" name=\"frequency\" xmi:id=\"BOUML_0x1fa67_1\" visibility=\"protected\">" << endl;
	out << indent << "\t\t\t\t<type xmi:type=\"uml:Class\" xmi:idref=\"BOUML_datatype_0\"/>" << endl;
	out << indent << "\t\t\t\t<lowerValue xmi:type=\"uml:LiteralString\" xmi:id=\"BOUML_MULTIPLICITY_L_0x1fa67_1\" value=\"1\"/>" << endl;
	out << indent << "\t\t\t\t<upperValue xmi:type=\"uml:LiteralString\" xmi:id=\"BOUML_MULTIPLICITY_U_0x1fa67_1\" value=\"1\"/>" << endl;
	out << indent << "\t\t\t\t<defaultValue xmi:type=\"uml:LiteralString\" xmi:id=\"BOUML_VALUE_0x1fa67_1\" value=\"-1\"/>" << endl;
	out << indent << "\t\t\t\t<ownedComment xmi:type=\"uml:Comment\" xmi:id=\"COMMENT_2\" body=\"percept update frequency (in s^-1)&#10;-1 -&gt; updateWhenNeeded&#10;&#10;\"/>" << endl;
	out << indent << "\t\t\t</ownedAttribute>" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"ActionBehavior\" xmi:id=\"BOUML_0x2a7e7_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"Memory\" xmi:id=\"BOUML_0x2a867_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"PerceptionBehavior\" xmi:id=\"BOUML_0x2a8e7_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"Role\" xmi:id=\"BOUML_0x2a967_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t</packagedElement>" << endl;
	out << indent << "\t<packagedElement xmi:type=\"uml:Profile\" xmi:id=\"BOUML_0x1f5e7_22\" name =\"HAVE\">" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"HumanRole\" xmi:id=\"BOUML_0x2a9e7_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t\t<ownedComment xmi:type=\"uml:Comment\" xmi:id=\"COMMENT_3\" body=\"Les ActionAgtBehaviors de HumanRole sont des HumanActivity.\"/>" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"HumanInstance\" xmi:id=\"BOUML_0x2aa67_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"Human\" xmi:id=\"BOUML_0x2aae7_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t\t<ownedComment xmi:type=\"uml:Comment\" xmi:id=\"COMMENT_4\" body=\"Les ActionAgtBehaviors de Human sont des HumanActivity.\"/>" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"AvatarInstance\" xmi:id=\"BOUML_0x2ab67_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"Avatar\" xmi:id=\"BOUML_0x2abe7_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"VirtualHumanInstance\" xmi:id=\"BOUML_0x2ac67_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"VirtualHuman\" xmi:id=\"BOUML_0x2ace7_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"HumanActivity\" xmi:id=\"BOUML_0x2ad67_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t\t<packagedElement xmi:type=\"uml:Stereotype\" name=\"PrimitiveHumanAction\" xmi:id=\"BOUML_0x2ade7_4\" visibility=\"package\">" << endl;
	out << indent << "\t\t</packagedElement>" << endl;
	out << indent << "\t</packagedElement>" << endl;
	out << indent << "</packagedElement>" << endl;
}
string XMI20Writer::buildIndent()
{
	string indent="";
	for(size_t i=0;i<currentIndentation;i++)
		indent+="\t";
	return indent;
}
void XMI20Writer::setId(shared_ptr<Element> e)
{
	if(e->getId().empty())
		e->setId(IDCalculator::calculateID("test",e->getFullName()));
}
