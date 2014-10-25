#ifndef _v2_VEHA_Entity_Ogre_Color_H
#define _v2_VEHA_Entity_Ogre_Color_H

#include "VEHA/Entity/Color.h"
#include "ogreMascaret_plateform.h"


namespace OgreMascaret
{

class OGRE_MASCARET_API OgreColor : public VEHA::Color
{
public:
	OgreColor();
	OgreColor(float a);
	OgreColor(int r,int g,int b);

};

}
#endif
