#include "Assistance/AReViImage.h"
#include "AReVi/Lib3D/transform3D.h"
#include "AReVi/Lib3D/light3D.h"
#include "AReVi/Lib3D/scene3D.h"
#include "AReVi/Shapes/sphere3D.h"
#include "AReVi/Lib3D/viewer3D.h"
#include "AReVi/Lib3D/urlTexture.h"
#include "VRPlateform/AReViWindow3D.h"
#include "VRPlateform/AReViMascaretApplication.h"

namespace AReViMascaret
{
AR_CLASS_DEF(AReViImage,Object3D)

AReViImage::AReViImage(ArCW & arCW) : OSDSprite(arCW)
{	
}

AReViImage::AReViImage(ArCW &arCW, string url, double ratio,OSD::Location loc)
	:	OSDSprite(arCW)
{

	ArRef<URLTexture> texture = URLTexture::NEW(url,true,false);
	if (texture.valid())
	 {
	 cout<<"Texture "<<url;
	 if (texture->getAlphaTest())
	   { cout<<" uses alpha"<<endl; }
	 else
	   { cout<<" no alpha"<<endl; }
	 if (texture->getBlending())
	  {  cout<<" and needs blending"<<endl; }
	 else
	  {  cout<<" but no blending"<<endl; }

	setTransient(false);
	setLocation(OSD::LOC_BOTTOM_LEFT);
	setRatio(ratio);
	setScale(0.05,0.05);
	setTexture(texture);

//	ArPtr<Viewer3D> viewer = shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer();
	}


}

void AReViImage::show()
{
//	shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer()->addOSD(thisRef());
    vector< shared_ptr<IEHA::Window3D> > windows = AReViMascaretApplication::getInstance()->getWindows();
    for(int i = 0; i < windows.size(); i++)
    {
        shared_dynamic_cast<AReViWindow3D>(windows[i])->getAReViViewer()->addOSD(thisRef());
    }
}
void AReViImage::hide()
{
//	shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer()->removeOSD(thisRef());
    vector< shared_ptr<IEHA::Window3D> > windows = AReViMascaretApplication::getInstance()->getWindows();
    for(int i = 0; i < windows.size(); i++)
    {
        shared_dynamic_cast<AReViWindow3D>(windows[i])->getAReViViewer()->removeOSD(thisRef());
    }
	setTransient(true);
}


AReViImage::~AReViImage ()
{
	cerr << "AReViImage::~AReViImage ()"<<endl;
}

}
