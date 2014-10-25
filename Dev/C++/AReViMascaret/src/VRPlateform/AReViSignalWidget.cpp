// AReViSignalWidget.cpp --- 
// 
// Filename: AReViSignalWidget.cpp
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

#include <set>


#include "VRPlateform/AReViSignalWidget.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Common/Event.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Kernel/InstanceValue.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/PrimitiveType.h"

#include "VEHA/Behavior/Common/behaviorScheduler.h"

#include "ArWidget/widgetInteractor.h"
#include "ArWidget/awArray.h"
#include "ArWidget/awLabel.h"
#include "ArWidget/awRadioGroup.h"
#include "ArWidget/awEntry.h"
#include "ArWidget/awMenuButton.h"

//#include "IEHA/VRApplication.h"


using namespace std;

namespace AReViMascaret
{
  AR_CLASS_NOVOID_DEF(AReViSignalWidget,ArObject)

  AReViSignalWidget::AReViSignalWidget(ArCW& arCW, AReViWindow3D* window) :
  ArObject(arCW), _viewer(window->getAReViViewer()), _window3D(window)
    {
      _widgetInteractor = WidgetInteractor::NEW();
      _parser = XmlParser::NEW();
      _parser->parseFile("signalWidget.xml");
				
      //_tutor = shared_ptr<Tutor>(new Tutor());
    }

  AReViSignalWidget::~AReViSignalWidget() {}

  void AReViSignalWidget::show()
    {
      if(_widgets.empty())
        _parse();

      _clear();
      
      _window->setPosition(_mouse->getAxis("x")->getValue(), _mouse->getAxis("y")->getValue());
      _classLabel->setText(_entity->getClassifier()->getName());

      _getSignals();

      bool visible = (_signalsArray->getNbVisibleWidgets() > 0);
      _window->setVisible(visible);
      _focus(visible);
    }
  
  
  void AReViSignalWidget::_parse()
  {
    /** parse & create widgets */
    if(!ArWidgetUtils::readWidgets(_parser->getRoot(),_widgets,_viewer))
      {
	cerr << "SignalWidget -- Error reading widgets description" << endl;
	exit(1);
      }

    /** window */
    ArWidgetUtils::findWidget("window",AwWidget::nullRef(),_window);
    ArWidgetUtils::findWidget("parametersWindow",AwWidget::nullRef(),_parametersWindow);


    /** array */
    ArWidgetUtils::findWidget("signalsArray",AwWidget::nullRef(),_signalsArray); 
    ArWidgetUtils::findWidget("parametersArray",AwWidget::nullRef(),_parametersArray);

    /** class name */
    ArWidgetUtils::findWidget("classLabel",AwWidget::nullRef(),_classLabel);
    ArWidgetUtils::findWidget("classLabelPara",AwWidget::nullRef(),_classLabelPara);


    if (ArWidgetUtils::findWidget("executeButton",AwWidget::nullRef(),_executeButton))
	{
   	      /*  void (AReViSignalWidget::*prout)(const AwButton::ClickEvent &evt) ;
		prout=&AReViSignalWidget::_onExecuteButtonClick;
		_executeButton->addClickCB(thisRef(),&AReViSignalWidget::_onExecuteButtonClick);*/

		/*if(_vecFunction.size()>0)
		{ 
		    vector<function_type>::const_iterator e;
		    for(e = _vecFunction.begin(); e != _vecFunction.end(); e++)
    		          _executeButton->addClickCB(thisRef(),*e);
		} 
		else */
		    _executeButton->addClickCB(thisRef(),&AReViSignalWidget::_onExecuteButtonClick);
	}
    
    /** cancel */
    if (ArWidgetUtils::findWidget("cancelButton",AwWidget::nullRef(),_cancelButton))
      _cancelButton->addClickCB(thisRef(),&AReViSignalWidget::_onCancelButtonClick);;
    if (ArWidgetUtils::findWidget("cancelParaButton",AwWidget::nullRef(),_cancelParaButton))
      _cancelParaButton->addClickCB(thisRef(),&AReViSignalWidget::_onCancelParaButtonClick);

  }

  void AReViSignalWidget::_clear()
  {
    /** delete operations/parameters widgets */
    while(_signalsArray->getNbVisibleWidgets() > 0)
      {
	_signalsArray->removeWidget(_signalsArray->accessWidget(0));	
      }

    if (_parametersArray)
    while(_parametersArray->getNbVisibleWidgets() > 0)
      {
	_parametersArray->removeWidget(_parametersArray->accessWidget(0));	
      }

    _primitiveParameters.clear();
    _entitiesParameters.clear();
    _sigs.clear();
  }
  void AReViSignalWidget::_onCancelButtonClick(const AwButton::ClickEvent &evt)
  {
    _focus(false);
    _window->setVisible(false);
    _clear();
  }

  void AReViSignalWidget::_onCancelParaButtonClick(const AwButton::ClickEvent &evt)
  {
    _focus(false);
    _parametersWindow->setVisible(false);
    _clear();
  }

  void AReViSignalWidget::_onExecuteButtonClick(const AwButton::ClickEvent &evt)
  {
      bool execute = true;


    map<string, shared_ptr<ValueSpecification> > values;
    map<string, ArRef<AwEntry> >::const_iterator prim;
    map<ArRef<AwRadioGroup>, vector<shared_ptr<Entity> > >::const_iterator enti;
  
    for(prim = _primitiveParameters.begin(); prim != _primitiveParameters.end() && execute; ++prim)
      {
        if((*prim).second->getText() == "")
          {
            //_setTextErrorDial("Please fill in all fields.");
            //_showErrorDial(true);
            execute = false;
          }
        else
          {
            values[prim->first] = _properties[prim->first]->getType()->createValueFromString((*prim).second->getText());
          }
      }
    for(enti = _entitiesParameters.begin(); enti != _entitiesParameters.end() && execute; enti++)
      {
        string paramName = "parameter";
        shared_ptr<Entity> entity = _findEntityInGroup((*enti).first, (*enti).second);
        shared_ptr<InstanceValue> val = make_shared<InstanceValue>(entity);
        map<ArRef<AwRadioGroup>, string >::const_iterator pi = _entitiesParametersName.find(enti->first);
        if (pi != _entitiesParametersName.end() ) paramName = pi->second;
        values[paramName] = val;
      }

    if(execute)
      {
        _focus(false);
        _parametersWindow->setVisible(false);
        _clear();
cerr << " SEND : " << _entity->getName() << " Signal : " << _currentSignal->getName() << endl;
       
#if 0
        _window3D->sendSignal(_entity,_currentSignal->getName(), values);
#else
//        VRApplication::getInstance()->sendSignal(_entity,_currentSignal->getName(), values);
#endif
      }

  }

  /** call operation when button was clicked */
  void AReViSignalWidget::_onSignalButtonClick(const AwButton::ClickEvent &evt)
  {
    _focus(false);
    map<string,string> params;
    string signalName = evt.button->accessLabel()->getText();

    shared_ptr<Signal> signal = _sigs[signalName];  
    _currentSignal = signal;
    _properties = signal->getAttributes(); 


    if (_properties.size() !=0)
    {
    map<string, shared_ptr<Property> >::iterator it;
    for(it = _properties.begin(); it != _properties.end(); it++)
      {
	string type = it->second->getType()->getName();
	string param = it->first;

	/** it's a primitive type */	
	if(type == "real" || type == "double" || type == "integer" || type == "int" || type == "boolean" || type == "bool" || type == "string")
	  {
	    ArRef<AwLabel> label = AwLabel::NEW();
	    label->setText(param + " : " +  type);
	    label->setTransient(false);
	    
	    ArRef<AwEntry> entry = AwEntry::NEW();
	    entry->setMultiline(false);
	    entry->setTransient(false);
	    entry->setLimitSize(15);
	    entry->setRequestSize(150, 30);
	    
	    if(type == "real" || type == "double" || type == "integer" || type == "int")
		entry->setNumericMode(true);
	    else
		entry->setNumericMode(false);
	    _primitiveParameters.insert(pair<string, ArRef<AwEntry> >(param, entry));
	    _parametersArray->addWidget(label);
	    _parametersArray->addWidget(entry);
	  }

	/** it's an entity */
	else
	  {
	    ArRef<AwArray> container = AwArray::NEW();
	    ArRef<AwMenuButton> menu = AwMenuButton::NEW();
	    ArRef<AwArray> liste = AwArray::NEW();
	    ArRef<AwRadioGroup> group = AwRadioGroup::NEW();
	    ArRef<AwLabel> showmore = AwLabel::NEW();
	    ArRef<AwLabel> labelType = AwLabel::NEW();
	    ArRef<AwLabel> labelParam = AwLabel::NEW();

	    container->setRenderer3D(_viewer);
	    container->setTransient(false);
	    container->setWidgetsSizeFixed(true);

	    showmore->setRenderer3D(_viewer);
	    showmore->setTransient(false);	    
	    
	    labelType->setRenderer3D(_viewer);
	    labelType->setTransient(false);	    
	    labelType->setText(": " + type);
	    
	    labelParam->setRenderer3D(_viewer);
	    labelParam->setTransient(false);	    
	    labelParam->setText(param);

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

	    /*ArRef<AwLabel> label = AwLabel::NEW();
	    label->setText("prout");
	    label->setTransient(false);
	    label->setRenderer3D(_viewer);
	    ArRef<AwLabel> label2 = AwLabel::NEW();
	    label2->setText("fsdfsdfds");
	    label2->setTransient(false);
	    label2->setRenderer3D(_viewer);

	    liste->addWidget(label);
	    ArRef<AwSlider> slider = AwSlider::NEW();
	    liste->addWidget(slider);
	    container->addWidget(liste);*/



	    _parametersArray->addWidget(container);
	  }
      } 

       _parametersWindow->setPosition(_mouse->getAxis("x")->getValue(), _mouse->getAxis("y")->getValue());
       _parametersWindow->setVisible(true);

    }
    else 
    {
      map<string, shared_ptr<ValueSpecification> > values;
#if 0
      _window3D->sendSignal(_entity,_currentSignal->getName(), values);
#else
//      VRApplication::getInstance()->sendSignal(_entity,_currentSignal->getName(), values);
#endif

      cerr << "areviSignalWidget OK" << endl;
    }
    
    _window->setVisible(false);
    _window->setPosition(_mouse->getAxis("x")->getValue(),
			 _mouse->getAxis("y")->getValue());
    _mouse->setMouseCapture(false);
  }

  void AReViSignalWidget::_focus(bool focus)
  {
    //_mouse->setMouseCapture(!focus);
    if(focus)
      _mouse->setRenderer(Renderer3D::nullRef());
    else
      _mouse->setRenderer(_viewer);
  }
 
  void AReViSignalWidget::_getSignals()
  {
    Classifier::OwnedBehaviorMap behaviors = _entity->getClassifier()->getOwnedBehavior();
    map<string, shared_ptr<Behavior> >::iterator itB;
    for (itB = behaviors.begin(); itB != behaviors.end(); itB++)
      {
	shared_ptr<StateMachine> stateMachine = shared_dynamic_cast<StateMachine>(itB->second);
	shared_ptr<Region> region = stateMachine->getRegion_at(0);
	if (region)
	  {
	    vector< shared_ptr<Transition> > transitions = region->getTransition();
	    for (unsigned int iTrans = 0; iTrans < transitions.size() ; iTrans++)
	      {
		vector< shared_ptr<Trigger> > triggers = transitions[iTrans]->getTrigger();
		for (unsigned int iTrig = 0; iTrig < triggers.size(); iTrig++)
		  {
		    shared_ptr<Event> event = triggers[iTrig]->getEvent();
		    if (event)
		      {
			if (event->getType() == "SignalEvent")
			  {
                            string sigName = (shared_dynamic_cast<SignalEvent>(event))->getSignalClass()->getName();
		            map<string, shared_ptr<Signal> >::iterator it = _sigs.find(sigName);
                            if (it == _sigs.end())
                            {
                               _sigs[sigName]=(shared_dynamic_cast<SignalEvent>(event))->getSignalClass();
			       ArRef<AwButton> signalButton = AwButton::NEW();
			       ArRef<AwLabel> signalLabel = AwLabel::NEW();
			       signalLabel->setText((shared_dynamic_cast<SignalEvent>(event))->getSignalClass()->getName());
			       signalLabel->setVisible(true);
			       signalButton->setLabel(signalLabel);
			       signalButton->setRenderer3D(_viewer);
			       signalButton->setTransient(false);
			       signalButton->setVisible(true);
			       signalButton->addClickCB(thisRef(),&AReViSignalWidget::_onSignalButtonClick);
			       _signalsArray->addWidget(signalButton);  
                            }
			  }
		      }
		  }
	      }
	  }

      }
  }


  void AReViSignalWidget::_findEntities(string type, ArRef<AwRadioGroup> group, ArRef<AwLabel> labelParam)
  {
    vector<shared_ptr<Entity> > entities = _environment->getEntities(); 
    vector<shared_ptr<Entity> > params;
    vector<shared_ptr<Entity> >::const_iterator e;
    string ParamName = labelParam->getText();
    for(e = entities.begin(); e != entities.end(); e++)
      {
	//if((*e)->getClassifier()->getName() == type)
	if((*e)->getClassifier()->isAByName(type))
	  {
	    
	    /** we found an entity which had the same type */
	    double tailleEmplacement = (double)*(*e)->getProperty("taille")->getValue();//CRADO -> a régler avec une liste defilante...
	    double tailleFlacon = (double)*(_entity)->getProperty("taille")->getValue();//CRADO
	    bool emplacementLibre = (bool)*(*e)->getProperty("isFree")->getValue();//CRADO

	    if(tailleEmplacement==tailleFlacon && emplacementLibre)//CRADO
		{
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
		    entity->addClickCB(thisRef(),&AReViSignalWidget::_onButtonParameterClick);

		    group->addWidget(entity);
		    ArRef<AwRadioButton> radio = ar_down_cast<AwRadioButton>(group->accessWidget(group->getSelectedButton()));
		    labelParam->setText(radio->accessLabel()->getText());
		    params.push_back(*e);
		}
	  }
      }
    pair<map<ArRef<AwRadioGroup>,vector<shared_ptr<Entity> > >::iterator,bool> ret;
    _entitiesParameters.insert(make_pair(group, params));
    _entitiesParametersName.insert(make_pair(group, ParamName));
  }
  
  shared_ptr<Entity> AReViSignalWidget::_findEntityInGroup(ArRef<AwRadioGroup> group, vector<shared_ptr<Entity> > entities)
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


void AReViSignalWidget::_onButtonParameterClick(const AwButton::ClickEvent &evt)
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

}

// 
// AReViSignalWidget.cpp ends here
