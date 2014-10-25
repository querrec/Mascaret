#ifndef _v2_VEHA_Entity_Animation_H
#define _v2_VEHA_Entity_Animation_H
#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/VirtualRealitySpecification.h"


namespace VEHA
{
class ShapeSpecification;
class VEHA_API AnimationSpecification : public VirtualRealitySpecification
{
public :
	AnimationSpecification(const std::string & name, const std::string & url);
	virtual ~AnimationSpecification();
	inline std::string getName() { return _name;}
	inline std::string getUrl() { return _url;}

	virtual void setShape(shared_ptr<ShapeSpecification> shape)=0;
	virtual shared_ptr<ShapeSpecification> getShape() const=0;
	virtual bool play(double speed=1.0, int sens=1, bool cycle=false)=0;
	virtual bool stop()=0;

	virtual shared_ptr<ValueSpecification> clone() const;

	template <class T1,class T2>
	inline boost::signals::connection addCallbackOnPlayStarted(shared_ptr<T1> instance,void (T2::*method)(shared_ptr<AnimationSpecification>));
	template <class T1,class T2>
	inline boost::signals::connection addCallbackOnPlayFinished(shared_ptr<T1> instance,void (T2::*method)(shared_ptr<AnimationSpecification>));
	template <class T1,class T2>
	inline boost::signals::connection addCallbackOnAnimationPosition(shared_ptr<T1> instance,void (T2::*method)(shared_ptr<AnimationSpecification>, double));

protected:
	inline void firePlayStarted();
	inline void firePlayFinished();
	inline void fireAnimationPosition(double position);
private :
	std::string _name;
	std::string _url;
    boost::signal<void (shared_ptr<AnimationSpecification>)> _cbOnPlayStarted;
	boost::signal<void (shared_ptr<AnimationSpecification>)> _cbOnPlayFinished;
    boost::signal<void (shared_ptr<AnimationSpecification>, double)> _cbOnAnimationPosition;
};
template <class T1,class T2>
boost::signals::connection AnimationSpecification::addCallbackOnPlayStarted(shared_ptr<T1> instance,void (T2::*method)(shared_ptr<AnimationSpecification>))
{
   	T2 * dummy1=(T1 *)0; // ensure T1 inherits from T2
   	(void)dummy1;
   	return _cbOnPlayStarted.connect(bind(method,instance,_1));
}
void AnimationSpecification::firePlayStarted()
{
	_cbOnPlayStarted(shared_dynamic_cast<AnimationSpecification>(shared_from_this()));
}
template <class T1,class T2>
boost::signals::connection AnimationSpecification::addCallbackOnPlayFinished(shared_ptr<T1> instance,void (T2::*method)(shared_ptr<AnimationSpecification>))
{
   	T2 * dummy1=(T1 *)0; // ensure T1 inherits from T2
   	(void)dummy1;
   	return _cbOnPlayFinished.connect(bind(method,instance,_1));
}
void AnimationSpecification::firePlayFinished()
{
	_cbOnPlayFinished(shared_dynamic_cast<AnimationSpecification>(shared_from_this()));
}
template <class T1,class T2>
boost::signals::connection AnimationSpecification::addCallbackOnAnimationPosition(shared_ptr<T1> instance,void (T2::*method)(shared_ptr<AnimationSpecification>,double))
{
   	T2 * dummy1=(T1 *)0; // ensure T1 inherits from T2
   	(void)dummy1;
   	return _cbOnAnimationPosition.connect(bind(method,instance,_1, _2));
}
void AnimationSpecification::fireAnimationPosition(double position)
{
	_cbOnAnimationPosition(shared_dynamic_cast<AnimationSpecification>(shared_from_this()), position);
}
}
#endif
