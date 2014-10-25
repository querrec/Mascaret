/*
 * OgreSoundSpecification.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */
#ifdef _WIN32
#include <winsock2.h>
#endif
#include "VRPlateform/OgreSoundSpecification.h"
#include "VRPlateform/OgrePointSpecification.h"

using namespace VEHA;
namespace OgreMascaret
{
OgreSoundSpecification::OgreSoundSpecification(const string & name, const string & url, double gain, double minDistance, double maxDistance, bool cycle)
    : SoundSpecification(name,url,gain,minDistance,maxDistance,cycle),
    _sound(make_shared<Sound::URLSound3D>(url))
{
	_sound->setGain(gain);
	_sound->setAttenuated(false); // TODO: Pour le moment
	//_sound->setAttenuated(true);
	_sound->setDistanceAttenuation(minDistance,maxDistance);
	_sound->setRepeat(cycle);
}
OgreSoundSpecification::~OgreSoundSpecification()
{

}

void OgreSoundSpecification::attachTo(shared_ptr<PointSpecification> point)
{
#if 0
	cerr << "Sound " << getName() << " attachTo ";
	cerr << "x: " << (shared_dynamic_cast<OgrePointSpecification>(point)->getGlobalPosition()).x;
	cerr << "x: " << (shared_dynamic_cast<OgrePointSpecification>(point)->getNode()->_getDerivedPosition()).x;
	cerr << endl;
#endif
    // TODO
	_sound->setPosition(shared_dynamic_cast<OgrePointSpecification>(point)->getGlobalPosition());
}

bool OgreSoundSpecification::play(double gain, double minDistance, double maxDistance, bool cycle)
{
	if(gain != -1)
		_sound->setGain(gain);

	if(minDistance != -1 && maxDistance != -1)
		_sound->setDistanceAttenuation(minDistance,maxDistance);

	_sound->setRepeat(cycle);
	_sound->play();
  
	return false; // TODO
}

bool OgreSoundSpecification::play(double gain, bool cycle)
{
	if(gain != -1)
		_sound->setGain(gain);

	_sound->setRepeat(cycle);
	_sound->play();

	return false; // TODO
}

bool OgreSoundSpecification::stop()
{
	if(!_sound->isStopped())
	{
		_sound->stop();
		return true;
	}
	return false;
}
}
