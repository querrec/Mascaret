/*
 * AReViWindow3D.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef AREVIWINDOW3D_H_
#define AREVIWINDOW3D_H_

#include "areviMascaret_plateform.h"
#include "IEHA/Visualisation/Window3D.h"
#include "AReVi/Lib3D/viewer3D.h"
#include "Interaction/AReViMouse.h"
#include "Interaction/AReViKeyboard.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/Entity.h"

//using namespace AReVi;

namespace AReViMascaret
{
    using AReVi::ArRef;
    using AReVi::Viewer3D;
  class AReViOSDDisplay;
  class AReViBalloon;
  class AReViOperationWidget;
  class AReViSignalWidget;
 // class AReViVocalPlayerWidget;
  class AReViImage;
  using VEHA::Entity;
  using VEHA::Environment;

  class AREVI_MASCARET_API AReViWindow3D : public IEHA::Window3D
    {
    public:
      AReViWindow3D();
      virtual ~AReViWindow3D();
      
      virtual void getPosition(unsigned int& xOut, unsigned int& yOut) const;
      virtual void setPosition(unsigned int x, unsigned int y);
      
      virtual void getSize(unsigned int& xOut, unsigned int& yOut) const;
      virtual void setSize(unsigned int x, unsigned int y);
      
      virtual bool getDecoration() const;
      virtual void setDecoration(bool deco);
      
      virtual bool getMouseCapture() const;
      virtual void setMouseCapture(bool captureMouse);
      
      virtual void enableSimpleInteractor(bool simpleInteractor);
      
      void showOperations(shared_ptr<Entity> entity,
			  shared_ptr<Environment> environment) const;
      
      void showSignals(shared_ptr<Entity> entity,
		       shared_ptr<Environment> environment) const;

      inline ArRef<Viewer3D> getAReViViewer() const;
      inline void setAReViViewer(ArRef<Viewer3D> areviViewer);

      void setKeyboard(shared_ptr<AReViKeyboard> keyboard);
      void setMouse(shared_ptr<AReViMouse> mouse);
      
      virtual void hide();
      virtual void showText(string text, float ratio);
      virtual void showTextWide(string text, float ratio);
      virtual void showBalloon(string text,shared_ptr<Entity> e);
      virtual void showImage(string url);


    protected:
      ArRef<Viewer3D> _areviViewer;
      ArRef<AReViOperationWidget> _operationWidget;
      ArRef<AReViSignalWidget> _signalWidget;
     // ArRef<AReViVocalPlayerWidget> _vocalPlayerWidget;
      shared_ptr<AReViMouse> _mouse;
      bool _mouseCapture;
      shared_ptr<AReViKeyboard> _keyboard;
      ArRef<AReViOSDDisplay> _osdDisplay;
      ArRef<AReViBalloon> _balloon;
      ArRef<AReViImage> _image;

	//FRED
	/*shared_ptr<ProcedureExecution> _procExec;
	int _totalActions;
	int _totalCorrectActions;
	long int _totalTime;
	long int _initialTime;
	shared_ptr<AID>  _aid;*/
	//END FRED
    };
  
  ArRef<Viewer3D> AReViWindow3D::getAReViViewer() const
    {
      return _areviViewer;
    }
  
  void AReViWindow3D::setAReViViewer(ArRef<Viewer3D> areviViewer)
  {
    this->_areviViewer = areviViewer;
  }
  
}
#endif /* AREVIWINDOW3D_H_ */
