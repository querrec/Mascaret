/*
 * PaddlePeripheric.h
 *
 *  Created on: 26 févr. 2011
 *      Author: ping
 */

#ifndef PADDLEPERIPHERIC_H_
#define PADDLEPERIPHERIC_H_
#include "Tools/veha_plateform.h"

#include <boost/bind.hpp>
#include <boost/signal.hpp>
#include "IEHA/Interaction/Peripheric.h"
#include "IEHA/Interaction/Axis.h"
#include "IEHA/Interaction/Button.h"
#include "IEHA/Interaction/CartesianBasis.h"
namespace IEHA
{
class VEHA_API ButtonEventArgs : public InteractionEventArgs
{
public:
	inline ButtonEventArgs(shared_ptr<Button> button) : button(button)
	{
		type=InteractionEventArgs::ButtonPressed;
	}
	shared_ptr<Button> button;
};
class VEHA_API AxisEventArgs : public InteractionEventArgs
{
public:
	inline AxisEventArgs(shared_ptr<Axis> axis) : axis(axis)
	{
		type=InteractionEventArgs::AxisChanged;
	}
	shared_ptr<Axis> axis;
};
class VEHA_API PaddlePeripheric : public Peripheric
{
public:
	PaddlePeripheric(const string& name);
	virtual ~PaddlePeripheric();

	void addAxis(shared_ptr<Axis> axis);
	void removeAxis(shared_ptr<Axis> axis);
	void removeAxis(const string& axis);
	shared_ptr<Axis> getAxis(const string& name);
	vector<shared_ptr<Axis> > getAxes();

	void addButton(shared_ptr<Button> button);
	void removeButton(shared_ptr<Button> button);
	void removeButton(const string& button);
	shared_ptr<Button> getButton(const string& name);
	vector<shared_ptr<Button> > getButtons();

	void addBasis(shared_ptr<CartesianBasis> basis);
	void removeBasis(shared_ptr<CartesianBasis> basis);
	void removeBasis(const string& basis);
	shared_ptr<CartesianBasis> getBasis(const string& name);
	vector<shared_ptr<CartesianBasis> > getBases();
protected:
	void _onButtonPressed(shared_ptr<Button> button,bool pressed);
	void _onAxisValueChanged(shared_ptr<Axis> axis,double value);
	vector<shared_ptr<Axis> > _axes;
	vector<shared_ptr<Button> > _buttons;
	vector<shared_ptr<CartesianBasis> > _bases;
	map<shared_ptr<Axis>, boost::signals::connection> _axesConnections;
	map<shared_ptr<Button>, boost::signals::connection> _buttonsConnections;
};
inline void PaddlePeripheric::addAxis(shared_ptr<Axis> axis)
{
	_axes.push_back(axis);
	//_axesConnections[axis]=axis->addCallbackValueChanged(shared_dynamic_cast<PaddlePeripheric>(shared_from_this()),&PaddlePeripheric::_onAxisValueChanged);
}
inline void PaddlePeripheric::removeAxis(shared_ptr<Axis> axis)
{
	size_t i=0;
	for(;i<_axes.size();i++)
	{
		if(_axes[i] == axis)
			break;
	}
	if(i < _axes.size())
	{
		//_axesConnections[_axes[i]].disconnect();
		//_axesConnections.erase(_axes[i]);
		_axes[i]=_axes.back();
		_axes.pop_back();
	}
}
inline void PaddlePeripheric::removeAxis(const string& axis)
{
	size_t i=0;
	for(;i<_axes.size();i++)
	{
		if(_axes[i]->getName() == axis)
			break;
	}
	if(i < _axes.size())
	{
		//_axesConnections[_axes[i]].disconnect();
		//_axesConnections.erase(_axes[i]);
		_axes[i]=_axes.back();
		_axes.pop_back();
	}
}
inline shared_ptr<Axis> PaddlePeripheric::getAxis(const string& name)
{
	for(size_t i=0;i<_axes.size();i++)
	{
		if(_axes[i]->getName() == name)
                {
			return _axes[i];
                }
	}
	return shared_ptr<Axis>();
}
inline vector<shared_ptr<Axis> > PaddlePeripheric::getAxes()
{
	return _axes;
}

inline void PaddlePeripheric::addButton(shared_ptr<Button> button)
{
	_buttons.push_back(button);
	//_buttonsConnections[button]=button->addCallbackPressed(shared_dynamic_cast<PaddlePeripheric>(shared_from_this()),&PaddlePeripheric::_onButtonPressed);
}
inline void PaddlePeripheric::removeButton(shared_ptr<Button> button)
{
	size_t i=0;
	for(;i<_buttons.size();i++)
	{
		if(_buttons[i] == button)
			break;
	}
	if(i < _buttons.size())
	{
		//_buttonsConnections[_buttons[i]].disconnect();
		//_buttonsConnections.erase(_buttons[i]);
		_buttons[i]=_buttons.back();
		_buttons.pop_back();
	}
}
inline void PaddlePeripheric::removeButton(const string& button)
{
	size_t i=0;
	for(;i<_buttons.size();i++)
	{
		if(_buttons[i]->getName() == button)
			break;
	}
	if(i < _buttons.size())
	{
		//_buttonsConnections[_buttons[i]].disconnect();
		//_buttonsConnections.erase(_buttons[i]);
		_buttons[i]=_buttons.back();
		_buttons.pop_back();
	}
}
inline shared_ptr<Button> PaddlePeripheric::getButton(const string& name)
{
	for(size_t i=0;i<_buttons.size();i++)
	{
		if(_buttons[i]->getName() == name)
			return _buttons[i];
	}
	return shared_ptr<Button>();
}
inline vector<shared_ptr<Button> > PaddlePeripheric::getButtons()
{
	return _buttons;
}
inline void PaddlePeripheric::addBasis(shared_ptr<CartesianBasis> basis)
{
	_bases.push_back(basis);
}
inline void PaddlePeripheric::removeBasis(shared_ptr<CartesianBasis> basis)
{
	size_t i=0;
	for(;i<_bases.size();i++)
	{
		if(_bases[i] == basis)
			break;
	}
	if(i < _bases.size())
	{
		_bases[i]=_bases.back();
		_bases.pop_back();
	}
}
inline void PaddlePeripheric::removeBasis(const string& basis)
{
	size_t i=0;
	for(;i<_bases.size();i++)
	{
		if(_bases[i]->getName() == basis)
			break;
	}
	if(i < _bases.size())
	{
		_bases[i]=_bases.back();
		_bases.pop_back();
	}
}
inline shared_ptr<CartesianBasis> PaddlePeripheric::getBasis(const string& name)
{
	for(size_t i=0;i<_bases.size();i++)
	{
		if(_bases[i]->getName() == name)
			return _bases[i];
	}
	return shared_ptr<CartesianBasis>();
}
inline vector<shared_ptr<CartesianBasis> > PaddlePeripheric::getBases()
{
	return _bases;
}

}

#endif /* PADDLEPERIPHERIC_H_ */
