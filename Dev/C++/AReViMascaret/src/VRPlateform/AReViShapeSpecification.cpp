/*
 * AReViShapeSpecification.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */
#include "AReVi/VRML/vrmlShape3D.h"
#include "AReVi/Lib3D/transform3D.h"
#include "AReVi/Lib3D/scene3D.h"
#include "AReVi/Lib3D/urlSound3D.h"
#include "AReVi/Shapes/box3D.h"
#include "AReVi/activity.h"
#include "AReVi/arObject.h"

#include "Assistance/AReViColor.h"
#include "VRPlateform/AReViShapeSpecification.h"
#include "VRPlateform/AReViPointSpecification.h"
#include "hLib/hLib.h"
#include "VRPlateform/AReViScene.h"
#include "IEHA/VRApplication.h"


#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

using namespace AReVi;
using IEHA::VRApplication;

namespace AReViMascaret
{
AReViShapeSpecification::AReViShapeSpecification(const string& name, const string & url, bool movable,bool recursive,const string& shader)
: ShapeSpecification(name,url,movable,recursive,shader), _obj(AReViEntityObject::NEW())
{
	boost::filesystem::path path1(url);
	string extension=boost::filesystem::extension(path1);
	boost::to_lower(extension);
	if (extension == ".wrl")
	{
		ArRef<VrmlShape3D> sh=VrmlShape3D::NEW();
		if(!sh->parseURL(url))
		{
			//cerr << "error in file : " <<url << endl<< sh->readParserMessages()<<endl;
		}
		_obj->setShape(sh);
		if(name == "skybox")
		{
			cerr <<"skybox: "<< sh<<endl;
		}
	}
	else if (extension == ".md5mesh")
	{
		ArRef<BodyShape> sh = HLibLoader::loadBodyShape(url);
		if(sh)
		{
			sh->setShowSkeleton(false);
			_obj->setShape(sh);
		}
		else
			cerr << "FAILED load body : " <<url<< endl;

	}
	else if (extension == ".smd")
	{
		ArRef<BodyShape> sh = HLibLoader::loadBodyShape(url);
		if(sh)
		{
			sh->setShowSkeleton(false);
			_obj->setShape(sh);
		}
		else
			cerr << "FAILED load body : " <<url<< endl;
	}
	else
	{
		cerr << "FAILED load body " << url << " : Unknown format" << endl;
	}
}
AReViShapeSpecification::AReViShapeSpecification(const string& name, const vector<double>& distances,const vector<string> & urls, bool movable,bool recursive,const string& shader)
: ShapeSpecification(name,distances,urls,movable,recursive,shader), _obj(AReViEntityObject::NEW())
{
	vector<ArRef<Shape3D> > shapes;

	for(size_t i=0;i<urls.size();i++)
	{
		string fileName=urls[i];

		boost::filesystem::path path1(fileName);
		string extension=boost::filesystem::extension(path1);
		boost::to_lower(extension);
		if (extension == ".wrl")
		{
			ArRef<VrmlShape3D> sh=VrmlShape3D::NEW();
			if(!sh->parseURL(fileName))
			{
				cerr << "Error in file : " <<fileName << sh->readParserMessages() << endl;
			}
			shapes.push_back(sh);
		}
		else if (extension == ".md5mesh")
		{
			ArRef<BodyShape> sh = HLibLoader::loadBodyShape(fileName);
			if(sh)
			{
				sh->setShowSkeleton(false);
				shapes.push_back(sh);
			}
			else
				cerr << "FAILED load body : " <<fileName<< endl;
		}
		else if (extension == ".smd")
		{
			ArRef<BodyShape> sh = HLibLoader::loadBodyShape(fileName);
			if(sh)
			{
				sh->setShowSkeleton(false);
				shapes.push_back(sh);
			}
			else
				cerr << "FAILED load body : " <<fileName<< endl;
		}
		else
		{
			cerr << "FAILED load body " << fileName << " : Unknown format" << endl;
		}
	}
	_obj->setLODShapes(distances,shapes);
}

AReViShapeSpecification::~AReViShapeSpecification()
{

}

void AReViShapeSpecification::setVisibility(bool visibility)
{
	// TODO: Le nom de la scene est en dur...
	if(visibility)
		shared_dynamic_cast<AReViScene>(VRApplication::getInstance()->getScene("scene"))->getScene()->addObject(_obj);
	else
		shared_dynamic_cast<AReViScene>(VRApplication::getInstance()->getScene("scene"))->getScene()->removeObject(_obj);
}
bool AReViShapeSpecification::getVisibility() const
{
	// TODO: A FAIRE...
	return true;
}

void AReViShapeSpecification::setScale(const Vector3& )
{

}
Vector3 AReViShapeSpecification::getScale() const
{
	return Vector3();
}
void AReViShapeSpecification::setEntity(shared_ptr<Entity> entity)
{
	_obj->setEntity(entity);
	_obj->setLocation(shared_dynamic_cast<AReViPointSpecification>(entity->getReferentialPoint())->_point);
	_obj->attachTo(shared_dynamic_cast<AReViPointSpecification>(entity->getReferentialPoint())->_point);
	shared_dynamic_cast<AReViPointSpecification>(entity->getReferentialPoint())->_point->attachTo(_obj);
}
shared_ptr<Entity> AReViShapeSpecification::getEntity() const
{
	return _obj->getEntity();
}


shared_ptr<Color> AReViShapeSpecification::getColor()
{
	cout<<"I'm in the middle of a Get color function!AMAZING"<<endl;
	shared_ptr<AReViColor> color(new AReViColor());

	ArRef<Object3D> shape = this->getObject3D();
	StlVector<ArRef<Shape3D> > shapes;
	StlVector<double> tmp;
	shape->queryShapes(tmp, shapes);

	for(unsigned int itShapes=0; itShapes<shapes.size(); itShapes++)
	{	
		for(unsigned int itSP=0; itSP<shapes[itShapes]->getNbParts(); itSP++)
		{	
			if (shapes[itShapes]->getPart(itSP) &&  shapes[itShapes]->getPart(itSP)->getClass()->isA(Surface3D::CLASS()))
			{	
				if(_surfaceMaterialMap.find(shapes[itShapes]->accessPart(itSP)->getAlias()) == _surfaceMaterialMap.end())
				{
					ArRef<Material3D> mat2 = Material3D::NEW();
					ar_down_cast<Surface3D>(shapes[itShapes]->accessPart(itSP))->readMaterial(mat2);
					_surfaceMaterialMap[shapes[itShapes]->accessPart(itSP)->getAlias()] = mat2;
				}
			}
		}
	}

	color->setSurfaceMap(_surfaceMaterialMap);
	return color;	
}

void AReViShapeSpecification::setColor(shared_ptr<Color> color)
{
	ArRef<Object3D> shape = this->getObject3D();
	ArRef<Material3D> mat;

	shared_ptr<AReViColor> aRcolor;
	aRcolor= shared_dynamic_cast<AReViColor>(color);

	mat = Material3D::NEW();
	mat->setTransparency(0.0);
	mat->setAmbientIntensity(1.0);
	mat->setDiffuseColor(color->r,color->g,color->b);
	StlVector<ArRef<Shape3D> > shapes;
	StlVector<double> tmp;
	shape->queryShapes(tmp, shapes);
	for(unsigned int itShapes=0; itShapes<shapes.size(); itShapes++)
	{
		for(unsigned int itSP=0; itSP<shapes[itShapes]->getNbParts(); itSP++)
		{
			if (shapes[itShapes]->getPart(itSP) &&  shapes[itShapes]->getPart(itSP)->getClass()->isA(Surface3D::CLASS()))
			{
				if(mat==NULL)
					mat = aRcolor->getSurfaceMap()[shapes[itShapes]->accessPart(itSP)->getAlias()];
					(ar_down_cast<Surface3D>(shapes[itShapes]->accessPart(itSP)))->writeMaterial(mat);
			}
		}
	}
	
}

void  AReViShapeSpecification::setTransparent(shared_ptr<Color> color)
{
	StlMap<StlString,ArRef<Material3D> > _surfaceMaterialMap;
	//shared_ptr<AReViColor> aRcolor;	
	//aRcolor= shared_dynamic_cast<AReViColor>(color);
	
	//commented because the cast above does not work

	ArRef<Object3D> shape=this->getObject3D();
	ArRef<Material3D> mat = Material3D::NEW();
	StlVector<ArRef<Shape3D> > shapes;
	StlVector<double> tmp;
	shape->queryShapes(tmp, shapes);
	
	for(unsigned int itShapes=0; itShapes<shapes.size(); itShapes++)
	{
		for(unsigned int itSP=0; itSP<shapes[itShapes]->getNbParts(); itSP++)
		{
			if (shapes[itShapes]->getPart(itSP) &&  shapes[itShapes]->getPart(itSP)->getClass()->isA(Surface3D::CLASS()))
			{
				ar_down_cast<Surface3D>(shapes[itShapes]->accessPart(itSP))->readMaterial(mat);
				//mat->setTransparency(aRcolor->a/100);
				cerr << "setting transparency: " << color->a << endl;
				mat->setTransparency(color->a);
				(ar_down_cast<Surface3D>(shapes[itShapes]->accessPart(itSP)))->writeMaterial(mat);
			}
		}
	}

}

void AReViShapeSpecification::growUp(int percent)
{
	cout<<"I'm in the middle of a growUp function!AMAZING"<<endl;

	_object3D=this->getObject3D();

	ArRef<Transform3D> transformSave = Transform3D::NEW();
	ArRef<Transform3D> transform;

	StlVector<ArRef<Shape3D> > shapes;
	StlVector<double> tmp;
	_object3D->queryShapes(tmp, shapes);
	for(unsigned int itShapes=0; itShapes<shapes.size(); itShapes++)
	{
	  shapes[itShapes]->readTransformation(transformSave); //recupere le transform associé à la shape
	  transform = Transform3D::NEW(); //cree un autre transform
	  Util3D::Transform & tf = transform->accessTransformation(); //recupere l'objet du nouveau transform pour le modifier
	  tf = transformSave->getTransformation(); //recopie celui du Save

	  _transformMap[shapes[itShapes]->getAlias()] = transformSave; //on stocke une map de transform associés aux shapes

	  transform->postScale(percent,percent,percent);
	  shapes[itShapes]->writeTransformation(transform);
	}
}

bool AReViShapeSpecification::restaureColor(shared_ptr<Color> color)
{
	//shared_ptr<AReViColor> aRcolor;
	//aRcolor= shared_dynamic_cast<AReViColor>(color);

	StlMap<StlString, ArRef<Material3D> >::iterator itMat;

	for(itMat = _surfaceMaterialMap.begin(); itMat!=_surfaceMaterialMap.end(); itMat++)
	{
		ArRef<Surface3D> s;
		Surface3D::CLASS()->findAlias(itMat->first, s);
		if (s)
		{
			s->writeMaterial(itMat->second);
		}
		else
			cerr << itMat->first << " not found!!" << endl;
	}
	_surfaceMaterialMap.clear();
	return false;
}

bool AReViShapeSpecification::restaureShape()
{
	StlMap<StlString, ArRef<Transform3D> >::iterator itTrans;

	for(itTrans = _transformMap.begin(); itTrans!=_transformMap.end(); itTrans++)
	{
		ArRef<Shape3D> s;
		Shape3D::CLASS()->findAlias(itTrans->first, s);
		if (s)
			s->writeTransformation(itTrans->second);
		else
			cerr << itTrans->first << " not found!!" << endl;
	}
	_transformMap.clear();

	return false;
}

}


