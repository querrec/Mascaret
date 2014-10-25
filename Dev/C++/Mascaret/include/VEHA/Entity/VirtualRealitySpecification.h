/*
 * VirtualRealitySpecification.h
 *
 *  Created on: 23 févr. 2011
 *      Author: soler
 */

#ifndef VIRTUALREALITYSPECIFICATION_H_
#define VIRTUALREALITYSPECIFICATION_H_
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/Classifier.h"
namespace VEHA
{

class VEHA_API VirtualRealitySpecification : public ValueSpecification
{
public:
	VirtualRealitySpecification(shared_ptr<Classifier> type);
	virtual ~VirtualRealitySpecification();
};

}
#endif /* VIRTUALREALITYSPECIFICATION_H_ */
