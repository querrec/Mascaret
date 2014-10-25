/*
 * Navigation.h
 *
 *  Created on: 16 mars 2011
 *      Author: soler
 */

#ifndef NAVIGATION_H_
#define NAVIGATION_H_
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include "IEHA/Interaction/Axis.h"

namespace IEHA
{
class Camera;

class VEHA_API Navigation : public enable_shared_from_this<Navigation>
{
public:
	enum NavigationMode
	{
		HUMAN, // Only yaw is significative for translations
		FREE
	};
	enum NavigationAxis
	{
		UNSET,TX,TY,TZ,ROLL,PITCH,YAW
	};
	Navigation(shared_ptr<Camera> cam);
	virtual ~Navigation();

	virtual void addNavigationAxis(shared_ptr<Axis> axis, NavigationAxis navAxis, double speed);
	void removeAllNavigationAxes();

	virtual void process();

	void setNavigationMode(NavigationMode mode);
	NavigationMode getNavigationMode();

protected:
	struct NavInfo
	{
		NavigationAxis axis;
		double speed;
	};
	void _onAxisChanged(shared_ptr<Axis> axis,double);
	NavigationMode _navigationMode;
	map<shared_ptr<Axis>,NavInfo> _navigationInfos;
	map<shared_ptr<Axis>,boost::signals::connection > _axisConnections;
	vector<shared_ptr<Axis> > _needUpdate;
	double _lastTime;
  shared_ptr<Camera> _camera;
};
inline void Navigation::setNavigationMode(Navigation::NavigationMode mode)
{
	_navigationMode=mode;
}
inline Navigation::NavigationMode Navigation::getNavigationMode()
{
	return _navigationMode;
}
}
#endif /* NAVIGATION_H_ */
