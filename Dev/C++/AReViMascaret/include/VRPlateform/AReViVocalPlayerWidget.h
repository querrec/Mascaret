/* AReViVocalPlayerWidget.h --- 
 * 
 * Filename: AReViVocalPlayerWidget.h
 * Description: Create a widget which list operations of the current clicked entity
 * Author: Julien Rolland
 * Maintainer: 
 * Created: lun. nov. 21 22:23:46 2011 (+0100)
 * Version: 
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change Log:
 * 
 * 
 */

/* Code: */

#ifndef AREVIVocalPlayerWidget_H_
#define AREVIVocalPlayerWidget_H_ 

#include "AReVi/arObject.h"
#include "ArWidget/aw_plateform.h"

#include "areviMascaret_plateform.h"
#include "AReVi/Lib3D/simpleInteractor.h"
#include "AReVi/Lib3D/viewer3D.h"
#include "Interaction/AReViMouse.h"

#include "ArWidget/awWidget.h"
#include "ArWidget/awButton.h"
#include "ArWidget/awArray.h"
#include "ArWidget/awLabel.h"
#include "ArWidget/widgetInteractor.h"
#include "ArWidget/awMenuButton.h"
#include "ArWidget/awRadioGroup.h"
#include "ArWidget/awToolbar.h"
#include "ArWidget/awEntry.h"
#include "ArWidget/utils.h"
#include "ArWidget/awSlider.h"

#include "VEHA/Entity/Entity.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VRPlateform/AReViWindow3D.h"



using namespace AReVi;
using namespace VEHA;
using namespace IEHA;

namespace AReViMascaret
{

  class AREVI_MASCARET_API AReViVocalPlayerWidget : public ArObject
  {
  public:
    AR_CLASS(AReViVocalPlayerWidget);
    AR_CONSTRUCTOR_1(AReViVocalPlayerWidget,AReViWindow3D*,_viewer);

    void initializeMap();

  protected:

    void _onExecuteButtonClick(const AwButton::ClickEvent &evt);

  protected:
    ArRef<Viewer3D> _viewer;
    AReViWindow3D* _window3D;
    shared_ptr<AReViMouse> _mouse;
    ArRef<SimpleInteractor> _interactor;
    ArRef<WidgetInteractor> _widgetInteractor;

    ArRef<XmlParser> _parser;
    ArRef<AwButton> _executeButton;


    StlMap<StlString, ArRef<AwWidget> > _widgets;
    map<string, string> _soundMap;
    map<string, string> _iconMap;
    map<string, int> _accessAssistancesMap;
  };
}

#endif /* AReViVocalPlayerWidget.h ends here */
