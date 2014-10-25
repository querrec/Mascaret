/*
 * CartesianBasis.h
 *
 *  Created on: 15 mars 2011
 *      Author: soler
 */

#ifndef CARTESIANBASIS_H_
#define CARTESIANBASIS_H_
#include "Tools/veha_plateform.h"
namespace IEHA
{

class Axis;
class VEHA_API CartesianBasis : public enable_shared_from_this<CartesianBasis>
{
public:
	enum AXES
	{
		TX,
		TY,
		TZ,
		YAW,
		PITCH,
		ROLL
	};
	CartesianBasis(const string& name);
	virtual ~CartesianBasis();

	const string& getName() const;
	void setName(const string& name);

	void setAxis(AXES axisName,shared_ptr<Axis> axis);
protected:
	string _name;
	map<AXES, shared_ptr<Axis> > _axes;
};
inline const string& CartesianBasis::getName() const
{
	return _name;
}
inline void CartesianBasis::setName(const string& name)
{
	_name=name;
}
inline void CartesianBasis::setAxis(AXES axisName,shared_ptr<Axis> axis)
{
	_axes[axisName]=axis;
}
}


#endif /* CARTESIANBASIS_H_ */
