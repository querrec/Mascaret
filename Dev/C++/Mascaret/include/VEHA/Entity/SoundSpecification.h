#ifndef _v2_VEHA_Entity_Sound_H
#define _v2_VEHA_Entity_Sound_H

#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/VirtualRealitySpecification.h"

namespace VEHA
{
class PointSpecification;
class VEHA_API SoundSpecification : public VirtualRealitySpecification
{
public:
	SoundSpecification(const std::string & name, const std::string & url, double gain = 1.0, double minDistance = 1.0, double maxDistance = 100.0, bool cycle=false);
	virtual ~SoundSpecification();

	inline std::string getName() { return _name;}
	inline double getGain() {return _gain;}
	inline double getMinDistance() {return _minDistance;}
	inline double getMaxDistance() {return _maxDistance;}

	virtual void attachTo(shared_ptr<PointSpecification> point)=0;
	virtual bool play(double gain = 1.0, double minDistance = 1.0, double maxDistance = 100.0, bool cycle=false)=0;
	virtual bool play(double gain = 1.0, bool cycle=false) = 0;
	virtual bool stop()=0;

	template <class T1,class T2>
	inline boost::signals::connection addCallbackOnPlayFinished(shared_ptr<T1> instance,void (T2::*method)(shared_ptr<SoundSpecification>));
	inline void firePlayFinished();
	virtual shared_ptr<ValueSpecification> clone() const;

private :
	std::string _name;
	double _gain;
	double _minDistance;
	double _maxDistance;
	bool _cycle;
	boost::signal<void (shared_ptr<SoundSpecification>)> _cbOnPlayFinished;
};

template <class T1,class T2>
boost::signals::connection SoundSpecification::addCallbackOnPlayFinished(shared_ptr<T1> instance,void (T2::*method)(shared_ptr<SoundSpecification>))
{
   	T2 * dummy1=(T1 *)0; // ensure T1 inherits from T2
   	(void)dummy1;
   	return _cbOnPlayFinished.connect(bind(method,instance,_1));
}
void SoundSpecification::firePlayFinished()
{
	_cbOnPlayFinished(shared_dynamic_cast<SoundSpecification>(shared_from_this()));
}

}
#endif
