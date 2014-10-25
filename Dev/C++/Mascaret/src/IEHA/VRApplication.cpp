/*
 * VRApplication.cpp
 *
 *  Created on: 26 févr. 2011
 *      Author: ping
 */

#include "IEHA/VRApplication.h"
#include "IEHA/Visualisation/SkyBox.h"
#include "IEHA/Visualisation/Scene.h"
#include "IEHA/Visualisation/Light.h"
#include "IEHA/Visualisation/Camera.h"
#include "VEHA/Entity/VirtualRealityComponentFactory.h"
#include "VEHA/Entity/PointSpecification.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Behavior/Common/Event.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "HAVE/VirtualHuman.h"
#include "HAVE/Body.h"

#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/Environment.h"

#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/BasicActions/ChangeViewPointAction.h"
#include "VEHA/Behavior/BasicActions/PlayAnimationAction.h"
#include "VEHA/Behavior/BasicActions/StopAnimationAction.h"
#include "VEHA/Behavior/BasicActions/OperationWidgetAction.h"
#include "VEHA/Behavior/BasicActions/SignalWidgetAction.h"
#include "IEHA/Interaction/PaddlePeripheric.h"
#include "IEHA/Interaction/ButtonAxis.h"
#include "IEHA/Interaction/Interaction.h"

#include "Tools/Loaders/ParserHelper.h"
#include "VEHA/Behavior/Common/pluginManager.h"

using namespace VEHA;

namespace IEHA
{
VRApplication * VRApplication::_instance=NULL;
VRApplication::VRApplication()
: MascaretApplication()
{
	if(_instance)
		throw std::runtime_error("Une autre instance de VRApplication a déjà été créée");
	_instance=this;
}

VRApplication::~VRApplication()
{
}

void VRApplication::parse(const string& name)
{
	MascaretApplication::parse(name);
	addEntitiesToScene();
	for(size_t i=_cameras.size(); i--; )
	{
		if(_startScene[i].empty())
		{
			cerr << "[VRApplication Warning] Scene with no name." << endl;
			break;
		}
		shared_ptr<Scene> scene = getScene(_startScene[i]);
		if(scene)
		{
			_cameras[i]->setCurrentScene(scene);
			_cameras[i]->gotoViewPoint(_startViewPoint[i]);
		}
		else
		{
			cerr << "[VRApplication Warning] Scene named " << name << " not found." << endl;
		}
	}
}

void VRApplication::parse(shared_ptr<XmlNode> root)
{
	MascaretApplication::parse(root);

	shared_ptr<XmlNode> sceneNode = root->getChildByName("Scene");
	if (sceneNode)
	{
		shared_ptr<Scene> scene=parseScene(sceneNode);
		_scenes.push_back(scene);
	}

	vector<shared_ptr<XmlNode> > rendererNodes = root->getChildNodesByName("Renderer");
	for(size_t i=0; i<rendererNodes.size(); i++)
	{
		shared_ptr<Window3D> window=parseRenderer(rendererNodes[i]);
		_windows.push_back(window);
	}
	PluginManager::getInstance()->fireMascaretWindowsCreated();

	shared_ptr<XmlNode> pariphericsNode = root->getChildByName("Peripherics");
	if (pariphericsNode)
	{
		parsePeripherics(pariphericsNode);
	}
	shared_ptr<XmlNode> interactionNode = root->getChildByName("Interactions");
	if (interactionNode)
	{
		parseInteractions(interactionNode);
	}
	shared_ptr<XmlNode> navigationNode = root->getChildByName("Navigation");
	if (navigationNode)
	{
		parseNavigation(navigationNode);
	}

	shared_ptr<XmlNode> disNode = root->getChildByName("DistributionServer");
	if (disNode)
	{
		parseDistributionServer(disNode);
	}
	else
	{
		shared_ptr<XmlNode> clientNode = root->getChildByName("DistantServer");
		if (clientNode)
		{
			parseClient(clientNode);
		}
	}
}

shared_ptr<Scene> VRApplication::parseScene(shared_ptr<XmlNode> sceneNode)
{
	string name;
	sceneNode->getPropertyString("name",name);
	shared_ptr<Scene> scene=getVRComponentFactory()->createScene(name);
	shared_ptr<XmlNode> childNode = sceneNode->getFirstChild();
	while (childNode)
	{
		if (childNode->getName() == "Area")
		{
		   string name=childNode->getProperty("name");
		   map<string, shared_ptr<Model> > models = getModels();
        	   map<string, shared_ptr<Model> >::iterator it;
        	   for(it = models.begin();it!=models.end();it++)
		   {
			map<string, shared_ptr<Environment> > environments = it->second->getEnvironments();
                	map<string, shared_ptr<Environment> >::iterator itEnv;
                	for(itEnv = environments.begin();itEnv!=environments.end();itEnv++)
                        {
		           shared_ptr<Zone> zone =itEnv->second->getMainZone();
			   if (zone->getName() == name)
				scene->setZone(zone);
			}
		   }
		}
		else
		if (childNode->getName() == "Decors")
		{
			shared_ptr<SkyBox> skybox(new SkyBox());
			string url;
			if(childNode->getPropertyString("url",url))
			{
				skybox->setUrl(url);
				shared_ptr<XmlNode> pos = childNode->getChildByName("Position");
				if(pos)
				{
					skybox->setPosition(PositionVectorFromNodeProperties(pos));
				}
				shared_ptr<XmlNode> orient = childNode->getChildByName("Orientation");
				if(orient)
				{
					skybox->setOrientation(OrientationVectorFromNodeProperties(orient));
				}
				scene->setSkybox(skybox);
			}

			// lights
			shared_ptr<XmlNode> lightNode = childNode->getFirstChild();
			while(lightNode)
			{
				if(lightNode->getName() == "Light")
				{
					_parseLight(scene, lightNode);
				}
				lightNode = lightNode->getNext();
			}

			// fog
			shared_ptr<XmlNode> fogNode = childNode->getChildByName("Fog");
			if(fogNode)
			{
				_parseFog(scene, fogNode);
			}
		}
		/*else if(childNode->getName() == "Shader")
		{
			// shaders
			if(childNode->getName() == "Shader")
			{
				ShaderParameters shader;
				shader.name=childNode->getProperty("name");
				shared_ptr<XmlNode> vertNode = childNode->getChildByName("VertexProgram");
				if(vertNode)
				{
					shader.urlVert=vertNode->getProperty("url");
				}
				shared_ptr<XmlNode> fragNode = childNode->getChildByName("FragmentProgram");
				if(fragNode)
				{
					shader.urlFrag=fragNode->getProperty("url");
				}
				sceneParameters.shaders.push_back(shader);
			}
		}*/
		childNode = childNode->getNext();
	}

	return scene;
}

shared_ptr<Window3D>
VRApplication::parseRenderer(shared_ptr<XmlNode> rendererNode)
{
	shared_ptr<Window3D> window(getVRComponentFactory()->createWindow3D());
	shared_ptr<Camera> cam(getVRComponentFactory()->createCamera(window));
  _cameras.push_back(cam);
	window->setCamera(cam);

	double nearDistance=0.1,farDistance=1000,fieldOfView=3.1415/3.0;
        bool headLight;

	if(rendererNode->getPropertyReal("near",nearDistance))
		cam->setNearDistance(nearDistance);
	if(rendererNode->getPropertyReal("far",farDistance))
		cam->setFarDistance(farDistance);
	if(rendererNode->getPropertyReal("fieldOfView",fieldOfView))
		cam->setFieldOfView(fieldOfView);
	if(rendererNode->getPropertyBoolean("headLight",headLight))
		cam->setHeadLight(headLight);

	shared_ptr<XmlNode> child = rendererNode->getFirstChild();
	while(child)
	{
		if (child->getName()=="Stereo")
		{
			double stereoEyeSeparation=0.065;
			double stereoViewDistance=1.0;
			child->getPropertyReal("eyeSeparation",stereoEyeSeparation);
			child->getPropertyReal("viewDistance",stereoViewDistance);
			cam->setStereo(true);
			cam->setStereoEyeSeparation(stereoEyeSeparation);
			cam->setStereoViewDistance(stereoViewDistance);
		}
		else if (child->getName()=="Window")
		{
			bool fullscreen=false, decoration=true,mouseCapture=false, simpleInteractor=true;
			int x=0, y=0, width=640,height=480;
			child->getPropertyBoolean("fullscreen",fullscreen);
			child->getPropertyBoolean("decoration",decoration);
			child->getPropertyBoolean("capture-mouse",mouseCapture);
			child->getPropertyInteger("x",x);
			child->getPropertyInteger("y",y);
			child->getPropertyInteger("width",width);
			child->getPropertyInteger("height",height);
			child->getPropertyBoolean("simpleInteractor",simpleInteractor);

			window->setDecoration(decoration);
			window->setSize(width,height);
			window->setMouseCapture(mouseCapture);
			window->enableSimpleInteractor(simpleInteractor);
		}
		else if (child->getName()=="Scene")
		{
			string sceneName,sceneViewpoint;
			child->getPropertyString("name", sceneName);
			child->getPropertyString("viewpoint", sceneViewpoint);
      _startScene.push_back(sceneName);
      _startViewPoint.push_back(sceneViewpoint);
      //FIXME index of _startScene and _startViewPoint could be broken
			//child->getPropertyString("shader",parameters.sceneShader);
		}
		child = child->getNext();
	}
	return window;
}

void VRApplication::parseInteractions(shared_ptr<XmlNode> interactionNode)
{
  for(size_t i=0; i<_windows.size(); i++)
  {
    shared_ptr<Interaction> interaction(new Interaction(_windows[i]));
    shared_ptr<XmlNode> action=interactionNode->getFirstChild();

    while(action)
    {
      if(action->getName() == "SendSignal")
      {
        string signal=action->getProperty("name");
        string target=action->getProperty("target");
        if(target == "")
          target="designated";
        string periphName=action->getProperty("peripheric");
        string buttonName=action->getProperty("button");
        string cursorName=action->getProperty("cursor");
        if(cursorName == "")
          cursorName="system";
        bool pressed;
        action->getPropertyBoolean("pressed",pressed);
        double maxdist=-1;
        action->getPropertyReal("maxdist",maxdist);
        shared_ptr<SendSignalAction> sendSignal(new SendSignalAction());
        sendSignal->setSignalClass(shared_ptr<Signal>(new Signal(signal)));
        shared_ptr<PaddlePeripheric> periph=shared_dynamic_cast<PaddlePeripheric>(_windows[i]->getPeripheric(periphName));
        if(periph)
        {
          shared_ptr<Button> button=periph->getButton(buttonName);
          if(button)
          {
            interaction->addButtonAction(button,sendSignal,target,pressed,cursorName,maxdist);
          }
          else
          {
            cerr << "SendSignal: The \"" << buttonName <<"\" button doesn't exist in the \"" << periphName << "\" peripheric"<<endl;
          }
        }
        else
        {
          cerr << "SendSignal: The \"" << periphName <<"\" peripheric doesn't exist" << endl;
        }
      }
      else if(action->getName() == "CallOperation")
      {
        string classifier=action->getProperty("classifier");
        try
        {
          shared_ptr<Class> targetClass=MascaretApplication::getInstance()->getModel()->getClassByFullname(classifier);
          string operation=action->getProperty("name");
          string target=action->getProperty("target");
          string periphName=action->getProperty("peripheric");
          string buttonName=action->getProperty("button");
          string cursorName=action->getProperty("cursor");
          if(cursorName == "")
            cursorName="system";
          bool pressed;
          action->getPropertyBoolean("pressed",pressed);
          double maxdist=-1;
          action->getPropertyReal("maxdist",maxdist);
          shared_ptr<CallOperationAction> callOp(new CallOperationAction());
          callOp->setOperation(targetClass->getOperations()[operation]);
          shared_ptr<PaddlePeripheric> periph=shared_dynamic_cast<PaddlePeripheric>(_windows[i]->getPeripheric(periphName));
          if(periph)
          {
            shared_ptr<Button> button=periph->getButton(buttonName);
            if(button)
            {
              interaction->addButtonAction(button,callOp,target,pressed,cursorName,maxdist);
            }
            else
            {
              cerr << "CallOperation: The \"" << buttonName <<"\" button doesn't exist in the \"" << periphName << "\" peripheric"<<endl;
            }
          }
          else
          {
            cerr << "CallOperation: The \"" << periphName <<"\" peripheric doesn't exist" << endl;
          }
        }
        catch(std::out_of_range e)
        {
          cerr << "CallOperation: The \"" << classifier <<"\" class doesn't exist (fullname is needed (ex: 'Package1::Package2::MyClass'))" << endl;
        }
      }
      else if(action->getName() == "ChangeViewPoint")
      {
         string viewPointName=action->getProperty("name");
         string viewPointDirection=action->getProperty("direction");
         string target=action->getProperty("target");
         if(target == "")
        target="none";
         string periphName=action->getProperty("peripheric");
         string buttonName=action->getProperty("button");
         string cursorName=action->getProperty("cursor");
         if(cursorName == "")
        cursorName="system";
         bool pressed;
         action->getPropertyBoolean("pressed",pressed);
         double maxdist=-1;
         action->getPropertyReal("maxdist",maxdist);

         shared_ptr<ChangeViewPointAction> changeViewPointAction(new ChangeViewPointAction());
         changeViewPointAction->setCamera(_windows[i]->getCamera());
         if (viewPointName != "") changeViewPointAction->setViewPointName(viewPointName);
         else changeViewPointAction->setViewPointDirection(viewPointDirection);
         shared_ptr<PaddlePeripheric> periph=shared_dynamic_cast<PaddlePeripheric>(_windows[i]->getPeripheric(periphName));
         if(periph)
         {
        shared_ptr<Button> button=periph->getButton(buttonName);
        if(button)
        {
          interaction->addButtonAction(button,changeViewPointAction,target,pressed,cursorName,maxdist);
        }
        else
        {
          cerr << "ChangeViewPoint: The \"" << buttonName <<"\" button doesn't exist in the \"" << periphName << "\" peripheric"<<endl;
        }
         }
         else
           {
        cerr << "ChangeViewPoint: The \"" << periphName <<"\" peripheric doesn't exist" << endl;
         }
                  }
      else if(action->getName() == "PlayAnimation")
      {
         string animationName=action->getProperty("animation");
         string target=action->getProperty("target");
         if(target == "")
        target="designated";
         string periphName=action->getProperty("peripheric");
         string buttonName=action->getProperty("button");
         string cursorName=action->getProperty("cursor");
         if(cursorName == "")
        cursorName="system";
         bool pressed;
         action->getPropertyBoolean("pressed",pressed);
         double maxdist=-1;
         action->getPropertyReal("maxdist",maxdist);

         shared_ptr<PlayAnimationAction> playAnimationAction(new PlayAnimationAction());
         playAnimationAction->setAnimationName(animationName);
         shared_ptr<PaddlePeripheric> periph=shared_dynamic_cast<PaddlePeripheric>(_windows[i]->getPeripheric(periphName));
         if(periph)
         {
        shared_ptr<Button> button=periph->getButton(buttonName);
        if(button)
        {
          interaction->addButtonAction(button,playAnimationAction,target,pressed,cursorName,maxdist);
        }
        else
        {
          cerr << "PlayAnimation: The \"" << buttonName <<"\" button doesn't exist in the \"" << periphName << "\" peripheric"<<endl;
        }
         }
         else
           {
        cerr << "PlayAnimation: The \"" << periphName <<"\" peripheric doesn't exist" << endl;
         }
                  }
      else if(action->getName() == "StopAnimation")
      {
         string animationName=action->getProperty("animation");
         string target=action->getProperty("target");
         if(target == "")
        target="designated";
         string periphName=action->getProperty("peripheric");
         string buttonName=action->getProperty("button");
         string cursorName=action->getProperty("cursor");
         if(cursorName == "")
        cursorName="system";
         bool pressed;
         action->getPropertyBoolean("pressed",pressed);
         double maxdist=-1;
         action->getPropertyReal("maxdist",maxdist);

         shared_ptr<StopAnimationAction> stopAnimationAction(new StopAnimationAction());
         stopAnimationAction->setAnimationName(animationName);
         shared_ptr<PaddlePeripheric> periph=shared_dynamic_cast<PaddlePeripheric>(_windows[i]->getPeripheric(periphName));
         if(periph)
         {
        shared_ptr<Button> button=periph->getButton(buttonName);
        if(button)
        {
          interaction->addButtonAction(button,stopAnimationAction,target,pressed,cursorName,maxdist);
        }
        else
        {
          cerr << "StopAnimation: The \"" << buttonName <<"\" button doesn't exist in the \"" << periphName << "\" peripheric"<<endl;
        }
         }
         else
           {
        cerr << "StopAnimation: The \"" << periphName <<"\" peripheric doesn't exist" << endl;
         }
                  }

      else if(action->getName() == "OpenDocument")
      {
        /*
        string signal=action->getProperty("name");
        string target=action->getProperty("target");
        if(target == "")
          target="designated";
        string periphName=action->getProperty("peripheric");
        string buttonName=action->getProperty("button");
        string cursorName=action->getProperty("cursor");
                      if(cursorName == "")
                       cursorName="system";
        bool pressed;
        action->getPropertyBoolean("pressed",pressed);
        double maxdist=-1;
        action->getPropertyReal("maxdist",maxdist);
        shared_ptr<OpenDocumentAction> openDocument(new OpenDocumentAction());

        shared_ptr<PaddlePeripheric> periph=shared_dynamic_cast<PaddlePeripheric>(getPeripheric(periphName));
        if(periph)
        {
          shared_ptr<Button> button=periph->getButton(buttonName);
          if(button)
          {
            interaction->addButtonAction(button,openDocument,target,pressed,cursorName,maxdist);
          }
          else
          {
            cerr << "OpenDocument: The \"" << buttonName <<"\" button doesn't exist in the \"" << periphName << "\" peripheric"<<endl;
          }
        }
        else
        {
          cerr << "OpenDocument: The \"" << periphName <<"\" peripheric doesn't exist" << endl;
        }
        */
      }
      /*
      else if(action->getName() == "LoadModel")
      {
        string signal=action->getProperty("name");
        string target=action->getProperty("target");
        if(target == "")
          target="designated";
        string periphName=action->getProperty("peripheric");
        string buttonName=action->getProperty("button");
        string cursorName=action->getProperty("cursor");
                      if(cursorName == "")
                       cursorName="system";
        bool pressed;
        action->getPropertyBoolean("pressed",pressed);
        double maxdist=-1;
        action->getPropertyReal("maxdist",maxdist);
        shared_ptr<LoadModelAction> loadModel(new LoadModelAction());

        shared_ptr<PaddlePeripheric> periph=shared_dynamic_cast<PaddlePeripheric>(getPeripheric(periphName));
        if(periph)
        {
          shared_ptr<Button> button=periph->getButton(buttonName);
          if(button)
          {
            interaction->addButtonAction(button,loadModel,target,pressed,cursorName,maxdist);
          }
          else
          {
            cerr << "LoadModel: The \"" << buttonName <<"\" button doesn't exist in the \"" << periphName << "\" peripheric"<<endl;
          }
        }
        else
        {
          cerr << "LoadModel: The \"" << periphName <<"\" peripheric doesn't exist" << endl;
        }
      }*/

      if(action->getName() == "OperationWidget")
      {
                    string target = action->getProperty("target");
                    if(target == "")
                      target="designated";
                    
                    string periphName=action->getProperty("peripheric");
                    string buttonName=action->getProperty("button");
                    string cursorName=action->getProperty("cursor");
                    if(cursorName == "")
                      cursorName="system";
                    bool pressed;
                    action->getPropertyBoolean("pressed",pressed);
                    double maxdist=-1;
                    action->getPropertyReal("maxdist",maxdist);
                    
                    shared_ptr<OperationWidgetAction> operationWidget(new OperationWidgetAction());
                    shared_ptr<PaddlePeripheric> periph=shared_dynamic_cast<PaddlePeripheric>(_windows[i]->getPeripheric(periphName));
                    if(periph)
                      {
                        shared_ptr<Button> button = periph->getButton(buttonName);
                        if(button)
                          {
                            interaction->addButtonAction(button,operationWidget,target,pressed,cursorName,maxdist);
                          }
                        else
                          {
                            cerr << "OperationWidget: The \"" << buttonName <<"\" button doesn't exist in the \"" << periphName << "\" peripheric"<<endl;
                          }
                      }
                    else
                      {
                        cerr << "OperationWidget: The \"" << periphName <<"\" peripheric doesn't exist" << endl;
                      }
                  }


      if(action->getName() == "SignalWidget")
      {
                    string target = action->getProperty("target");
                    if(target == "")
                      target="designated";
                    
                   string periphName=action->getProperty("peripheric");
                    string buttonName=action->getProperty("button");
                    string cursorName=action->getProperty("cursor");
                    if(cursorName == "")
                      cursorName="system";
                    bool pressed;
                    action->getPropertyBoolean("pressed",pressed);
                    double maxdist=-1;
                    action->getPropertyReal("maxdist",maxdist);
                    
                    shared_ptr<SignalWidgetAction> signalWidget(new SignalWidgetAction());
                    shared_ptr<PaddlePeripheric> periph=shared_dynamic_cast<PaddlePeripheric>(_windows[i]->getPeripheric(periphName));
                    if(periph)
                      {
                        shared_ptr<Button> button = periph->getButton(buttonName);
                        if(button)
                          {
                            interaction->addButtonAction(button,signalWidget,target,pressed,cursorName,maxdist);
                          }
                        else
                          {
                            cerr << "SignalWidget: The \"" << buttonName <<"\" button doesn't exist in the \"" << periphName << "\" peripheric"<<endl;
                          }
                      }
                    else
                      {
                        cerr << "SignalWidget: The \"" << periphName <<"\" peripheric doesn't exist" << endl;
                      }
                  }

      action = action->getNext();
    }
    _windows[i]->setInteraction(interaction);
  }
}
void VRApplication::parseNavigation(shared_ptr<XmlNode> navigationNode)
{
  for(size_t i=0; i<_windows.size(); i++)
  {
    shared_ptr<Navigation> nav(new Navigation(_windows[i]->getCamera()));
    string mode=navigationNode->getProperty("mode");
    if(mode == "free")
      nav->setNavigationMode(Navigation::FREE);
    else
      nav->setNavigationMode(Navigation::HUMAN);
    shared_ptr<XmlNode> child=navigationNode->getFirstChild();
    while(child)
    {
      Navigation::NavigationAxis axis=Navigation::UNSET;
      if(child->getName()=="TX")
      {
        axis=Navigation::TX;
      }
      else if(child->getName()=="TY")
      {
        axis=Navigation::TY;
      }
      else if(child->getName()=="TZ")
      {
        axis=Navigation::TZ;
      }
      else if(child->getName()=="Roll")
      {
        axis=Navigation::ROLL;
      }
      else if(child->getName()=="Pitch")
      {
        axis=Navigation::PITCH;
      }
      else if(child->getName()=="Yaw")
      {
        axis=Navigation::YAW;
      }
      if(axis != Navigation::UNSET)
      {
        string periph=child->getProperty("peripheric");
        shared_ptr<PaddlePeripheric> paddle=shared_dynamic_cast<PaddlePeripheric>(_windows[i]->getPeripheric(periph));
        if(paddle)
        {
          string axisName=child->getProperty("axis");
          shared_ptr<Axis> ax=paddle->getAxis(axisName);
          if(ax)
          {
            double speed=1.0;
            child->getPropertyReal("speed",speed);
            nav->addNavigationAxis(ax,axis,speed);
          }
          else
            cerr << "Navigation: Axis "<< axisName << " not found"<<endl;
        }
        else
          cerr << "Navigation: Peripheric "<< periph << " not found"<<endl;
      }
      child=child->getNext();
    }
    _windows[i]->setNavigation(nav);
  }
}
void VRApplication::parsePeripherics(shared_ptr<XmlNode> periphNode)
{
  for(size_t i=0; i<_windows.size(); i++)
  {
    shared_ptr<XmlNode> child=periphNode->getFirstChild();
    while(child)
    {
      if(child->getName()=="Peripheric")
      {
        string periphName=child->getProperty("name");
        shared_ptr<PaddlePeripheric> periph=shared_dynamic_cast<PaddlePeripheric>(_windows[i]->getPeripheric(periphName));
        if(!periph)
        {
          periph=shared_ptr<PaddlePeripheric>(new PaddlePeripheric(periphName));
          _windows[i]->addPeripheric(periph);
        }
        shared_ptr<XmlNode> child2=child->getFirstChild();
        while(child2)
        {
          if(child2->getName()=="ButtonAxis")
          {
            string axisName=child2->getProperty("name");
            shared_ptr<ButtonAxis> axis(new ButtonAxis(axisName));
            shared_ptr<XmlNode> buttonNode=child2->getFirstChild();
            while(buttonNode)
            {
              if(buttonNode->getName() == "Button")
              {
                string periphName2=buttonNode->getProperty("peripheric");
                shared_ptr<PaddlePeripheric> periph2=shared_dynamic_cast<PaddlePeripheric>(_windows[i]->getPeripheric(periphName2));
                if(periph2)
                {
                  string buttonName=buttonNode->getProperty("button");
                  shared_ptr<Button> button=periph2->getButton(buttonName);
                  if(button)
                  {
                    string typeStr=buttonNode->getProperty("type");
                    ButtonAxis::ButtonType type=ButtonAxis::SetValue;
                    if(typeStr=="increase-value")
                      type=ButtonAxis::IncreaseValue;
                    double value;
                    buttonNode->getPropertyReal("value",value);
                    bool pressed=true;
                    buttonNode->getPropertyBoolean("pressed",pressed);
                    axis->addButton(button,pressed,type,value);
                  }
                  else
                  {
                    cerr << "Add ButtonAxis: Button "<< buttonName<< " not found in "<<periphName2<<endl;
                  }
                }
                else
                {
                  cerr << "Add ButtonAxis: Peripheric "<<periphName2 << " not found"<<endl;
                }

              }
              buttonNode=buttonNode->getNext();
            }
            periph->addAxis(axis);
          }
          else if(child2->getName()=="Button") //TODO: mais ce n'est pas forcement très utile...
          {
          }
          child2=child2->getNext();
        }
      }
      child=child->getNext();
    }
  }
}
void VRApplication::parseDistributionServer(shared_ptr<XmlNode> disNode)
{
	string sessionName;
	int port;

	sessionName = disNode->getProperty("session");
	disNode->getPropertyInteger("port",port);

	createDistributionServer(sessionName, port);
}

void VRApplication::parseClient(shared_ptr<XmlNode> clientNode)
{
	string hostname;
	int port;

	hostname = clientNode->getProperty("hostname");
	clientNode->getPropertyInteger("port",port);

	createClient(hostname, port);
}

void VRApplication::_parseLight(shared_ptr<Scene> scene, shared_ptr<XmlNode> lightNode)
{
	//cerr << " LIGHT ############## " << endl;
	shared_ptr<Light> light=getVRComponentFactory()->createLight();
	Scene::Layer l=Scene::Main;
	if(lightNode->getProperty("layer") == "back")
		l=Scene::Back;
	else if(lightNode->getProperty("layer") == "front")
		l=Scene::Front;
	bool directional;
	if(!lightNode->getPropertyBoolean("directional",directional))
		light->setDirectional(true);

	shared_ptr<XmlNode> posLight = lightNode->getChildByName("Position");
	Vector3 posVector;
	double roll=0,pitch=0,yaw=0;
	if(posLight)
	{
		posVector = PositionVectorFromNodeProperties(posLight);
	}
	shared_ptr<XmlNode> orientLight = lightNode->getChildByName("Orientation");
	if(orientLight)
	{
		orientLight->getPropertyReal("roll",roll);
		orientLight->getPropertyReal("pitch",pitch);
		orientLight->getPropertyReal("yaw",yaw);
	}
	shared_ptr<PointSpecification> pos=getVRComponentFactory()->createPoint(posVector,RotationVector(roll,pitch,yaw));
	light->setLocation(pos);
	shared_ptr<XmlNode> colorLight = lightNode->getChildByName("Color");
	if(colorLight)
	{
		light->setColor(ColorFromNodeProperties(colorLight));
	}
	shared_ptr<XmlNode> attLight = lightNode->getChildByName("Attenuation");
	if(attLight)
	{
		double constant,linear,quadratic;
		attLight->getPropertyReal("constant",constant);
		attLight->getPropertyReal("linear",linear);
		attLight->getPropertyReal("quadratic",quadratic);
		light->setAttenuationConstant(constant);
		light->setAttenuationQuadratic(quadratic);
		light->setAttenuationLinear(linear);
	}
	shared_ptr<XmlNode> spotLight = lightNode->getChildByName("SpotProperties");
	if(spotLight)
	{
		double spot_exponent,spot_cut_off;
		spotLight->getPropertyReal("exponent",spot_exponent);
		spotLight->getPropertyReal("cutOff",spot_cut_off);
		light->setSpotCutOff(spot_cut_off);
		light->setSpotExponent(spot_exponent);
	}
	scene->addLight(light,l);
}

void VRApplication::_parseFog(shared_ptr<Scene> scene, shared_ptr<XmlNode> fogNode)
{
	shared_ptr<XmlNode> propertiesNode = fogNode->getChildByName("Properties");
	if(propertiesNode)
	{
		int degree;
		double density;
		propertiesNode->getPropertyInteger("degree",degree);
		propertiesNode->getPropertyReal("density",density);
		scene->setFog(degree, density);
	}

	shared_ptr<XmlNode> colorFog = fogNode->getChildByName("Color");
	if(colorFog)
	{
		scene->setFogColor(ColorFromNodeProperties(colorFog));
	}
}

void VRApplication::addEntitiesToScene()
{
	map<string, shared_ptr<Model> > models = getModels();
	map<string, shared_ptr<Model> >::iterator it;

	//for each models...
	for(it = models.begin();it!=models.end();it++)
	{
		map<string, shared_ptr<Environment> > environments = it->second->getEnvironments();
		map<string, shared_ptr<Environment> >::iterator itEnv;
		//... add all entities of each environment
		for(itEnv = environments.begin();itEnv!=environments.end();itEnv++)
		{
			vector<shared_ptr<Entity> > entities=itEnv->second->getEntities();
			vector<shared_ptr<VirtualHuman> > humans=itEnv->second->getVirtualHumans();
			for(size_t i=0;i<humans.size();i++)
			{
				entities.push_back(humans[i]->getBody());
				if(!humans[i]->getBody()->getParent())
					humans[i]->getBody()->setParent(itEnv->second->getMainZone());
			}
			for(size_t i=0;i<_scenes.size();i++)
			{
				for(size_t j=0;j<entities.size();j++)
				{
					_scenes[i]->addEntity(entities[j]);
					//_scenes[i]->setZone(itEnv->second->getMainZone());
				}
			}
		}
	}
}
bool VRApplication::step()
{
	for(size_t i=_windows.size(); i--; )
  {
		if(_windows[i]->getNavigation())
      _windows[i]->getNavigation()->process();
  }
	return MascaretApplication::step();
}

void VRApplication::sendSignal(shared_ptr<VEHA::Entity> entity, string signalName, map<string, shared_ptr<ValueSpecification> > values)
{
  
   if(_vecFunction.size()>0)
   { 
	   vector<SigCallFunc>::const_iterator e;
	   for(e = _vecFunction.begin(); e != _vecFunction.end(); e++)
	      {
		SigCallFunc f = (*e);
		BehaviorExecution * be = f.behavior; 
		void (BehaviorExecution::*function)(shared_ptr<VEHA::Entity>, string, map<string, shared_ptr<ValueSpecification> >);
		function = f.function;
		(be->*function)(entity,signalName,values);
	       }
   } 
   else
   {		 
	   cerr << "VRApplication::sendSignal" << endl;
	   _getSignals(entity);
	   vector<shared_ptr<StateMachineBehaviorExecution> > stmExec = entity->getStateMachineExecutions();
	   for (unsigned int i = 0; i < stmExec.size(); i++)
	   {

	      shared_ptr<Signal> signalC = _sigs[signalName];
	      shared_ptr<InstanceSpecification> signal (new InstanceSpecification(signalName,signalC));
	      
	      map<string, shared_ptr<ValueSpecification> >::iterator it ;
	      for (it = values.begin(); it != values.end(); it++)
	      {
		 signal->getProperty(it->first)->addValue(it->second);
	      }

	      (shared_dynamic_cast<StateMachineBehaviorExecution>(stmExec[i]))->addSignal(signal);

	   }
   }
}

void VRApplication::_getSignals(shared_ptr<VEHA::Entity> entity)
  {
    Classifier::OwnedBehaviorMap behaviors = entity->getClassifier()->getOwnedBehavior();
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
                            cerr << " ############## " << sigName << endl;
                            _sigs[sigName]=(shared_dynamic_cast<SignalEvent>(event))->getSignalClass();
                          }
                      }
                  }
              }
          }

      }
  }
}
