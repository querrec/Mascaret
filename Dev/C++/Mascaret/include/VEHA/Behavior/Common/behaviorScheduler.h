#ifndef BEHAVIOR_SCHEDULER_H
#define BEHAVIOR_SCHEDULER_H

#include <list>
#include <vector>
#include <set>

#include "Tools/utils.h"
#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Time/TimeExpression.h"

namespace VEHA
{
class Behavior;
class BehaviorExecution;

struct SchedInfo
{
	SchedInfo(shared_ptr<BehaviorExecution>b,TimeExpression time):be(b),nextTime(time){}
	shared_ptr<BehaviorExecution> be;
	TimeExpression nextTime;

	inline bool operator<(SchedInfo const &other) const
	{
		return nextTime < other.nextTime;
	}
};

class VEHA_API BehaviorScheduler
{
public:
	static inline shared_ptr<BehaviorScheduler> getInstance();
	static void destroyInstance();
	virtual ~BehaviorScheduler();
protected:
	BehaviorScheduler();
	static shared_ptr<BehaviorScheduler> _instance;
public:
	void step();
	shared_ptr<BehaviorExecution> executeBehavior(shared_ptr<Behavior> ,shared_ptr<InstanceSpecification> host,const Parameters& p,bool sync=false );
	void suspendExecutionBehavior(shared_ptr<BehaviorExecution> be);
	void deleteExecutionBehavior(shared_ptr<BehaviorExecution> be);
	void reActivate(shared_ptr<BehaviorExecution> be);
	void _insert(shared_ptr<BehaviorExecution> be, double dt);
	inline std::set<shared_ptr<BehaviorExecution> > getAllBehaviorExecutions();
public:
	inline bool isSuspended() const {return _suspended;}
	inline double getAccelerationFactor() const {return _virtualTimeSpeedFactor;}
	/** Temps réel passé */
	inline double getElapsedRealTime() const {return TimeExpression::createTimeOfDay() - _startRealTime - _totalSuspendedRealTime;}
	/** Temps simulé passé */
	inline double getElapsedVirtualTime() const {return _currentVirtualTime - _startVirtualTime;}

	void setSuspended(bool);
	void setAccelerationFactor(double);
	void forceCurrentVirtualTime(TimeExpression); /* Changer la date dans la simulation */

	/** Date dans la simulation */
	PRE_DEPRECATED double getCurrentTime() POST_DEPRECATED;
	/** Date dans la simulation */
	TimeExpression getCurrentVirtualTime();
protected:
	bool _suspended;
	double _virtualTimeSpeedFactor;
	TimeExpression _startRealTime;
	double _totalSuspendedRealTime;
	TimeExpression _lastRealTimeCheck;
	TimeExpression _startVirtualTime;
	TimeExpression _currentVirtualTime;
	TimeExpression _startSuspendTime;
protected:
	void _updateCurrentVirtualTime();
	void _jumpToNext();
	void _activate();
	void _onBehaviorRestarted(BehaviorExecution* be);
protected:
	int _cycle;
	/** List of currently executable behaviors */
	std::list<SchedInfo> _behaviors;
	std::vector<shared_ptr<BehaviorExecution> > _suspendedBehaviors;
	std::set<shared_ptr<BehaviorExecution> > _allBehaviors;
};

inline shared_ptr<BehaviorScheduler> BehaviorScheduler::getInstance()
{
	if (!_instance)
		_instance=shared_ptr<BehaviorScheduler>(new BehaviorScheduler());
	return _instance;
}

inline std::set<shared_ptr<BehaviorExecution> > BehaviorScheduler::getAllBehaviorExecutions()
{
	return _allBehaviors;
}
}
#endif
