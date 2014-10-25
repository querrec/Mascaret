#include "Animation/HLibAnimation.h"
#include "AReVi/activity.h"
#include "AReVi/VRML/vrmlShape3D.h"
#include "AReVi/Lib3D/transform3D.h"

using namespace AReVi;
using namespace hLib;
namespace AReViMascaret
{

AR_CLASS_DEF(HLibAnimation,AReViAnimation)

HLibAnimation::HLibAnimation(ArCW & arCW) :
AReViAnimation(arCW)
{
}

HLibAnimation::~HLibAnimation() {}

void HLibAnimation::applyPose(ArConstRef<Keyframe> kf)
{
	vector<double> dists;
	vector<ArRef<Shape3D> > shapes;
	_shape->getObject3D()->queryShapes(dists,shapes);
	size_t i=0;
	/*size_t size=dists.size();
	if(size)
	{
		double x1,y1,z1,x2,y2,z2;
		shared_dynamic_cast<AReViInterface3D>(Interface3D::get())->getViewer()->getPosition(x1,y1,z1);
		_object->getObject3D()->getPosition(x2,y2,z2);
		double squareDist=(x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1);
		double limit;
		while(i<size)
		{
			limit=dists[i];
			if(limit*limit < squareDist)
			{
				i++;
			}
			else
			{
				break;
			}
		}
		if(i >= size)
		{
			cerr << "ignoring pose "<< _name <<" to "<< _object << endl;
			return;
		}
	}*/
	ArRef<BodyShape> sh = ar_down_cast<BodyShape>(shapes[i]);
	if(sh == BodyShape::nullRef())
	{
		std::cerr << _shape->getUrl() << endl;
		return;
	}

	//sh->applyPose(kf);
	ArRef<Skeleton> sk = sh->accessSkeleton();
	for(unsigned int i=0; i < sk->getNbJoints(); ++i)
	{
		sk->accessJoint(i)->maskMotion();
		sk->accessJoint(i)->accessBase().setMatrix(kf->getJoint(i)->getBase().matrix);
		sk->accessJoint(i)->unmaskMotion();
	}
}
}
