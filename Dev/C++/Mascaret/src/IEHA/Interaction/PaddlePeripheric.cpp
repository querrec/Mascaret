/*
 * PaddlePeripheric.cpp
 *
 *  Created on: 1 mars 2011
 *      Author: soler
 */

#include "IEHA/Interaction/PaddlePeripheric.h"

namespace IEHA
{
PaddlePeripheric::PaddlePeripheric(const string& name) : Peripheric(name)
{

}
PaddlePeripheric::~PaddlePeripheric()
{

}
void PaddlePeripheric::_onButtonPressed(shared_ptr<Button> ,bool )
{

}
void PaddlePeripheric::_onAxisValueChanged(shared_ptr<Axis> ,double )
{

}
}
