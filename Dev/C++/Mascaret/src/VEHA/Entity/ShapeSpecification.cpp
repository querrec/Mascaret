#include "MascaretApplication.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/PointSpecification.h"
#include <stdexcept>
#include "VEHA/Kernel/PrimitiveType.h"
#include "Tools/utils.h"

namespace VEHA
{
ShapeSpecification::ShapeSpecification(const string& name) //protected
:VirtualRealitySpecification(MascaretApplication::getInstance()->getModel()->getBasicType("shape")),_name(name)
{
}
ShapeSpecification::ShapeSpecification(const string& name, const string & url, bool movable,bool recursive,const string& shader)
:VirtualRealitySpecification(MascaretApplication::getInstance()->getModel()->getBasicType("shape")),_name(name)
{
	_url=url;
}
ShapeSpecification::ShapeSpecification(const string& name, const vector<double>& distances,const vector<string> & urls, bool movable,bool recursive,const string& shader)
:VirtualRealitySpecification(MascaretApplication::getInstance()->getModel()->getBasicType("shape")),_name(name)
{
	if(urls.size()>0)
	{
		_url=urls[0];
	}
}
ShapeSpecification::~ShapeSpecification()
{
}

const ValueSpecification& ShapeSpecification::operator= (const ValueSpecification& vs)
{
	throw std::runtime_error("can't clone a ShapeSpecification");
	if (vs.getType() != getType())
		throw std::bad_cast();
	const ShapeSpecification & p= dynamic_cast<const ShapeSpecification&>(vs);
	setScale(p.getScale());
	_url=p.getUrl();
	return *this;
}
shared_ptr<ValueSpecification> ShapeSpecification::clone() const
{
	//throw std::runtime_error("can't clone a ShapeSpecification");
	//	shared_ptr<ShapeSpecification> p(new ShapeSpecification());
	//	p->setPosition(getPosition());
	//	p->setRotation(getRotation());
	//	setScale(p.getScale());
	//	setBody(p.getBody());
	//	return p;
	return shared_ptr<ValueSpecification>();
}

}
