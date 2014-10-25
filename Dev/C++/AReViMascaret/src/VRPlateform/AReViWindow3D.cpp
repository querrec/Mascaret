/*
 * AReViWindow3D.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */

#include "Tools/veha_plateform.h"
#include "VRPlateform/AReViWindow3D.h"
#include "VRPlateform/AReViOperationWidget.h"
#include "VRPlateform/AReViSignalWidget.h"
//#include "VRPlateform/AReViVocalPlayerWidget.h"
#include "VRPlateform/AReViShapeSpecification.h"
#include <boost/enable_shared_from_this.hpp>
#include "Assistance/AReViOSDDisplay.h"
#include "Assistance/AReViBalloon.h"
#include "Assistance/AReViImage.h"
#include "AReVi/Lib3D/boundingBox3D.h"

#include "Interaction/AReViCursor.h"

#include <boost/shared_ptr.hpp>

namespace AReViMascaret
{
  AReViWindow3D::AReViWindow3D() :
		_mouseCapture(false)
  {
    _areviViewer = Viewer3D::NEW();
    _areviViewer->setCloseAction(Renderer3D::CLOSE_LEAVE);
    _areviViewer->setMapped(true);
    _operationWidget = AReViOperationWidget::NEW(_areviViewer);
    _signalWidget = AReViSignalWidget::NEW(this);
   // _vocalPlayerWidget = AReViVocalPlayerWidget::NEW(this);
 
    _keyboard=shared_ptr<AReViKeyboard>(new AReViKeyboard());
    _mouse=shared_ptr<AReViMouse>(new AReViMouse());
    addPeripheric(_mouse);
    addPeripheric(_keyboard);
    setMouse(_mouse);
    setKeyboard(_keyboard);

    shared_ptr<AReViCursor> cursor(new AReViCursor());
    cursor->setRenderer(_areviViewer);
    addCursor(cursor);
  }

  AReViWindow3D::~AReViWindow3D()
  {}

  void AReViWindow3D::getPosition(unsigned int& xOut, unsigned int& yOut) const
  {
    _areviViewer->getWindowPosition((int&)xOut,(int&)yOut);
  }
  void AReViWindow3D::setPosition(unsigned int x, unsigned int y)
  {
    _areviViewer->setWindowPosition((int)x,(int)y);
  }
  
  void AReViWindow3D::getSize(unsigned int& xOut, unsigned int& yOut) const
  {
    _areviViewer->getWindowSize(xOut,yOut);
  }
  void AReViWindow3D::setSize(unsigned int x, unsigned int y)
  {
    _areviViewer->setWindowSize(x,y);
  }
  bool AReViWindow3D::getDecoration() const
  {
    return _areviViewer->getDecoration();
  }
  void AReViWindow3D::setDecoration(bool deco)
  {
    _areviViewer->setDecoration(deco);
  }

  void AReViWindow3D::setMouse(shared_ptr<AReViMouse> mouse)
  {
    _mouse = mouse;
    _mouse->setRenderer(_areviViewer);
    _mouse->setMouseCapture(false);
    //_mouse->setMouseCapture(_mouseCapture);
  }

  void AReViWindow3D::setKeyboard(shared_ptr<AReViKeyboard> keyboard)
  {
    _keyboard = keyboard;
    _keyboard->setRenderer(_areviViewer);
  }

  bool AReViWindow3D::getMouseCapture() const
  {
    return _mouse->getMouseCapture();
  }
  void AReViWindow3D::setMouseCapture(bool captureMouse)
  {
    _mouseCapture = captureMouse;
     _mouse->setMouseCapture(captureMouse);
  }

  void AReViWindow3D::enableSimpleInteractor(bool simpleInteractor)
  {
      cerr << "SimpleInteractor not implemented in AReViMascaret" << endl;
  }
  
  void AReViWindow3D::showOperations(shared_ptr<VEHA::Entity> entity,
				     shared_ptr<VEHA::Environment> environment) const
  {
    _operationWidget->setMouse(_mouse);
    _operationWidget->setEntity(entity);
    _operationWidget->setEnvironment(environment);
    _operationWidget->show();
  }

  void AReViWindow3D::showSignals(shared_ptr<VEHA::Entity> entity,
				  shared_ptr<VEHA::Environment> environment) const
  {
cerr << " AReViWindow3D::showSignals" << endl;
    _signalWidget->setMouse(_mouse);
    _signalWidget->setEntity(entity);
    _signalWidget->setEnvironment(environment);
    _signalWidget->show();
  }


  void AReViWindow3D::showText(string text, float ratio)
  {
	_osdDisplay = AReViOSDDisplay::NEW();
	_osdDisplay->displayText(text,ratio,OSD::LOC_BOTTOM_CENTER);
  }

  void AReViWindow3D::showTextWide(string text, float ratio)
  {
	_osdDisplay = AReViOSDDisplay::NEW();
	_osdDisplay->displayText(text,ratio,OSD::LOC_TOP_CENTER);
  }

  void AReViWindow3D::showBalloon(string text,shared_ptr<Entity> e)
  {
	Vector3 v=e->getGlobalPosition();
	shared_ptr<AReViShapeSpecification> shapeSpec=shared_dynamic_cast<AReViShapeSpecification>(e->getActiveShape());
	ArRef<Object3D> shape=shapeSpec->getObject3D();
	ArRef<BoundingBox3D> bb=BoundingBox3D::NEW();
	shape->computeBoundingBox(bb,Base3D::nullRef());
	double x,y,z;
	bb->getSize(x,y,z);
	
	_balloon = AReViBalloon::NEW(text,x*2,y*2,v.x,v.y+y,v.z+z,0.0);
	_balloon->displayText(text);
  }

  void AReViWindow3D::showImage(string url)
  {
	cout<<"Print image"<<endl;
	unsigned int y,z;
 	getSize(y,z);
	_image = AReViImage::NEW(url,1,OSD::LOC_BOTTOM_LEFT);
	_image->show();

  }

  void AReViWindow3D::hide()
  {
	if(_osdDisplay!=NULL)
		_osdDisplay->hide();
	if(_balloon!=NULL)
		_balloon->hide();
	if(_image!=NULL)
		_image->hide();
  }

}
