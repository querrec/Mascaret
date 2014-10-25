/* AReViOperationWidget.h --- 
 * 
 * Filename: AReViOperationWidget.h
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

#ifndef AREVIOPERATIONWIDGET_H_
#define AREVIOPERATIONWIDGET_H_ 

#include "AReVi/arObject.h"
#if 0
#include "ArWidget/aw_plateform.h"
#endif

#include "areviMascaret_plateform.h"
#include "AReVi/Lib3D/simpleInteractor.h"
#include "AReVi/Lib3D/viewer3D.h"
#include "Interaction/AReViMouse.h"

#include "ArWidget/utils.h"
#include "ArWidget/awButton.h"

#include "VEHA/Entity/Entity.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"

//namespace AReVi
//{
    class AwRadioGroup;
    class AwEntry;
//}

//using AReVi::AwRadioGroup;
//using AReVi::AwEntry;

using namespace VEHA;

namespace AReViMascaret
{

  class AREVI_MASCARET_API AReViOperationWidget : public ArObject
  {
  public:
    AR_CLASS(AReViOperationWidget);
    AR_CONSTRUCTOR_1(AReViOperationWidget,ArRef<Viewer3D>,_viewer);

    inline void setEntity(shared_ptr<Entity> entity) { _entity = entity; }
    inline shared_ptr<Entity> getEntity() { return _entity; }
    inline ArRef<Viewer3D> getViewer() { return _viewer; }
    // inline ArRef<AReViMouse> getMouse() { return _mouse; }
    inline void setMouse(shared_ptr<AReViMouse> mouse) { _mouse = mouse; }
    inline void setEnvironment(shared_ptr<Environment> environment) { _environment = environment; }
    void show();

  protected:
    void _getOperations();
    void _getParameters(shared_ptr<Operation> operation);
    shared_ptr<Parameter> _findParameter(string parameterName);
    void _findEntities(string type, ArRef<AwRadioGroup> group, ArRef<AwLabel> labelParam);
    shared_ptr<Entity> _findEntityInGroup(ArRef<AwRadioGroup> group, vector<shared_ptr<Entity> > entities);
    void _focus(bool focus);
    void _parse();
    void _clear();

    void _initErrorDial();
    void _setTextErrorDial(string text);
    void _showErrorDial(bool show);

    void _onExecuteButtonClick(const AwButton::ClickEvent &evt);
    void _onCancelOpButtonClick(const AwButton::ClickEvent &evt);
    void _onCancelParaButtonClick(const AwButton::ClickEvent &evt);
    void _onOperationButtonClick(const AwButton::ClickEvent &evt);
    void _onErrorCloseButtonClick(const AwButton::ClickEvent &evt);
    void _onButtonParameterClick(const AwButton::ClickEvent &evt);
    void _onButtonGroupClick(const AwButton::ClickEvent &evt);

  protected:
    ArRef<Viewer3D> _viewer;
    shared_ptr<AReViMouse> _mouse;
    ArRef<SimpleInteractor> _interactor;
    ArRef<WidgetInteractor> _widgetInteractor;
    shared_ptr<Entity> _entity;
    shared_ptr<Environment> _environment;

    ArRef<AReVi::XmlParser> _parser;
    ArRef<AwButton> _executeButton;
    ArRef<AwButton> _cancelOpButton;
    ArRef<AwButton> _cancelParaButton;
    ArRef<AwWidget> _operationsWindow;
    ArRef<AwWidget> _parametersWindow;
    ArRef<AwArray> _operationsArray;
    ArRef<AwArray> _parametersArray;

    ArRef<AwArray> _errorArray;
    ArRef<AwButton> _errorCloseButton;
    ArRef<AwLabel> _errorLabel;

    ArRef<AwLabel> _classLabelOp;
    ArRef<AwLabel> _classLabelPara;
    map<string, ArRef<AwEntry> > _primitiveParameters;
    std::multimap<ArRef<AwRadioGroup>, vector<shared_ptr<Entity> > > _entitiesParameters;

    StlMap<StlString, ArRef<AwWidget> > _widgets;
    
    shared_ptr<Class> _class;
    map<string, shared_ptr<Operation> > _operations;
    vector<shared_ptr<Parameter> > _parameters;
    shared_ptr<BehaviorExecution> _behaviorExecution;

    string _currentOperation;
  };
}

#endif /* AReViOperationWidget.h ends here */
