/*
 * BodyClass.h
 *
 *  Created on: 8 mars 2011
 *      Author: soler
 */

#ifndef BODYCLASS_H_
#define BODYCLASS_H_
#include "Tools/veha_plateform.h"
#include "VEHA/Entity/EntityClass.h"
#include "HAVE/VirtualHumanClass.h"

namespace HAVE
{
using VEHA::Property;
using VEHA::Operation;
class VEHA_API BodyClass : public VEHA::EntityClass
{
public:
	BodyClass(const string& name, VirtualHumanClass* ownerClass);
	virtual ~BodyClass();

	virtual inline map<string, shared_ptr<Operation> > getOperations() const
	{
		map<string, shared_ptr<Operation> > ops=_ownerClass->getOperations();
		ops.insert(_operations.begin(),_operations.end());
		return ops;
	}
	virtual inline bool hasOperation(const string& name) {return _operations.find(name) != _operations.end() || _ownerClass->hasOperation(name);}
	virtual inline map<string, shared_ptr<Property> > getAttributes() const
	{
		map<string, shared_ptr<Property> > atts=_ownerClass->getAttributes();
		atts.insert(_attributes.begin(),_attributes.end());
		return atts;
	}
	virtual inline bool hasAttribute(const string& name) {return _attributes.find(name) != _attributes.end() || _ownerClass->hasAttribute(name);}
protected:
	VirtualHumanClass* _ownerClass;
};
}
#endif /* BODYCLASS_H_ */
