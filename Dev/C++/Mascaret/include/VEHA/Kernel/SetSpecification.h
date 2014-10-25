#ifndef _v2_VEHA_Kernel_SetSpecification_H
#define _v2_VEHA_Kernel_SetSpecification_H
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
using namespace std;
#include "Tools/veha_plateform.h"
namespace VEHA
{
class ValueSpecification;

class VEHA_API SetSpecification
{
public :
	SetSpecification();
	virtual ~SetSpecification();
	// --- Attributes ---
	//------ isOrdered -----
protected :
	bool    _isOrdered;
public :
	inline bool getIsOrdered(void) { return _isOrdered;}
	inline void setIsOrdered(bool isOrdered) { _isOrdered = isOrdered;}
	//------ isUnique -----
protected :
	bool    _isUnique;
public :
	inline bool getIsUnique(void) { return _isUnique;}
	inline void setIsUnique(bool isUnique) { _isUnique = isUnique;}
	// --- Relations ---
	//------ maximumNumber -----
protected :
	ValueSpecification *  _maximumNumber;
public :
	inline ValueSpecification * getMaximumNumber(void) { return _maximumNumber;}
	inline void setMaximumNumber(ValueSpecification * maximumNumber) { _maximumNumber = maximumNumber;}
	//------ minimumNumber -----
protected :
	ValueSpecification *  _minimumNumber;
public :
	inline ValueSpecification * getMinimumNumber(void) { return _minimumNumber;}
	inline void setMinimumNumber(ValueSpecification * minimumNumber) { _minimumNumber = minimumNumber;}
	// --- Operations ---
};

}



#endif
