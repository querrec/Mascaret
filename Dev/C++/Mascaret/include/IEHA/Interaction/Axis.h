/*
 * Axe.h
 *
 *  Created on: 26 févr. 2011
 *      Author: ping
 */

#ifndef AXIS_H_
#define AXIS_H_
#include "Tools/veha_plateform.h"
#include <boost/bind.hpp>
#include <boost/signal.hpp>
namespace IEHA
{
using boost::signal;
class VEHA_API Axis : public enable_shared_from_this<Axis>
{
public:
	Axis(const string& name);
	virtual ~Axis();

	const string& getName() const;
	void setName(const string& name);

	double getValue() const;
	void setValue(double val);

	template<class T1, class T2>
	boost::signals::connection addCallbackValueChanged(shared_ptr<T1> obj,void (T2::*method)(shared_ptr<Axis>,double));

protected:
	void fireValueChanged();
	string _name;
	double _value;
private:
	signal<void (shared_ptr<Axis>, double)> _cbValueChanged;
};
inline const string& Axis::getName() const
{
	return _name;
}
inline void Axis::setName(const string& name)
{
	_name=name;
}

inline double Axis::getValue() const
{
	return _value;
}
inline void Axis::setValue(double val)
{
	if(val != _value)
	{
		fireValueChanged();
		_value=val;
	}
}
template<class T1, class T2>
inline boost::signals::connection Axis::addCallbackValueChanged(shared_ptr<T1> obj,void (T2::*method)(shared_ptr<Axis>,double))
{
   	T2 * dummy1=(T1 *)0; // ensure T1 inherits from T2
   	(void)dummy1;
   	return _cbValueChanged.connect(bind(method,obj,_1,_2));
}
inline void Axis::fireValueChanged()
{
	_cbValueChanged(shared_from_this(),getValue());
}

}
#endif /* AXIS_H_ */
