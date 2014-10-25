#include "VEHA/Kernel/PrimitiveType.h"
#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Kernel/LiteralString.h"
#include "VEHA/Kernel/LiteralBoolean.h"

#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/Color.h"
#include "VEHA/Entity/RotationVector.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Entity/PointSpecification.h"
#include "VEHA/Entity/PathSpecification.h"
#include "VEHA/Entity/SoundSpecification.h"
#include "VEHA/Entity/AnimationSpecification.h"

#include "VEHA/Entity/VirtualRealityComponentFactory.h"
#include "MascaretApplication.h"

namespace VEHA
{

PrimitiveType::PrimitiveType(const string& name)
:Classifier(name),_type(name)
{
}

PrimitiveType::~PrimitiveType()
{
}

shared_ptr<ValueSpecification> PrimitiveType::createValueFromString(const string & str)
{
	shared_ptr<ValueSpecification> value;
	try
	{
		if (_type == "real" || _type == "double" )
			value = make_shared<LiteralReal>(str);
		else if (_type == "integer")
			value = make_shared<LiteralInteger>(str);
		else if (_type == "string")
			value = make_shared<LiteralString>(str);
		else if (_type =="boolean")
			value = make_shared<LiteralBoolean>(str);
		else if (_type =="color")
			value = make_shared<Color>(str);
		else if (_type =="vector")
			value = make_shared<Vector3>(str);
		else if (_type =="rotation")
			value = make_shared<RotationVector>(str);
		else if (_type =="shape")
			value = MascaretApplication::getInstance()->getVRComponentFactory()->createShapeFromString(str);
		else if (_type =="point")
			value = MascaretApplication::getInstance()->getVRComponentFactory()->createPointFromString(str);
		else if (_type =="path")
			value = make_shared<PathSpecification>(str);
		else if (_type =="sound")
			value = MascaretApplication::getInstance()->getVRComponentFactory()->createSoundFromString(str);
		else if (_type =="animation")
			value = MascaretApplication::getInstance()->getVRComponentFactory()->createAnimationFromString(str);
		//TODO les autres
		else
		{
			cerr << "createValueFromString Default: " << _type << " unknown, value is string" << endl;
			value = make_shared<LiteralString>(str);
		}
	}
	catch(std::exception& e)
	{
		cerr << "createValueFromString Error: " << str << " of type " << _type << endl;
		throw;
	}
	return value;
}

}
