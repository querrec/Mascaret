/*
 * AReViShapeSpecification.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef OGRESOUNDSPECIFICATION_H_
#define OGRESOUNDSPECIFICATION_H_
#include "ogreMascaret_plateform.h"
#include "VEHA/Entity/SoundSpecification.h"
#include "Tools/Sound3D.h"
namespace OgreMascaret
{
using std::string;
using boost::shared_ptr;
using Sound::URLSound3D;
using VEHA::PointSpecification;
class OGRE_MASCARET_API OgreSoundSpecification : public VEHA::SoundSpecification
{
public:
	OgreSoundSpecification(const string & name, const string & url, double gain = 1.0, double minDistance = 1.0, double maxDistance = 100.0, bool cycle=false);
	virtual ~OgreSoundSpecification();

	virtual void attachTo(shared_ptr<PointSpecification> point);
	virtual bool play(double gain = 1.0, double minDistance = 1.0, double maxDistance = 100.0, bool cycle=false);
	virtual bool play(double gain = 1.0, bool cycle=false);
	virtual bool stop();

	shared_ptr<URLSound3D> getSound();
protected:
	shared_ptr<URLSound3D> _sound;
};
inline shared_ptr<URLSound3D> OgreSoundSpecification::getSound()
{
	return _sound;
}
}
#endif /* AREVISOUNDSPECIFICATION_H_ */
