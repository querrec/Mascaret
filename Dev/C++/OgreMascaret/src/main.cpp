/*
 * main.cpp
 *
 *  Created on: 28 févr. 2011
 *      Author: soler
 */
#ifdef _WIN32
#include <winsock2.h>
#endif
#include "VRPlateform/OgreMascaretApplication.h"

using namespace OgreMascaret;



int main(int argc, char **argv) {
    if(argc<2)
    {
        cerr << "usage: " << argv[0]
            << " fileName" << endl;
        return 0;
    }
    OgreMascaretApplication appli;
    appli.init();
    ((MascaretApplication*)&appli)->parse(argv[1]);

    appli.run();
}
