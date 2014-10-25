#ifndef AREVIOSDDISPLAY_H
#define AREVIOSDDISPLAY_H
#include "AReVi/Lib3D/osdSprite.h"
#include "AReVi/Contrib/cairoTexture.h"

using namespace AReVi;
namespace AReViMascaret
{
class CairoTextManager;
class AReViOSDDisplay : public AReVi::OSDSprite
{
public:
	AR_CLASS (AReViOSDDisplay)
	AR_CONSTRUCTOR (AReViOSDDisplay)
	void show();
	void hide();
	void displayText(string text, double ratio,OSD::Location loc);
	void print();

protected:

	AReVi::ArRef<AReVi::CairoTexture> _cairoTexture;
	AReVi::ArRef<CairoTextManager> _textManager;
	StlString _text;
	int _numPage;
};
}
#endif
