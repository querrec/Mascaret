#ifndef PARSERHELPER_H_
#define PARSERHELPER_H_

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/RotationVector.h"
#include "VEHA/Entity/Color.h"
#include "VEHA/Entity/Entity.h"

class XmlNode;
using boost::shared_ptr;
using std::string;

VEHA::Vector3 PositionVectorFromNodeProperties(boost::shared_ptr<XmlNode> node);
VEHA::Vector3 OrientationVectorFromNodeProperties(boost::shared_ptr<XmlNode> node);
VEHA::RotationVector RotationVectorFromNodeProperties(boost::shared_ptr<XmlNode> node);

VEHA::Color ColorFromNodeProperties(boost::shared_ptr<XmlNode> node);

VEHA::Axe AxeFromNodeProperty(boost::shared_ptr<XmlNode> node, string const & propertyName);

#endif /* PARSERHELPER_H_ */
