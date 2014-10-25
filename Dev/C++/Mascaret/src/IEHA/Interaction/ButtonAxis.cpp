/*
 * ButtonAxis.cpp
 *
 *  Created on: 1 mars 2011
 *      Author: soler
 */

#include "IEHA/Interaction/ButtonAxis.h"

namespace IEHA
{
ButtonAxis::ButtonAxis(const string& name) : Axis(name)
{

}
ButtonAxis::~ButtonAxis()
{

}
void ButtonAxis::_onButtonPressed(shared_ptr<Button> button, bool pressed)
{
	for(unsigned int i=0;i<_buttonsInfos.size();i++)
	{
		if(_buttonsInfos[i].button == button && _buttonsInfos[i].pressed==pressed)
		{
			if(_buttonsInfos[i].type == IncreaseValue)
				_value+=_buttonsInfos[i].value;
			else
				_value=_buttonsInfos[i].value;
		}
	}
}
}
