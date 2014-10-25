#ifndef _v2_VEHA_Behavior_Common_BehaviorExecution_H
#define _v2_VEHA_Behavior_Common_BehaviorExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Time/TimeExpression.h"

#include "VEHA/Behavior/Common/Parameters.h"
#include <boost/signal.hpp>

namespace VEHA
{
class InstanceSpecification;
class ValueSpecification;
class TimeExpression;
class Behavior;

class VEHA_API BehaviorExecution : public enable_shared_from_this<BehaviorExecution>
{
public :
	BehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync = true);
	virtual ~BehaviorExecution();
	// --- Attributes ---
	// --- Relations ---
	//------ specification -----
protected :
	weak_ptr<Behavior>    _specification;
	weak_ptr<InstanceSpecification> _host;

        bool _sync;
public :
        inline bool getSync() {return _sync;}
	inline shared_ptr<InstanceSpecification>  getHost(){return _host.lock();}
	inline shared_ptr<Behavior> getSpecification(void) { return _specification.lock();}
	//		inline void setSpecification(shared_ptr<Behavior> specification) { _specification = specification;}
	//------ start -----
protected :
	TimeExpression    _start;
public :
	inline const TimeExpression& getStart(void) { return _start;}
	inline void setStart(const TimeExpression& start) { _start = start;}
	//------ finish -----
protected :
	TimeExpression    _finish;
	bool _isFinished;

	bool _isSuspended;
    TimeExpression _lastCalledTime;
public :
	inline const TimeExpression& getFinish(void) { return _finish;}
	inline void setFinish(const TimeExpression& finish) { _finish = finish; }
	virtual void stop(void);
	inline bool isFinished(){return _isFinished;}
	virtual void restart(void);
	virtual void pause(void);
	inline bool isSuspended(){return _isSuspended;}
    inline void setLastCallTime(TimeExpression time) { _lastCalledTime = time; }
    inline TimeExpression getLastCallTime() { return _lastCalledTime; }
	// --- Operations ---
	virtual double execute(double dt)=0;

	Parameters parameters;
	vector<shared_ptr<ValueSpecification> >result;
public :
	void addCallbackOnBehaviorStart(const boost::function1<void, BehaviorExecution*> & f);
	void removeCallbackOnBehaviorStart(const boost::function1<void, BehaviorExecution*>& f);
	void addCallbackOnBehaviorStop(const boost::function1<void, BehaviorExecution*> & f);
	void removeCallbackOnBehaviorStop(const boost::function1<void, BehaviorExecution*>& f);
	void addCallbackOnBehaviorSuspended(const boost::function1<void, BehaviorExecution*> & f);
	void removeCallbackOnBehaviorSuspended(const boost::function1<void, BehaviorExecution*>& f);
	void addCallbackOnBehaviorRestarted(const boost::function1<void, BehaviorExecution*> & f);
	void removeCallbackOnBehaviorRestarted(const boost::function1<void, BehaviorExecution*>& f);
protected :
	boost::signal<void (BehaviorExecution*)> _cbOnBehaviorStart;
	boost::signal<void (BehaviorExecution*)> _cbOnBehaviorStop;
	boost::signal<void (BehaviorExecution*)> _cbOnBehaviorSuspended;
	boost::signal<void (BehaviorExecution*)> _cbOnBehaviorRestarted;



};

}
#endif
