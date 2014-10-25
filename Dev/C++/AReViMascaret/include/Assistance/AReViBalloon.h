#ifndef AReViBalloon_H
#define AReViBalloon_H

#include "AReVi/Lib3D/object3D.h"
#include "AReVi/Shapes/shape3D.h"
#include "AReVi/Shapes/mesh3D.h"
#include "AReVi/Contrib/cairoTexture.h"

using namespace AReVi;
using namespace std;
namespace AReViMascaret
{
class CairoTextManager;
class AReViBalloon : public Object3D
{
	public :
		AR_CLASS (AReViBalloon)
		AR_CONSTRUCTOR (AReViBalloon)
		AR_CONSTRUCTOR_7 (AReViBalloon, string, text, double, height, double, width, double, posX, double, posY, double, posZ, double, oriZ)
		void displayText(string text);
		void show(void);
		void hide(void);
		void print();
		
	private :

		ArRef<Mesh3D> _mesh;
		ArRef<Shape3D> _shape;
		ArRef<CairoTexture> _ctexture;
		
		void _drawNewAReViBalloon();

	
		StlString _text;
		size_t _numPage;
		
		AReVi::ArRef<CairoTextManager> _textManager;
		ArRef<Light3D> _light;
		double _time;
};

ArRef <Transform3D> calculateTransformation (double p, double r, double y);

}
#endif
