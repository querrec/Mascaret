using System;
using System.IO;
using System.Collections.Generic;



namespace Mascaret
{
    public class ActionNode : ActivityNode
    {
        private string fml;
        public string Fml
        {
            get { return fml; }
            set { fml = value; }
        }

        private Action action;
        public Action Action
        {
            get { return action; }
            set { action = value; }
        }


        private BehaviorExecution currentExecution;
        public BehaviorExecution CurrentExecution
        {
            get { return currentExecution; }
            set { currentExecution = value; }
        }

        //default parameters kind = "action"
        public ActionNode(string name, string kind)
            : base(name, kind)
        {
        }

        //default parameters sync = false
        public void start(InstanceSpecification host, Dictionary<string, InstanceSpecification> affectations, bool sync)
        {
           // StreamWriter file = MascaretApplication.Instance.logfile;
            if (action == null)
            {
               // file.WriteLine("Le noeud " + name + " n'a pas d'action associée!!!"); file.Flush();
                return;
            }

            Dictionary<string, ValueSpecification> param = new Dictionary<string, ValueSpecification>();
            if (action.Kind == "SendSignal")
            {
                
                if (((SendSignalAction)action).Target == null)
                {
                    foreach (ActivityEdge currentEdge in Outgoing)
                    {
                        if (currentEdge.Target.Kind == "object")
                        {
                            if (affectations.ContainsKey(currentEdge.Target.name))
                            {
                                System.Console.WriteLine("Sending signal to : " + currentEdge.Target.name);
                                SendSignalAction ssAct = (SendSignalAction)action;
                                ssAct.Target = new SendSignalTarget();
                                ssAct.Target.target = affectations[currentEdge.Target.name];
                            }

                            else
                            {
                                System.Console.WriteLine("affectation of " + currentEdge.Target.name + " not found");
                            }

                        }
                    }
                }
            }
            if (action.Kind == "CallOperation")
            {
               // file.WriteLine("Executing Call operation" + action.name + " " + ((CallOperationAction)action).Operation.getFullName()); file.Flush();

                if (((CallOperationAction)action).isDynamic())
                {
                    param = ((CallOperationAction)action).getParameters();

                }
                else
                {
                    foreach (InputPin currentPin in action.InputPins)
                    {

                        string paramName = currentPin.getIncomingObjectNode()[0].name;
                      //  file.WriteLine(" Looking for : " + paramName); file.Flush();
                        if (!affectations.ContainsKey(currentPin.getIncomingObjectNode()[0].name))
                        {
                    //        file.WriteLine(((CallOperationAction)action).Operation.getFullName() + " from " + action.Owner.name + " MISS " + currentPin.getIncomingObjectNode()[0].name); file.Flush();
                        }
                        InstanceValue val = new InstanceValue(affectations[currentPin.getIncomingObjectNode()[0].name]);
                    //    file.WriteLine("..... Trouve " + val.ToString()); file.Flush();
                        param.Add(currentPin.name, val);
                    }
                }

                if (Activity != null)
                {
                    //	action.Context(Activity.Context);
                }

            }
            currentExecution = BehaviorScheduler.Instance.executeBehavior(action, host, param, sync);
        }

        public void stop()
        {
            if (currentExecution != null)
                currentExecution.stop();
        }

        public bool isRunning()
        {
            if (currentExecution == null)
                return false;
            else
                return !currentExecution.IsFinished;
        }
    }
}
