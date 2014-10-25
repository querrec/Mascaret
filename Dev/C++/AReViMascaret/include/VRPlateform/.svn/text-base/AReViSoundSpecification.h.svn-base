/*
 * AReViShapeSpecification.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef AREVISOUNDSPECIFICATION_H_
#define AREVISOUNDSPECIFICATION_H_
#include "areviMascaret_plateform.h"
#include "VEHA/Entity/SoundSpecification.h"
#include "AReVi/Lib3D/urlSound3D.h"
namespace AReViMascaret
{
using AReVi::ArRef;
using AReVi::URLSound3D;
using std::string;
using boost::shared_ptr;
using VEHA::PointSpecification;
class AREVI_MASCARET_API AReViSoundSpecification : public VEHA::SoundSpecification
{
public:
	AReViSoundSpecification(const string & name, const string & url, double gain = 1.0, double minDistance = 1.0, double maxDistance = 100.0, bool cycle=false);
	virtual ~AReViSoundSpecification();

	virtual void attachTo(shared_ptr<PointSpecification> point);
	virtual bool play(double gain = 1.0, double minDistance = 1.0, double maxDistance = 100.0, bool cycle=false);
	virtual bool play(double gain = 1.0, bool cycle=false);
	virtual bool stop();

	ArRef<URLSound3D> getSound();
protected:
	ArRef<URLSound3D> _sound;
};
inline ArRef<URLSound3D> AReViSoundSpecification::getSound()
{
	return _sound;
}
}
#endif /* AREVISOUNDSPECIFICATION_H_ */
