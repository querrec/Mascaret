#include "AReVi/Contrib/cairoTexture.h"
#include "Assistance/CairoTextManager.h"

#include "VRPlateform/AReViWindow3D.h"
#include "VRPlateform/AReViMascaretApplication.h"
#include "Assistance/AReViOSDDisplay.h"

using namespace AReVi;
using namespace AReViMascaret;
using std::cerr;
using std::endl;
AR_CLASS_DEF(AReViOSDDisplay,OSDSprite)

namespace AReViMascaret
{

AReViOSDDisplay::AReViOSDDisplay(ArCW& arCW)
: OSDSprite(arCW,CairoTexture::NEW(128*3, 128* 1))
{
	setTransient(false);
	_cairoTexture=ar_const_cast(ar_down_cast<CairoTexture>(_texture));
	_textManager=CairoTextManager::NEW(_cairoTexture);
	setLocation(OSD::LOC_BOTTOM_CENTER);
	//setSize(30,10);
	setRatio(3);
	setScale(1,1);
}
AReViOSDDisplay::~AReViOSDDisplay()
{
}

void AReViOSDDisplay::show()
{
//	shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer()->addOSD(thisRef());
    vector< shared_ptr<IEHA::Window3D> > windows = AReViMascaretApplication::getInstance()->getWindows();
    for(int i = 0; i < windows.size(); i++)
    {
        shared_dynamic_cast<AReViWindow3D>(windows[i])->getAReViViewer()->addOSD(thisRef());
    }
}
void AReViOSDDisplay::hide()
{
//	shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer()->removeOSD(thisRef());
    vector< shared_ptr<IEHA::Window3D> > windows = AReViMascaretApplication::getInstance()->getWindows();
    for(int i = 0; i < windows.size(); i++)
    {
        shared_dynamic_cast<AReViWindow3D>(windows[i])->getAReViViewer()->removeOSD(thisRef());
    }
	setTransient(true);
}

void AReViOSDDisplay::displayText(string text,double ratio,OSD::Location loc)
{
	_numPage=0;
	_text=text;
	setLocation(loc);
	setRatio(ratio);
	show();
	print();
}

void AReViOSDDisplay::print()
{
	unsigned int w, h;
	bool finished=true;
	_cairoTexture->getSize(w, h);
	_cairoTexture->clear();

	_cairoTexture->setSourceColor(1,1,0.3,0.90);
	_cairoTexture->rectangle(0,0,w,h);
	_cairoTexture->fill();

	_cairoTexture->setSourceColor(0.0,0.0,0.0,1);
	_cairoTexture->rectangle(0,0,w,h);
	_cairoTexture->stroke();

	_cairoTexture->setSourceColor(0.0,0.0,0.0,1.0);
	finished=!_textManager->printPage(_text,_numPage++);
	_cairoTexture->render();
}

}
