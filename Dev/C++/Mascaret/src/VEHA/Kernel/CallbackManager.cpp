#include <iostream>
#include "VEHA/Kernel/CallbackManager.h"

//static CallbackManager_changesMethod * _instance=NULL;
namespace VEHA {
map<void*,vector<boost::signal0<void> *> > CallbackManager_changesMethod::_signals;
}
