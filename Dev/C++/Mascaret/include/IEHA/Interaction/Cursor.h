/*
 * Cursor.h
 *
 *  Created on: 26 févr. 2011
 *      Author: ping
 */

#ifndef CURSOR_H_
#define CURSOR_H_
#include "Tools/veha_plateform.h"
#include "IEHA/Interaction/Axis.h"
#include "VEHA/Entity/Entity.h"
namespace IEHA
{
using VEHA::Entity;

class VEHA_API Cursor
{
public:
	Cursor(const string& name);
	virtual ~Cursor();

	const string& getName() const;
	void setName(const string& name);

	virtual void getPosition(int& x,int& y) const=0;

protected:
	string _name;
};
inline Cursor::Cursor(const string& name) : _name(name)
{
}
inline Cursor::~Cursor()
{
}
inline const string& Cursor::getName() const
{
	return _name;
}
inline void Cursor::setName(const string& name)
{
	_name=name;
}
}
#endif /* CURSOR_H_ */
