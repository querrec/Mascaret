/*
 * Peripheric.h
 *
 *  Created on: 26 févr. 2011
 *      Author: ping
 */

#ifndef PERIPHERIC_H_
#define PERIPHERIC_H_
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include "Tools/veha_plateform.h"

namespace IEHA
{
using boost::signal;

class VEHA_API InteractionEventArgs
{
public:
	enum InteractionType
	{
		ButtonPressed,
		AxisChanged,
		CommunicationOccurs
	};

	InteractionType type;
};

class VEHA_API Peripheric : public enable_shared_from_this<Peripheric>
{
public:
	Peripheric(const string& name);
	virtual ~Peripheric();

	inline const string& getName() const;
	inline void setName(const string& name);

protected:
	string _name;
	signal<void (shared_ptr<Peripheric>, const InteractionEventArgs&)> _cbInteractionOccurs;
};
const string& Peripheric::getName() const
{
	return _name;
}
void Peripheric::setName(const string& name)
{
	_name=name;
}
}

#endif /* PERIPHERIC_H_ */
