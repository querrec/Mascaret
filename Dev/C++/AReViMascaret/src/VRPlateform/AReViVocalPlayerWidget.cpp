// AReViVocalPlayerWidget.cpp --- 
// 
// Filename: AReViVocalPlayerWidget.cpp
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

#include "VRPlateform/AReViVocalPlayerWidget.h"
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
#include "iostream"
#include "fstream"
using namespace std;

namespace AReViMascaret
{
  AR_CLASS_NOVOID_DEF(AReViVocalPlayerWidget,ArObject)

  AReViVocalPlayerWidget::AReViVocalPlayerWidget(ArCW& arCW, AReViWindow3D* window) :
  ArObject(arCW), _viewer(window->getAReViViewer()), _window3D(window)
    {
      _widgetInteractor = WidgetInteractor::NEW();
      _parser = XmlParser::NEW();
      _parser->parseFile("vocalPlayerWidget.xml");

      /** parse & create widgets */
      if(!ArWidgetUtils::readWidgets(_parser->getRoot(),_widgets,_viewer))
        {
	  cerr << "VocalPlayerWidget -- Error reading widgets description" << endl;
	  exit(1);
        }
      /** execute */
      if (ArWidgetUtils::findWidget("executeButton2",AwWidget::nullRef(),_executeButton))
        _executeButton->addClickCB(thisRef(),&AReViVocalPlayerWidget::_onExecuteButtonClick);

      ArRef<AwWidget> widgets = _widgets["executeButton2"];
	widgets->setPosition(200,960); //crade, recup la taille de la window

	initializeMap();
   }


  AReViVocalPlayerWidget::~AReViVocalPlayerWidget() {}


  void AReViVocalPlayerWidget::_onExecuteButtonClick(const AwButton::ClickEvent &evt)
    {
	shared_ptr<ProcedureExecution> procExec = _window3D->getProcExecution();
	vector<shared_ptr<ActionNode> > actionNode = procExec->getActionToExecute();
	if(actionNode.size()>0)
	{
	   map<string, string>::iterator findIt = _soundMap.find(actionNode[0]->getName());
	   map<string, string>::iterator findIt2 = _iconMap.find(actionNode[0]->getName());

	   if (findIt == _soundMap.end())
		return;
	   else
	     {
	        _window3D->sendVocalAssistance(_soundMap[actionNode[0]->getName()]); //envoi assistanceVocale
		//comptage des assistances
		int compteur = _accessAssistancesMap[actionNode[0]->getName()];
		compteur +=1;
		_accessAssistancesMap[actionNode[0]->getName()]=compteur;

		if (findIt2 != _iconMap.end())
	     	     _window3D->sendIconAssistance(_iconMap[actionNode[0]->getName()]); 
	     }
	}
	else
	   cerr << "pas d'action à effectuer dans la procédure, donc pas d'instruction vocale" << endl;


	//parcours la map des logs et ecrit dans un fichier
	ofstream file;
	file.open("logsAssistances.txt");
	map<string, int>::iterator it;
	if (file.is_open())  
	  {
		for(it = _accessAssistancesMap.begin(); it!=_accessAssistancesMap.end(); it++)
			file << it->first << "	" << it->second << endl;
	   file.close();
	  } 
	else 
	    cout << "Error opening file" << endl;

    }

  void AReViVocalPlayerWidget::initializeMap()
    {
        ArRef<XmlParser> parser = XmlParser::NEW();
        parser->parseFile("vocalAssistances.xml");

	ArRef<AReVi::XmlNode> root_node = parser->getRoot();
	ArRef<AReVi::XmlNode> node_Action=root_node->getChild("Action");
	while(node_Action!= NULL)
	  { 		  
		string name =  node_Action->getProperty("name"); 
		string soundFile =  node_Action->getProperty("soundFile");
		_soundMap[name]=soundFile;
		_accessAssistancesMap[name]=0;
		node_Action=node_Action->getNext();
	  }

        ArRef<XmlParser> parser2 = XmlParser::NEW();
        parser2->parseFile("iconeAssistances.xml");

	ArRef<AReVi::XmlNode> root2_node = parser2->getRoot();
	ArRef<AReVi::XmlNode> node2_Action=root2_node->getChild("Action");
	while(node2_Action!= NULL)
	  { 		  
		string name2 =  node2_Action->getProperty("name"); 
		string iconFile =  node2_Action->getProperty("iconFile");
		_iconMap[name2]=iconFile;
		node2_Action=node2_Action->getNext();
	  }

    }
}

// 
// AReViVocalPlayerWidget.cpp ends here
