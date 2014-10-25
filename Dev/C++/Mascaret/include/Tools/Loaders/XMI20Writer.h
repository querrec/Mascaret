/*
 * XMI20Writer.h
 *
 *  Created on: 7 févr. 2011
 *      Author: soler
 */

#ifndef XMI20WRITER_H_
#define XMI20WRITER_H_
#include <iostream>
#include "Tools/veha_plateform.h"

namespace VEHA
{
namespace Kernel
{
class Model;
class Package;
class Class;
class Property;
class Operation;
class Element;
class Parameter;
}
namespace Behavior
{
namespace Activity
{
class Activity;
class ActivityNode;
class ActivityEdge;
}
namespace StateMachine
{
class StateMachine;
}
}
}
using boost::shared_ptr;
using VEHA::Model;
using VEHA::Package;
using VEHA::Class;
using VEHA::Property;
using VEHA::Operation;
using VEHA::Parameter;
using VEHA::Element;
using VEHA::Activity;
using VEHA::ActivityNode;
using VEHA::ActivityEdge;
using VEHA::StateMachine;
class XMI20Writer
{
public:
	XMI20Writer();
	~XMI20Writer();

	void writeModel(shared_ptr<Model> m,const std::string& filename);
	void writeModel(shared_ptr<Model> m,std::ostream& filename);
	void writePackage(std::ostream& out, shared_ptr<Package> p,bool initialPackage=false);
	void writeClass(std::ostream& out, shared_ptr<Class> c);
	void writeAttribute(std::ostream& out, shared_ptr<Property> p);
	void writeOperation(std::ostream& out, shared_ptr<Operation> op);
	void writeParameter(std::ostream& out, shared_ptr<Parameter> p);
	void writeAssociation(std::ostream& out, shared_ptr<Property> p);
	void writeActivity(std::ostream& out, shared_ptr<Activity> act,const std::string& opId);
	void writeActivityNode(std::ostream& out,shared_ptr<ActivityNode> node);
	void writeStateMachine(std::ostream& out, shared_ptr<StateMachine> sm,const std::string& classifierId);
protected:
	void writeMascaretProfile(std::ostream& out);
	std::string buildIndent();
	void setId(shared_ptr<Element> e);
	size_t currentIndentation;
	int currentCommentNumber;
	std::map<std::string,shared_ptr<Activity> > _activities;
	std::map<std::string,shared_ptr<StateMachine> > _stateMachines;
};




#endif /* XMI20WRITER_H_ */
