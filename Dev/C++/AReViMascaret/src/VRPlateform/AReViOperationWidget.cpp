// AReViOperationWidget.cpp --- 
// 
// Filename: AReViOperationWidget.cpp
// Description: 
// Author: 
// Maintainer: 
// Created: lun. nov. 21 23:06:36 2011 (+0100)
// Version: 
// Last-Updated: 
//           By: 
//     Update #: 0
// URL: 
// Keywords: 
// Compatibility: 
// 
// 

// Commentary: 
// 
// 
// 
// 

// Change Log:
// 
// 
// 
// 

// Code:

#include "VRPlateform/AReViOperationWidget.h"

#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Kernel/InstanceValue.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/PrimitiveType.h"

#include "ArWidget/widgetInteractor.h"
#include "ArWidget/awArray.h"
#include "ArWidget/awLabel.h"
#include "ArWidget/awRadioGroup.h"
#include "ArWidget/awEntry.h"
#include "ArWidget/awMenuButton.h"

namespace AReViMascaret
{
  AR_CLASS_NOVOID_DEF(AReViOperationWidget,ArObject)

  AReViOperationWidget::AReViOperationWidget(ArCW& arCW, ArRef<Viewer3D> viewer) :
  ArObject(arCW), _viewer(viewer)
    {
      //_interactor = SimpleInteractor::NEW();
      _widgetInteractor = WidgetInteractor::NEW();
      //_interactor->setRenderer(_viewer);
      //_interactor->setObjectInteraction(true);
      _widgetInteractor->setRenderer(_viewer);

      _parser = XmlParser::NEW();
      _parser->parseFile("operationWidget.xml");

      _initErrorDial();
    }

  AReViOperationWidget::~AReViOperationWidget() {}

  void AReViOperationWidget::show()
    {
      _class = _entity->getClassifier();
      
      if(_widgets.empty())
        _parse();

      _getOperations();
      if(_operations.size())
	{
	  _operationsWindow->setPosition(_mouse->getAxis("x")->getValue(),
					 _mouse->getAxis("y")->getValue());
	  _operationsWindow->setVisible(true);
	  _focus(true);
	}
    }
  
  void AReViOperationWidget::_initErrorDial()
  {
    _errorArray = AwArray::NEW();
    _errorArray->setTransient(false);
    _errorArray->setVisible(false);
    _errorArray->setRenderer3D(_viewer);

    _errorCloseButton = AwButton::NEW();
    _errorCloseButton->setTransient(false);
    _errorCloseButton->addClickCB(thisRef(),&AReViOperationWidget::_onErrorCloseButtonClick);
    _errorCloseButton->setRenderer3D(_viewer);

    _errorLabel = AwLabel::NEW();
    _errorLabel->setTransient(false);
    _errorLabel->setRenderer3D(_viewer);

    _errorArray->addWidget(_errorLabel);
    _errorArray->addWidget(_errorCloseButton);
  }
  
  void AReViOperationWidget::_setTextErrorDial(string text)
  {
    _errorLabel->setText(text);
  }

  void AReViOperationWidget::_showErrorDial(bool show)
  {
    _errorArray->setVisible(show);
  }
  
  void AReViOperationWidget::_parse()
  {
    /** parse & create widgets */
    if(!ArWidgetUtils::readWidgets(_parser->getRoot(),_widgets,_viewer))
      {
	cerr << "OperationWidget -- Error reading widgets description" << endl;
	exit(1);
      }

    /** windows */
    ArWidgetUtils::findWidget("operationsWindow",AwWidget::nullRef(),_operationsWindow);
    ArWidgetUtils::findWidget("parametersWindow",AwWidget::nullRef(),_parametersWindow);

    /** arrays */
    ArWidgetUtils::findWidget("operationsArray",AwWidget::nullRef(),_operationsArray); 
    ArWidgetUtils::findWidget("parametersArray",AwWidget::nullRef(),_parametersArray);

    /** class name */
    ArWidgetUtils::findWidget("classLabelOp",AwWidget::nullRef(),_classLabelOp);
    ArWidgetUtils::findWidget("classLabelPara",AwWidget::nullRef(),_classLabelPara);
    
    /** execute */
    if (ArWidgetUtils::findWidget("executeButton",AwWidget::nullRef(),_executeButton))
      _executeButton->addClickCB(thisRef(),&AReViOperationWidget::_onExecuteButtonClick);

    /** cancel */
    if (ArWidgetUtils::findWidget("cancelOpButton",AwWidget::nullRef(),_cancelOpButton))
      _cancelOpButton->addClickCB(thisRef(),&AReViOperationWidget::_onCancelOpButtonClick);
    if (ArWidgetUtils::findWidget("cancelParaButton",AwWidget::nullRef(),_cancelParaButton))
      _cancelParaButton->addClickCB(thisRef(),&AReViOperationWidget::_onCancelParaButtonClick);
  }

  void AReViOperationWidget::_clear()
  {
    /** delete operations/parameters widgets */
    while(_operationsArray->getNbVisibleWidgets() > 0)
      {
	_operationsArray->removeWidget(_operationsArray->accessWidget(0));	
      }
    while(_parametersArray->getNbVisibleWidgets() > 0)
      {
	_parametersArray->removeWidget(_parametersArray->accessWidget(0));	
      }
    _primitiveParameters.clear();
    _entitiesParameters.clear();
  }

  /** Events */
  // TODO : refaire plus propre, c'est tres sale ...
  void AReViOperationWidget::_onExecuteButtonClick(const AwButton::ClickEvent &evt)
  {
    bool execute = true;
    vector<shared_ptr<ValueSpecification> > values;
    map<string, ArRef<AwEntry> >::const_iterator prim;
    map<ArRef<AwRadioGroup>, vector<shared_ptr<Entity> > >::const_iterator enti;
   
    /** add primitive parameters */
    for(prim = _primitiveParameters.begin(); prim != _primitiveParameters.end() && execute; ++prim)
      {
	if((*prim).second->getText() == "")
	  {
	    _setTextErrorDial("Please fill in all fields.");
	    _showErrorDial(true);
	    execute = false;
	  }
	else
	  {
	    values.push_back(_findParameter((*prim).first)->getType()->createValueFromString((*prim).second->getText()));
	  }
      }
    /** add entity parameters */
    for(enti = _entitiesParameters.begin(); enti != _entitiesParameters.end() && execute; enti++)
      {
	shared_ptr<Entity> entity = _findEntityInGroup((*enti).first, (*enti).second);
	shared_ptr<InstanceValue> val = make_shared<InstanceValue>(entity);
        values.push_back(val);
      }
    
    if(execute)
      {
        _focus(false);
	_parametersWindow->setVisible(false);
	_clear();
	_entity->executeOperation(_currentOperation,values);
      }
    _mouse->setMouseCapture(false);

  }

  void AReViOperationWidget::_onCancelOpButtonClick(const AwButton::ClickEvent &evt)
  {
    _focus(false);
    _operationsWindow->setVisible(false);
    _clear();
    //_mouse->setMouseCapture(false);
  }

  void AReViOperationWidget::_onCancelParaButtonClick(const AwButton::ClickEvent &evt)
  {
    _focus(false);
    _parametersWindow->setVisible(false);
    _clear();
  }

  void AReViOperationWidget::_onErrorCloseButtonClick(const AwButton::ClickEvent &evt)
  {
    _showErrorDial(false);
  }

  void AReViOperationWidget::_onButtonParameterClick(const AwButton::ClickEvent &evt)
  {
    ArRef<AwRadioButton> radio = ar_down_cast<AwRadioButton>(evt.button);
    string id = radio->getID();
    if(!id.empty())
      {
        ArRef<AwLabel> label;
        if(ArWidgetUtils::findWidget(id.substr(0, id.find("_")),AwWidget::nullRef(),label))
          label->setText(radio->accessLabel()->getText());
      }
  }
  /** call operation when button was clicked */
  void AReViOperationWidget::_onOperationButtonClick(const AwButton::ClickEvent &evt)
  {
    _focus(true);
    _currentOperation = evt.button->accessLabel()->getText();
    _getParameters(_operations.find(_currentOperation)->second);
    if(_parameters.empty())
      {
	std::vector<shared_ptr<ValueSpecification> > p;
	_entity->executeOperation(_currentOperation,p);
      }
    else
      {
	_operationsWindow->setVisible(false);
	_parametersWindow->setPosition(_mouse->getAxis("x")->getValue(),
				       _mouse->getAxis("y")->getValue());
	_parametersWindow->setVisible(true);
      }
  }

  void AReViOperationWidget::_focus(bool focus)
  {
    //_mouse->setMouseCapture(!focus);
    if(focus)
      _mouse->setRenderer(Renderer3D::nullRef());
    else
      _mouse->setRenderer(_viewer);
  }

  void AReViOperationWidget::_getOperations()
  {
    _classLabelOp->setText(_class->getName());
    _operations = _class->getOperations();
    map<string, shared_ptr<Operation> >::const_iterator it;
    
    for(it = _operations.begin(); it != _operations.end(); it++)
      {
	ArRef<AwButton> op = AwButton::NEW();
	ArRef<AwLabel> label = AwLabel::NEW();
        label->setText((*it).first);
	op->setLabel(label);
        op->setRenderer3D(_viewer);
	op->setTransient(false);
	op->addClickCB(thisRef(),&AReViOperationWidget::_onOperationButtonClick);
        _operationsArray->addWidget(op);        
      }
  }

  void AReViOperationWidget::_getParameters(shared_ptr<Operation> operation)
  {
    _classLabelPara->setText(_class->getName());
    _parameters = operation->getParameter();
    vector<shared_ptr<Parameter> >::const_iterator it;

    for(it = _parameters.begin(); it != _parameters.end(); it++)
      {
	string type = (*it)->getType()->getName();
	string param = (*it)->getName();

	/** it's a primitive type */	
	if(type == "real" ||
	   type == "double" ||
	   type == "integer" ||
	   type == "int" ||
	   type == "boolean" ||
	   type == "bool" ||
	   type == "string"
	   )
	  {
	    ArRef<AwLabel> label = AwLabel::NEW();
	    label->setText(param + " : " +  type);
	    label->setTransient(false);
	    label->setRenderer3D(_viewer);
	    
	    ArRef<AwEntry> entry = AwEntry::NEW();
	    entry->setMultiline(false);
	    entry->setTransient(false);
	    entry->setLimitSize(15);
	    entry->setRequestSize(150, 30);
	    
	    if(type == "real" ||
	       type == "double" ||
	       type == "integer" ||
	       type == "int")
	      {
		entry->setNumericMode(true);
	      }
	    else
	      {
		entry->setNumericMode(false);
	      }
	    _primitiveParameters.insert(pair<string, ArRef<AwEntry> >(param, entry));
	    entry->setRenderer3D(_viewer);
	    _parametersArray->addWidget(label);
	    _parametersArray->addWidget(entry);
	  }

	/** it's an entity */
	else
	  {
	    ArRef<AwArray> container = AwArray::NEW();
	    ArRef<AwMenuButton> menu = AwMenuButton::NEW();
	    ArRef<AwRadioGroup> group = AwRadioGroup::NEW();
	    ArRef<AwLabel> showmore = AwLabel::NEW();
	    ArRef<AwLabel> labelType = AwLabel::NEW();
	    ArRef<AwLabel> labelParam = AwLabel::NEW();
	   	    
	    container->setRenderer3D(_viewer);
	    container->setTransient(false);

	    showmore->setRenderer3D(_viewer);
	    showmore->setTransient(false);	    
	    
	    labelType->setRenderer3D(_viewer);
	    labelType->setTransient(false);	    
	    labelType->setText(": " + type);
	    
	    labelParam->setRenderer3D(_viewer);
	    labelParam->setTransient(false);	    
	    labelParam->setText("?");

	    menu->setLabel(showmore);
	    menu->setRenderer3D(_viewer);
	    menu->setTransient(false);

            group->setMode(AwArray::ARRAY_VERT);
	    group->setRenderer3D(_viewer);
	    group->setTransient(false);	    
	    _findEntities(type, group, labelParam);

	    menu->setToolbar(group);
	    container->addWidget(labelParam);
	    container->addWidget(labelType);
	    container->addWidget(menu);
	    
	    _parametersArray->addWidget(container);
	  }
      } 
  }

  shared_ptr<Parameter> AReViOperationWidget::_findParameter(string parameterName)
  {
    vector<shared_ptr<Parameter> >::const_iterator it;
    for(it = _parameters.begin(); it != _parameters.end(); it++)
      {
	if(parameterName == (*it)->getName())
	  return (*it);
      }
  }

  void AReViOperationWidget::_findEntities(string type, ArRef<AwRadioGroup> group, ArRef<AwLabel> labelParam)
  {
    vector<shared_ptr<Entity> > entities = _environment->getEntities(); 
    vector<shared_ptr<Entity> > params;
    vector<shared_ptr<Entity> >::const_iterator e;
    for(e = entities.begin(); e != entities.end(); e++)
      {
	if((*e)->getClassifier()->getName() == type)
	  {
	    /** we found an entity which had the same type */
	    ArRef<AwLabel> label = AwLabel::NEW();
	    ArRef<AwRadioButton> entity = AwRadioButton::NEW();
	    
	    label->setText((*e)->getName());
	    label->setTransient(false);
	    label->setRenderer3D(_viewer);

	    entity->setLabel(label);
	    entity->setTransient(false);
	    entity->setRenderer3D(_viewer);
            labelParam->setID(type + group->getID());
            entity->setID(type + group->getID() + "_" + (*e)->getName());
            entity->addClickCB(thisRef(),&AReViOperationWidget::_onButtonParameterClick);

	    group->addWidget(entity);
            
            ArRef<AwRadioButton> radio = ar_down_cast<AwRadioButton>(group->accessWidget(group->getSelectedButton()));
            labelParam->setText(radio->accessLabel()->getText());
	    params.push_back(*e);
	  }
      }
    pair<map<ArRef<AwRadioGroup>,vector<shared_ptr<Entity> > >::iterator,bool> ret;
    _entitiesParameters.insert(make_pair(group, params));
  }
  
  shared_ptr<Entity> AReViOperationWidget::_findEntityInGroup(ArRef<AwRadioGroup> group, vector<shared_ptr<Entity> > entities)
  {
    ArRef<AwRadioButton> radio = ar_down_cast<AwRadioButton>(group->accessWidget(group->getSelectedButton()));
    vector<shared_ptr<Entity> >::const_iterator it;
    for(it = entities.begin(); it != entities.end(); it++)
      {
	if(radio->accessLabel()->getText() == (*it)->getName())
	  {
	    return (*it);
	  }
      }
  }
  
}

// 
// AReViOperationWidget.cpp ends here
