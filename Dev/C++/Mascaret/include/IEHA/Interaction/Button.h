/*
 * Button.h
 *
 *  Created on: 26 févr. 2011
 *      Author: soler
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include "Tools/veha_plateform.h"

namespace IEHA
{
using boost::signal;
class VEHA_API Button : public enable_shared_from_this<Button>
{
public:
	Button(const string& name);
	virtual ~Button();

	const string& getName() const;
	void setName(const string& name);

	bool getPressed();
	void setPressed(bool val);

	template<class T1, class T2>
	boost::signals::connection addCallbackPressed(shared_ptr<T1> obj,void (T2::*method)(shared_ptr<Button>,bool));

protected:
	void firePressedChanged();
	string _name;
	bool _pressed;
private:
	signal<void (shared_ptr<Button>, bool)> _cbPressedChanged;
};
inline const string& Button::getName() const
{
	return _name;
}
inline void Button::setName(const string& name)
{
	_name=name;
}

inline bool Button::getPressed()
{
	return _pressed;
}
inline void Button::setPressed(bool val)
{
	_pressed=val;
	firePressedChanged();
}
template<class T1, class T2>
inline boost::signals::connection Button::addCallbackPressed(shared_ptr<T1> obj,void (T2::*method)(shared_ptr<Button>,bool))
{
   	T2 * dummy1=(T1 *)0; // ensure T1 inherits from T2
   	(void)dummy1;
   	return _cbPressedChanged.connect(boost::bind(method,obj,_1,_2));
}
inline void Button::firePressedChanged()
{
	_cbPressedChanged(shared_from_this(),getPressed());
}
}

#endif /* BUTTON_H_ */
