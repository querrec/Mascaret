#include <iostream>

#include "ogreMascaret.h"

#include "VEHA/Behavior/Common/pluginManager.h"

using namespace std;

int main(int argc, char ** argv)
{
   if (argc !=2 )
      cerr << "usage : " << argv[0] << " applicationFilename" << endl;
   else
   {
      //PluginManager::getInstance()->openPlugin("./plugins.so");
      OgreMascaret * appli = new OgreMascaret(argv[1]);
      appli->run();
   }
   return 1;
}
