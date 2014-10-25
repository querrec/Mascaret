/*
 * SetInRedAction.h
 *
 *  Created on: 18 Juillet. 2011
 *      Author: querrec
 */

#ifndef CHANGEVIEWPOINTACTION_H_
#define CHANGEVIEWPOINTACTION_H_

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace IEHA{class Camera;}
namespace VEHA
{

class VEHA_API ChangeViewPointAction : public Action
{
public :
	ChangeViewPointAction();
	virtual ~ChangeViewPointAction();

protected :
  shared_ptr<IEHA::Camera> _camera;
	string _viewPointName;
	string _viewPointDirection;
public :
	inline shared_ptr<IEHA::Camera> getCamera() { return _camera;}
	inline void setCamera(shared_ptr<IEHA::Camera> camera) { _camera=camera;}
	inline string getViewPointName() { return _viewPointName;}
	inline void setViewPointName(string viewPointName) { _viewPointName=viewPointName;}
	inline string getViewPointDirection() { return _viewPointDirection;}
	inline void setViewPointDirection(string viewPointDirection) { _viewPointDirection=viewPointDirection;}

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);

};

}


#endif /* SETINREDACTION_H_ */
