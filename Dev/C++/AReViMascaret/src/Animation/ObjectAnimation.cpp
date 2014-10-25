#include "AnimLib/arTrackBase3D.h"
#include "AnimLib/arTrackRotation.h"
#include "AnimLib/arTrackTranslation.h"
#include "Animation/ObjectAnimation.h"

using namespace AReVi;
using namespace AnimLib;
namespace AReViMascaret
{
AR_CLASS_DEF(ObjectAnimation,AReViAnimation)

ObjectAnimation::ObjectAnimation(ArCW & arCW) : AReViAnimation(arCW)
{
}

ObjectAnimation::~ObjectAnimation()
{
}

void ObjectAnimation::addTrack(ArRef<ArKeyFrameTrack> track)
{
	_tracks.push_back(track);
	ArRef<Base3D> reference;
	if (_shape->getEntity()->getParent())
	{
		shared_ptr<AReViShapeSpecification> parentShape=shared_dynamic_cast<AReViShapeSpecification>(_shape->getEntity()->getParent()->getActiveShape());
		reference=parentShape->getObject3D();
	}
	else
	{
		reference = _shape->getObject3D();
	}
	if (track->getClass()->isA(ArTrackRotation::CLASS()))
	{
		ar_down_cast<ArTrackRotation>(track)->initRotation(reference,_shape->getObject3D());
	}
	if (track->getClass()->isA(ArTrackTranslation::CLASS()))
	{
		ar_down_cast<ArTrackTranslation>(track)->initTranslation(reference,_shape->getObject3D());
	}
	if (track->getClass()->isA(ArTrackBase3D::CLASS()))
	{
		ar_down_cast<ArTrackBase3D>(track)->initBase3D(reference,_shape->getObject3D());
	}
}

void ObjectAnimation::setPosition(double position)
{
	for (unsigned int i = 0; i < _tracks.size(); i++)
		_tracks[i]->setPosition(position);
}

double ObjectAnimation::getDuration(void)
{
	double maxDuration = 0;
	for (unsigned int i = 0; i < _tracks.size() ; i++)
	{
		double currentDuration = _tracks[i]->getDuration();
		if (currentDuration> maxDuration) maxDuration = currentDuration;
	}
	return maxDuration;
}

}
