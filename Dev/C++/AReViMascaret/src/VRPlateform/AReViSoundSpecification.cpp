/*
 * AReViSoundSpecification.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */

#include "VRPlateform/AReViSoundSpecification.h"
#include "VRPlateform/AReViPointSpecification.h"
#include "VRPlateform/AReViWindow3D.h"
#include "VRPlateform/AReViMascaretApplication.h"

using namespace AReVi;
using namespace VEHA;
namespace AReViMascaret
{
AReViSoundSpecification::AReViSoundSpecification(const string & name, const string & url, double gain, double minDistance, double maxDistance, bool cycle)
: SoundSpecification(name,url,gain,minDistance,maxDistance,cycle), _sound(URLSound3D::NEW(url,true))
{
	_sound->setGain(gain);
	_sound->setAttenuated(true);
	_sound->setDistanceAttenuation(minDistance,maxDistance);
	_sound->setRepeat(cycle);
}
AReViSoundSpecification::~AReViSoundSpecification()
{

}

void AReViSoundSpecification::attachTo(shared_ptr<PointSpecification> point)
{
	_sound->setLocation(shared_dynamic_cast<AReViPointSpecification>(point)->_point);
	_sound->attachTo(shared_dynamic_cast<AReViPointSpecification>(point)->_point);
}

bool AReViSoundSpecification::play(double gain, double minDistance, double maxDistance, bool cycle)
{
	if(gain != -1)
		_sound->setGain(gain);

	if(minDistance != -1 && maxDistance != -1)
		_sound->setDistanceAttenuation(minDistance,maxDistance);

	_sound->setRepeat(cycle);
	_sound->play();

	return _sound->fail();
}
bool AReViSoundSpecification::play(double gain, bool cycle)
{
	if(gain != -1)
		_sound->setGain(gain);

	_sound->setRepeat(cycle);
	_sound->play();

	return _sound->fail();
}
bool AReViSoundSpecification::stop()
{
	if(!_sound->isStopped())
	{
		_sound->stop();
		return true;
	}
	return false;
}
}
