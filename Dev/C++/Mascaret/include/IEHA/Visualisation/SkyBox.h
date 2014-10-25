/*
 * Light.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef SKYBOX_H_
#define SKYBOX_H_
#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Vector3.h"

namespace IEHA
{
using VEHA::Vector3;
class VEHA_API SkyBox
{
public:

	inline Vector3 getPosition() const;
	inline Vector3 getOrientation() const;
	inline std::string getUrl() const;
	inline void setPosition(Vector3 location);
	inline void setOrientation(Vector3 location);
	inline void setUrl(std::string url);

protected:
	string _url;
	Vector3 _position;
	Vector3 _orientation;
};
Vector3 SkyBox::getPosition() const
{
	return _position;
}
Vector3 SkyBox::getOrientation() const
{
	return _orientation;
}
std::string SkyBox::getUrl() const
{
	return _url;
}
void SkyBox::setPosition(Vector3 position)
{
	_position=position;
}
void SkyBox::setOrientation(Vector3 orientation)
{
	_orientation=orientation;
}
void SkyBox::setUrl(std::string url)
{
	_url=url;
}
}

#endif /* SKYBOX_H_ */
