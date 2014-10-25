/*
 * ButtonAxis.h
 *
 *  Created on: 26 févr. 2011
 *      Author: ping
 */

#ifndef BUTTONAXIS_H_
#define BUTTONAXIS_H_
#include "Tools/veha_plateform.h"
#include "IEHA/Interaction/Axis.h"
#include "IEHA/Interaction/Button.h"

namespace IEHA
{

class VEHA_API ButtonAxis : public Axis
{
public:
	enum ButtonType
	{
		IncreaseValue,
		SetValue
	};
	ButtonAxis(const string& name);
	virtual ~ButtonAxis();

	void addButton(shared_ptr<Button> but,bool pressed, ButtonType type,double value);
protected:

	typedef struct _ButtonInfos
	{
		shared_ptr<Button> button;
		bool pressed;
		ButtonType type;
		double value;
	} ButtonInfos;
	void _onButtonPressed(shared_ptr<Button> button, bool pressed);
	vector<ButtonInfos> _buttonsInfos;
};
inline void ButtonAxis::addButton(shared_ptr<Button> but, bool pressed,ButtonType type,double value)
{
	ButtonInfos infos;
	infos.button=but;
	infos.pressed=pressed;
	infos.type=type;
	infos.value=value;
	_buttonsInfos.push_back(infos);
	but->addCallbackPressed(shared_dynamic_cast<ButtonAxis>(shared_from_this()),&ButtonAxis::_onButtonPressed);
}
}
#endif /* BUTTONAXIS_H_ */
