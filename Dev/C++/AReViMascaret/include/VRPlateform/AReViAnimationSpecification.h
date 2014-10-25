/*
 * AReViShapeSpecification.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef AREVIANIMATIONSPECIFICATION_H_
#define AREVIANIMATIONSPECIFICATION_H_
#include "AReVi/activity.h"
#include "areviMascaret_plateform.h"
#include "VEHA/Entity/AnimationSpecification.h"
#include "Animation/AReViAnimation.h"


namespace AReViMascaret
{
using boost::shared_ptr;
using std::string;
using VEHA::ShapeSpecification;
using VEHA::AnimationSpecification;
using AReVi::ArRef;
using AReVi::Activity;

class AREVI_MASCARET_API AReViAnimationSpecification : public AnimationSpecification
{
public:
	AReViAnimationSpecification(const string & name, const string & url, int sens=1, double speed=1.0, bool cycle=false);
	virtual ~AReViAnimationSpecification();

	virtual void setShape(shared_ptr<ShapeSpecification> shape);
	virtual shared_ptr<ShapeSpecification> getShape() const;
	virtual bool play(double speed=1.0, int sens=1, bool cycle=true);
	virtual bool stop();
protected:
	friend class AnimationActivity;
	bool _activityAnimation(ArRef<Activity> act, double dt);
	void _setPosition(double position);
	ArRef<Activity> _act;
	double _time;
	int _sens;
	double _speed;
	bool _cycle;
	shared_ptr<AReViShapeSpecification> _shape;
	ArRef<AReViAnimation> _animation;
};


class AREVI_MASCARET_API AnimationActivity : public Activity
{
public:
	AR_CLASS(AnimationActivity)
	AR_CONSTRUCTOR_2(AnimationActivity,double,interval,AReViAnimationSpecification*,animation)
protected:
	bool _activityAnimation(ArRef<Activity> act, double dt);
	AReViAnimationSpecification* _animation;
};

}
#endif /* AREVIANIMATIONSPECIFICATION_H_ */
