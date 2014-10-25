/*
 * main.cpp
 *
 *  Created on: 28 févr. 2011
 *      Author: soler
 */
#include "AReVi/arSystem.h"
#include "AReViNS/networkService.h"
#include "VRPlateform/AReViMascaretApplication.h"
#include "hLib/hLib.h"

#ifdef USE_GTK
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include <gtk/gtk.h>
#endif


using namespace AReViMascaret;
using namespace AReVi;

namespace AnimLib
{
  void
  registerAnimLibClasses();
}

namespace AReViMascaret
{
  void
  registerAReViMascaretClasses();
}


void
registerArWidgetClasses(); 


ArRef<Scheduler>
simulationInit(void)
{

	ArRef<Scheduler> sched=RealTimeScheduler::NEW(1e-3);
	return(sched);
}

int main(int argc, char **argv) {
    AnimLib::registerAnimLibClasses();
    registerAReViNSClasses();
    registerArWidgetClasses();
    AReViMascaret::registerAReViMascaretClasses();

    ArSystem arevi(argc,argv);

    if(ArSystem::getCommandLine().size()<2)
    {
        cerr << "usage: " << ArSystem::getCommandLine()[0]
            << " fileName" << endl;
        return 0;
    }

    ArSystem::loadPlugin("MagickImageLoader");
    ArSystem::loadPlugin("XmlParser");
#ifdef _WIN32
    ArSystem::loadPlugin("SoundWaveAPI");
#else
	ArSystem::loadPlugin("Sound3DPlayer");
#endif
        hLibInit();
	ArSystem::enterSimulation(&simulationInit);



	AReViMascaretApplication appli;
	appli.parse(ArSystem::getCommandLine()[1]);
	appli.init();


#ifdef USE_GTK
	cerr << "Using GTK" << endl;
	// Interface GTK
	if(!gtk_init_check(&argc,&argv))
	{
		cerr << "Cannot initialize GTK" << endl;
		return(-1);
	}

	//if(ArSystem::enterSimulation(&simulationInit))
	{
		while(appli.step())
		{
			while(gtk_events_pending())
			{
				gtk_main_iteration();
			}
		}
	}
#else
	appli.run();
#endif
}
