/*
 * Navigation.cpp
 *
 *  Created on: 16 mars 2011
 *      Author: soler
 */
#include "IEHA/Interaction/Navigation.h"
#include "IEHA/Visualisation/Camera.h"
#include "IEHA/VRApplication.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Entity/RotationVector.h"
#ifdef _MSC_VER
#define M_PI 3.14159265358979323846
#endif
namespace IEHA
{
Navigation::Navigation(shared_ptr<Camera> cam)
: _navigationMode(HUMAN),
  _navigationInfos(),
  _axisConnections(),
  _needUpdate(),
  _camera(cam)
{
	_lastTime=BehaviorScheduler::getInstance()->getElapsedVirtualTime();
}
Navigation::~Navigation()
{
}

void Navigation::addNavigationAxis(shared_ptr<Axis> axis, NavigationAxis navAxis, double speed)
{
	_axisConnections[axis]=axis->addCallbackValueChanged(shared_from_this(),&Navigation::_onAxisChanged);
	NavInfo info;
	info.axis=navAxis;
	info.speed=speed;
	_navigationInfos[axis]=info;
}
void Navigation::removeAllNavigationAxes()
{
	for(map<shared_ptr<Axis>,boost::signals::connection >::iterator i=_axisConnections.begin();i!=_axisConnections.end();i++)
		i->second.disconnect();
	_axisConnections.clear();
	_navigationInfos.clear();
}
void Navigation::process()
{
	double time=BehaviorScheduler::getInstance()->getElapsedVirtualTime();
	double dt=time-_lastTime;
	_lastTime=time;
	if(_camera)
	{
		bool translate=false;
		bool rotate=false;
		Vector3 vec;
		Vector3 ori;//=cam->getLocation()->getLocalOrientation();
		map<shared_ptr<Axis>,NavInfo>::iterator it=_navigationInfos.begin();
		
		while(it!=_navigationInfos.end())
		{
			if(it->second.axis==TX)
			{
				
				double val=it->first->getValue();
				if(val != 0.0)
				{
					vec.x=val*it->second.speed*dt;
					vec.x=val*it->second.speed*0.1;
					translate=true;
				}
			}
			else if(it->second.axis==TY)
			{
				double val=it->first->getValue();
				if(val != 0.0)
				{
					vec.y=val*it->second.speed*dt;
					vec.y=val*it->second.speed*0.1;
					translate=true;
				}
			}
			else if(it->second.axis==TZ)
			{
				double val=it->first->getValue();
				if(val != 0.0)
				{
					vec.z=val*it->second.speed*dt;
					vec.z=val*it->second.speed*0.1;
					translate=true;
				}
			}
			if(it->second.axis==ROLL)
			{
				double val=it->first->getValue();
				if(val != 0.0)
				{
					ori.x=val*it->second.speed*dt;
					ori.x=val*it->second.speed*0.1;
					rotate=true;
				}
			}
			else if(it->second.axis==PITCH)
			{
				double val=it->first->getValue();
				if(val != 0.0)
				{
					ori.y=val*it->second.speed*dt;
					ori.y=val*it->second.speed*0.1;
					/*if(ori.y>M_PI/2)
						ori.y=M_PI/2-0.001;
					else if(ori.y<-M_PI/2)
						ori.y=-M_PI/2+0.001;*/
					rotate=true;
				}
			}
			else if(it->second.axis==YAW)
			{
				double val=it->first->getValue();
				if(val != 0.0)
				{
					ori.z=val*it->second.speed*dt;
					ori.z=val*it->second.speed*0.1;
					rotate=true;
				}
			}
			it++;
		}
		if(translate)
		{
			_camera->translate(vec,_navigationMode);
		}
		if(rotate)
		{
			_camera->rotate(ori,_navigationMode);
		}
	}
	_needUpdate.clear();
}
void Navigation::_onAxisChanged(shared_ptr<Axis> axis,double /*value*/)
{
	_needUpdate.push_back(axis);
}
}
