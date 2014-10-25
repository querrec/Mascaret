/*
 * AReViAnimationSpecification.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */
#include "AReVi/Contrib/xmlParser.h"
#include "AnimLib/utils.h"

#include "VRPlateform/AReViAnimationSpecification.h"
#include "Animation/HLibAnimation.h"
#include "Animation/ObjectAnimation.h"

using namespace VEHA;
using namespace hLib;
using namespace AnimLib;
namespace AReViMascaret
{

AReViAnimationSpecification::AReViAnimationSpecification(const string & name, const string & url, int sens, double speed, bool cycle)
: AnimationSpecification(name,url), _time(0.0), _sens(sens), _speed(speed), _cycle(cycle)
{
	_act = AnimationActivity::NEW(0.04,this);
	_act->setSuspended(true);
	boost::filesystem::path path1(url);
	string extension=boost::filesystem::extension(path1);
	boost::to_lower(extension);
	if (extension == ".smd")
	{
		ArRef<hLib::Animation> hanimation = HLibLoader::loadAnimation(url);
		ArRef<HLibAnimation> animation = HLibAnimation::NEW();
		animation->setName(name);
		animation->setHLibAnimation(hanimation);
		animation->setURL(url);
		_animation=animation;
	}
	if (extension == ".md5anim")
	{
		ArRef<hLib::Animation> hanimation = HLibLoader::loadAnimation(url);
		ArRef<HLibAnimation> animation = HLibAnimation::NEW();
		animation->setName(name);
		animation->setHLibAnimation(hanimation);
		animation->setURL(url);
		_animation=animation;
	}
	else if (extension == ".anim")
	{
		ArRef<XmlParser> parser = XmlParser::NEW();
		if (!parser->parseFile(url))
			cerr << " Error parsing : " << url << endl;

		ArRef<AReVi::XmlNode> children = parser->getRoot();
		if (!children) cerr << "ERREUR....." << endl;

		ArRef<ObjectAnimation> animation = ObjectAnimation::NEW();
		animation->setName(name);
		animation->setURL(url);

		StlMap<StlString, ArRef<ArKeyFrameTrack> > tracks;
		if (AnimLibUtils::readTracks(children,tracks))
		{
			StlMap<StlString, ArRef<ArKeyFrameTrack> >::iterator it = tracks.begin();
			while(it != tracks.end())
			{
				animation->addTrack(it->second);
				it ++;
			}
		}
	}
}
AReViAnimationSpecification::~AReViAnimationSpecification()
{

}

void AReViAnimationSpecification::setShape(shared_ptr<ShapeSpecification> shape)
{
	_shape=shared_dynamic_cast<AReViShapeSpecification>(shape);
	_animation->setShape(_shape);
}
shared_ptr<ShapeSpecification> AReViAnimationSpecification::getShape() const
{
	return _shape;
}
bool AReViAnimationSpecification::play(double speed, int sens, bool cycle)
{

	if(speed != 1.0)
		_speed=1.0;
	if(sens != 1)
		_sens=sens;
	if(cycle)
		_cycle=cycle;
//_speed = 1;
//_cycle = true;
_cycle = cycle;
_sens = sens;
_speed = speed;
_time=0;

	_act->setSuspended(false);
	firePlayStarted();
	return true;
}
bool AReViAnimationSpecification::stop()
{
	_act->setSuspended(true);
	_time = 0;
	firePlayFinished();
	return true;
}
void AReViAnimationSpecification::_setPosition(double position)
{
	fireAnimationPosition(position);
	if(!_shape->getObject3D()->isCurrentlyDisplayed())
		return;
	_animation->setPosition(position);
	/*vector< shared_ptr<Entity> > children=_shape->getEntity()->getChildren();
	for (unsigned int i = 0; i < children.size(); i++)
	{
		children[i]->setAnimationPosition(animationName, position);
	}*/
}
bool AReViAnimationSpecification::_activityAnimation(ArRef<Activity> /*act*/, double dt)
{
	//cerr << _speed  << " : " << _time << " / " << dt << " : " << _animation->getDuration()<< endl;
	if (dt>1) dt/=100;
	if (_time<= _animation->getDuration())
	{
		_setPosition(_time);
		_time+=(dt)*_speed;
		return true;
	}
	else
	{
		if (_cycle)
		{
			if (_sens) _time = 0;
			else _time = _animation->getDuration();

			_setPosition(_time);
			_time+=(dt)*_speed;

			return true;
		}
		else
		{
			_time = _animation->getDuration();
			_setPosition(_time-0.01);
			_time = 0.0;
			stop();
			return true;
		}
	}
}


AR_CLASS_NOVOID_DEF(AnimationActivity,Activity)
AnimationActivity::AnimationActivity(ArCW& arCW, double interval,AReViAnimationSpecification* animation) : Activity(arCW,interval), _animation(animation)
{
	setBehavior(thisRef(),&AnimationActivity::_activityAnimation);
}
AnimationActivity::~AnimationActivity()
{
}
bool AnimationActivity::_activityAnimation(ArRef<Activity> act, double dt)
{
	return _animation->_activityAnimation(act,dt);
}


}
