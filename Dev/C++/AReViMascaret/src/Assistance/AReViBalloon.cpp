#include "Assistance/AReViBalloon.h"
#include "AReVi/Lib3D/transform3D.h"
#include "AReVi/Lib3D/light3D.h"
#include "AReVi/Lib3D/scene3D.h"
#include "Assistance/CairoTextManager.h"
#include "VRPlateform/AReViWindow3D.h"
#include "VRPlateform/AReViMascaretApplication.h"

namespace AReViMascaret
{
AR_CLASS_DEF(AReViBalloon,Object3D)

AReViBalloon::AReViBalloon(ArCW & arCW) : Object3D(arCW)
{	
}

AReViBalloon::AReViBalloon(ArCW &arCW, string text, double height, double width, double posX, double posY, double posZ, double /*oriZ*/)
	:	Object3D(arCW)
{
	setTransient(false);
	/* Instanciations */
	_mesh = Mesh3D::NEW();
	_shape = Shape3D::NEW();
	cerr << height << " "<<width << endl;
	_ctexture = CairoTexture::NEW(1024*height, 1024* width);
	_textManager=CairoTextManager::NEW(_ctexture);
	_textManager->setFontSize(200);

	/* Configuration */ 
	_ctexture->setLineWidth(12);
	_ctexture->setLineCap(CairoTexture::LINE_CAP_ROUND);

	double sx = height / 2;
	double sy = width / 2;

	/* Création mesh */
	_mesh->accessVertices().push_back(Util3D::Dbl3(-sx,-sy,0));
	_mesh->accessVertices().push_back(Util3D::Dbl3(+sx,-sy,0));
	_mesh->accessVertices().push_back(Util3D::Dbl3(+sx,+sy,0));
	_mesh->accessVertices().push_back(Util3D::Dbl3(-sx,+sy,0));
	_mesh->accessTextures().push_back(Util3D::Dbl2(0.0,0.0));
	_mesh->accessTextures().push_back(Util3D::Dbl2(1.0,0.0));
	_mesh->accessTextures().push_back(Util3D::Dbl2(1.0,1.0));
	_mesh->accessTextures().push_back(Util3D::Dbl2(0.0,1.0));
	_mesh->accessVertexFaces().push_back(Util3D::Face(0,1,3));
	_mesh->accessVertexFaces().push_back(Util3D::Face(1,2,3));
	_mesh->computeDefaultNormals(true);
	if (!_mesh->applyChanges(true)) 
		cerr << "ERREUR MESH ...." << endl;

	/* Orientation correcte mesh */
	ArRef<Transform3D> tr = calculateTransformation (1.57,1.57,0);

	_mesh->writeTransformation(tr);

	/* Lien entre objets */
	_shape->addRootPart(_mesh);
	_mesh->setTexture(_ctexture);
	_mesh->setTextureRepeat(false,false);
	
	/*----Dessin de la bulle et du texte----*/
	_text = text;

	/*-------Lien avec la shape--------*/
	setShape(_shape); 
	_shape->setBillboard(true,false);
	
	/*--------Placement de la bulle au-dessus du personnage----------*/
	translate(posX,posY,posZ);
	
	_light=Light3D::NEW();
	_light->setDirectional(false);
//	_light->setLocation(shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer());
//	_light->attachTo(shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer());
    vector< shared_ptr<IEHA::Window3D> > windows = AReViMascaretApplication::getInstance()->getWindows();
    for(int i = 0; i < windows.size(); i++)
    {
        _light->setLocation(shared_dynamic_cast<AReViWindow3D>(windows[i])->getAReViViewer());
        _light->attachTo(shared_dynamic_cast<AReViWindow3D>(windows[i])->getAReViViewer());
    }

	/*--------Affichage de la bulle------*/
	show();

}

void AReViBalloon::hide()
{
//	shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer()->accessFrontScene()->removeObject(thisRef());
//	shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer()->accessFrontScene()->removeLight(_light);
    vector< shared_ptr<IEHA::Window3D> > windows = AReViMascaretApplication::getInstance()->getWindows();
    for(int i = 0; i < windows.size(); i++)
    {
        shared_dynamic_cast<AReViWindow3D>(windows[i])->getAReViViewer()->accessFrontScene()->removeObject(thisRef());
        shared_dynamic_cast<AReViWindow3D>(windows[i])->getAReViViewer()->accessFrontScene()->removeLight(_light);
    }
	setTransient(true);
}

void AReViBalloon::show()
{
//	shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer()->accessFrontScene()->addObject(thisRef());
//	shared_dynamic_cast<AReViWindow3D>(AReViMascaretApplication::getInstance()->accessWindow())->getAReViViewer()->accessFrontScene()->addLight(_light);
    vector< shared_ptr<IEHA::Window3D> > windows = AReViMascaretApplication::getInstance()->getWindows();
    for(int i = 0; i < windows.size(); i++)
    {
        shared_dynamic_cast<AReViWindow3D>(windows[i])->getAReViViewer()->accessFrontScene()->addObject(thisRef());
        shared_dynamic_cast<AReViWindow3D>(windows[i])->getAReViViewer()->accessFrontScene()->addLight(_light);
    }
}


void AReViBalloon::displayText(string text)
{
	if (text != "")
	{
   		_text = text;
	}

	cout<<"TEXTE : "<<_text<<" = "<<text<<endl;

	show();
	print();

}

AReViBalloon::~AReViBalloon ()
{
	cerr << "AReViBalloon::~AReViBalloon ()"<<endl;
}


void 
AReViBalloon::_drawNewAReViBalloon()
{
	unsigned int w, h;
	_ctexture->clear();
	_ctexture->getSize(w, h);
	_ctexture->setSourceColor(1.0,1.0,1.0,1.0);

	_ctexture->curveTo(0,h/10,0,0,w/10,0);
	_ctexture->lineTo(w-w/10,0);
	_ctexture->curveTo(w-w/10,0,w,0,w,h/10);
	_ctexture->lineTo(w,h-2*h/10);
	_ctexture->curveTo(w,h-2*h/10,w,h-h/10,w-w/10,h-h/10);
	_ctexture->lineTo(w/2+w/15,h-h/10);
	_ctexture->curveTo(w/2+w/15,h-h/10,w/2+w/15,h,w/2,h);	
	_ctexture->curveTo(w/2,h,w/2,h-h/10,w/2-w/15,h-h/10);	
	_ctexture->lineTo(w/10,h-h/10);
	_ctexture->curveTo(w/10,h-h/10,0,h-h/10,0,h-2*h/10);
	_ctexture->lineTo(0,h/10);
	_ctexture->fill();
	_ctexture->setSourceColor(0.0,1.0,1.0,1.0);
}


void AReViBalloon::print()
{
	unsigned int w, h;
	_ctexture->getSize(w, h);
	//----Dessin de la bulle adaptable----
	_drawNewAReViBalloon();

	bool finished=!_textManager->printPage(_text,_numPage++);
	//----Affichage du rendu---
	_ctexture->render();
}



ArRef<Transform3D> calculateTransformation(double p, double r, double y){
    ArRef<Transform3D> tr = Transform3D::NEW();
    tr->prePitch(p);
    tr->preRoll(r);
    tr->preYaw(y);
    return tr;
}
}
