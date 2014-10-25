/*
 * Scene.cpp
 *
 *  Created on: 27 févr. 2011
 *      Author: ping
 */

#include "IEHA/Visualisation/Scene.h"

namespace IEHA
{
Scene::Scene(const string& name) : _name(name), _fogColor(VEHA::Color(230,230,230)), _density(0.005)
{

}
Scene::~Scene()
{

}
}
