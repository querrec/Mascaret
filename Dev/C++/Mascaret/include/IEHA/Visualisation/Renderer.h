#ifndef RENDERER_H_
#define RENDERER_H_

#include "Tools/veha_plateform.h"
//#include "VEHA/Entity/Entity.h"
//#include "VEHA/Entity/Vector3.h"
//#include "VEHA/Kernel/Environment.h"

namespace VEHA{
  class Entity;
}
namespace IEHA
{

class Camera;
class Navigation;
class VEHA_API Renderer
{
public:
	Renderer();
	virtual ~Renderer();

	virtual void getPosition(unsigned int& xOut, unsigned int& yOut) const=0;
	virtual void setPosition(unsigned int x, unsigned int y)=0;

	virtual void getSize(unsigned int& xOut, unsigned int& yOut) const=0;
	virtual void setSize(unsigned int x, unsigned int y)=0;

	inline const shared_ptr<Camera> getCamera() const {return _camera;}
	inline shared_ptr<Camera> accessCamera() {return _camera;}
	inline void setCamera(shared_ptr<Camera> cam) {_camera=cam;}

	virtual void showText(string text, float ratio)=0;
  virtual void showTextWide(string text, float ratio)=0;
  virtual void showBalloon(string text,shared_ptr<VEHA::Entity> e)=0;
	virtual void showImage(string url)=0;

  inline shared_ptr<Navigation> getNavigation();
  inline void setNavigation(shared_ptr<Navigation> navigation);

protected:
	shared_ptr<Camera> _camera;
	shared_ptr<Navigation> _navigation;

};

inline shared_ptr<Navigation> Renderer::getNavigation()
{
	return _navigation;
}
inline void Renderer::setNavigation(shared_ptr<Navigation> navigation)
{
	_navigation=navigation;
}
}
#endif /* RENDERER_H_ */
