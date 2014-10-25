#ifndef OGRERENDERTOTEXTURE_H_
#define OGRERENDERTOTEXTURE_H_
#include "ogreMascaret_plateform.h"
#include "IEHA/Visualisation/RenderToTexture.h"
#include <Ogre.h>

namespace OgreMascaret
{
using std::string;
using boost::shared_ptr;
class OGRE_MASCARET_API OgreRenderToTexture :
  public IEHA::RenderToTexture
{
public:
	OgreRenderToTexture();
	virtual ~OgreRenderToTexture();

	virtual void getPosition(unsigned int& xOut, unsigned int& yOut) const;
	virtual void setPosition(unsigned int x, unsigned int y);

	virtual void getSize(unsigned int& xOut, unsigned int& yOut) const;
	virtual void setSize(unsigned int x, unsigned int y);

  virtual void showText(string text, float ratio){}
  virtual void showTextWide(string text, float ratio){}
  virtual void showBalloon(string text,shared_ptr<VEHA::Entity> e){}

  virtual void showImage(string url){}
protected:
};
}
#endif /* OGRERENDERTOTEXTURE_H_ */
