/*
 * AReViShapeSpecification.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef OGREANIMATIONSPECIFICATION_H_
#define OGREANIMATIONSPECIFICATION_H_

#include "ogreMascaret_plateform.h"
#include "VEHA/Entity/AnimationSpecification.h"
#include "Animation/OgreAnimation.h"
#include <Ogre.h>

namespace OgreMascaret
{
using boost::shared_ptr;
using std::string;
using VEHA::ShapeSpecification;
using Ogre::FrameEvent;


class OGRE_MASCARET_API OgreAnimationSpecification : public VEHA::AnimationSpecification, public Ogre::FrameListener
{
public:
	OgreAnimationSpecification(const string & name, const string & url, int sens=1, double speed=1.0, bool cycle=false);
	virtual ~OgreAnimationSpecification();

	virtual void setShape(shared_ptr<ShapeSpecification> shape);
	virtual shared_ptr<ShapeSpecification> getShape() const;
	virtual bool play(double speed=1.0, int sens=1, bool cycle=false);
	virtual bool stop();

        virtual bool frameEnded(const FrameEvent& evt);


protected:
	void _setPosition(double position);
	double _time;
	int _sens;
	double _speed;
	bool _cycle;
	shared_ptr<OgreShapeSpecification> _shape;
        string _ogreAnimName;
        Ogre::AnimationState* _anim;
};
}
#endif /* AREVIANIMATIONSPECIFICATION_H_ */
