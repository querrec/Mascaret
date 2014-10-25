#include "IEHA/Visualisation/Renderer.h"
#include "IEHA/Interaction/Navigation.h"

namespace IEHA
{
Renderer::Renderer()
{
}
Renderer::~Renderer()
{
	if(_navigation)
		_navigation->removeAllNavigationAxes();
}
}
