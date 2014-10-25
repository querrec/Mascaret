/*
 * BodyClass.cpp
 *
 *  Created on: 8 mars 2011
 *      Author: soler
 */

#include "HAVE/BodyClass.h"

using namespace VEHA;
namespace HAVE
{
BodyClass::BodyClass(const string& name, VirtualHumanClass* ownerClass) : EntityClass(name), _ownerClass(ownerClass)
{

}
BodyClass::~BodyClass()
{

}
}
