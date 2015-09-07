using System;
using System.IO;
using System.Xml.Linq;
using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class VRApplication : MascaretApplication
    {

        public Windows3D window;

        public new static VRApplication Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new VRApplication();
                }
                return (VRApplication)instance;
            }
        }

        public VRApplication()
            : base()
        {
            // window = new Windows3D();
            instance = this;
        }

        public override void parse(string applicationFileName, string baseDir, bool loadAll)
        {
            base.parse(applicationFileName, baseDir, loadAll);
        }

        public override void parse(XElement root, bool loadAll)
        {
            base.parse(root, loadAll);

            XElement interactionNode = root.Element("Interactions");
            if (interactionNode != null)
            {
                parseInteractions(interactionNode);
            }
        }

        protected void parseInteractions(XElement interactionNode)
        {

           

            Interaction interaction = new Interaction();

            foreach (XElement action in interactionNode.Elements())
            {
                if (action.Name.LocalName == "SendSignal")
                {
                    string signal = action.Attribute("name").Value;
                    string target = "";
                    if (action.Attribute("target") != null) target = action.Attribute("target").Value;
                    if (target == "") target = "designated";
                    string periphName = action.Attribute("peripheric").Value;
                    string buttonName = action.Attribute("button").Value;
                    bool pressed = (bool)action.Attribute("pressed");

                    SendSignalAction sendSignal = new SendSignalAction();
                    sendSignal.SignalClass = new Signal(signal);
                   
                    Peripheric periph = this.window.getPeripheric(periphName);
                    Button button = periph.getButton(buttonName);
                    if (button != null)
                    {
                        interaction.addButtonAction(button, sendSignal, target, pressed);
                    }
                }
                else if(action.Name.LocalName == "CallOperation")
                {
                    string classifier=action.Attribute("classifier").Value;
                    Class targetClass=MascaretApplication.Instance.Model.AllClasses[classifier][0];
                    string operation=action.Attribute("name").Value;
                    string target=action.Attribute("target").Value;
                    string periphName=action.Attribute("peripheric").Value;
                    string buttonName=action.Attribute("button").Value;
                    bool pressed = (bool)action.Attribute("pressed");

                    CallOperationAction callOp = new CallOperationAction();
                    callOp.Operation = targetClass.Operations[operation];

                    Peripheric periph = this.window.getPeripheric(periphName);
                    Button button = periph.getButton(buttonName);
                    if (button != null)
                    {
                        interaction.addButtonAction(button, callOp, target, pressed);
                    }
                }   
                else if (action.Name.LocalName == "CallProcedure")
                {
                    string procedure = action.Attribute("name").Value;
                    string organisation = action.Attribute("organisation").Value;

                    string target = "";
                    if (action.Attribute("target") != null) target = action.Attribute("target").Value;
                    if (target == "") target = "designated";
                    string periphName = action.Attribute("peripheric").Value;
                    string buttonName = action.Attribute("button").Value;
                    bool pressed = (bool)action.Attribute("pressed");

                    CallProcedureAction callProc = new CallProcedureAction();
                    callProc.Procedure = procedure;
                    callProc.OrganisationalEntity = organisation;

                    Peripheric periph = this.window.getPeripheric(periphName);
                    Button button = periph.getButton(buttonName);
                    if (button != null)
                    {
                        interaction.addButtonAction(button, callProc, target, pressed);
                    }
                }
            }

        }


    }
}
