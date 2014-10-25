#ifndef AReViImage_H
#define AReViImage_H

#include "AReVi/Lib3D/object3D.h"
#include "AReVi/Shapes/shape3D.h"
#include "AReVi/Lib3D/osdSprite.h"
#include "AReVi/Shapes/mesh3D.h"
#include "AReVi/Contrib/cairoTexture.h"

using namespace AReVi;
using namespace std;
namespace AReViMascaret
{
class CairoTextManager;
class AReViImage : public AReVi::OSDSprite
{
	public :
		AR_CLASS (AReViImage)
		AR_CONSTRUCTOR (AReViImage)
		AR_CONSTRUCTOR_3 (AReViImage, string, url, double, ratio,OSD::Location, loc)
		void show(void);
		void hide(void);
		
	private :

		ArRef<Mesh3D> _mesh;
		ArRef<Shape3D> _shape;

};

}
#endif
