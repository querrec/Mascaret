#include <stdio.h>
#include <iostream>

#include "ogreMascaret.h"
#include "ogreInterface3D.h"

#include "VEHA/Behavior/Common/behaviorScheduler.h"

using namespace std;

OgreMascaret::OgreMascaret(string filename) : VEHAApplication()
{
	OgreInterface3D::init();	
	parse(filename);
	(shared_dynamic_cast<OgreInterface3D>(OgreInterface3D::get()))->setApplication(this);
	(shared_dynamic_cast<OgreInterface3D>(OgreInterface3D::get()))->getRoot()->startRendering();
}

OgreMascaret::~OgreMascaret()
{
}
bool OgreMascaret::step()
{
    BehaviorScheduler::getInstance()->step();
	return true;
}
void OgreMascaret::createDistributionServer(string sessionName, int port)
{

}
void OgreMascaret::createClient(string hostname, int port)
{

}
