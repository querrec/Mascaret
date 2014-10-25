#include "Tools/Loaders/ParserHelper.h"
#include "Tools/Loaders/XmlParser.h"
#include "VEHA/Entity/Entity.h"

VEHA::Vector3 PositionVectorFromNodeProperties(shared_ptr<XmlNode> node)
{
	double x,y,z;
	node->getPropertyReal("x",x);
	node->getPropertyReal("y",y);
	node->getPropertyReal("z",z);
	return VEHA::Vector3(x,y,z);
}

VEHA::Vector3 OrientationVectorFromNodeProperties(shared_ptr<XmlNode> node)
{
	double x,y,z;
	node->getPropertyReal("roll",x);
	node->getPropertyReal("pitch",y);
	node->getPropertyReal("yaw",z);
	return VEHA::Vector3(x,y,z);
}

VEHA::RotationVector RotationVectorFromNodeProperties(shared_ptr<XmlNode> node)
{
	double x,y,z,angle;
	node->getPropertyReal("x",x);
	node->getPropertyReal("y",y);
	node->getPropertyReal("z",z);
	node->getPropertyReal("angle",angle);
	return VEHA::RotationVector(x,y,z,angle);
}

VEHA::Color ColorFromNodeProperties(shared_ptr<XmlNode> node)
{
	double r,g,b;
	node->getPropertyReal("r",r);
	node->getPropertyReal("g",g);
	node->getPropertyReal("b",b);
	return VEHA::Color(r,g,b);
}

VEHA::Axe AxeFromNodeProperty(shared_ptr<XmlNode> node, string const & propertyName)
{
	string value;
	node->getPropertyString(propertyName, value);
	if (value == "X") return VEHA::X;
	if (value == "Y") return VEHA::Y;
	if (value == "Z") return VEHA::Z;
	if (value == "-X") return VEHA::minusX;
	if (value == "-Y") return VEHA::minusY;
	if (value == "-Z") return VEHA::minusZ;
	//TODO: Exception erreur valeur axe ?
}
