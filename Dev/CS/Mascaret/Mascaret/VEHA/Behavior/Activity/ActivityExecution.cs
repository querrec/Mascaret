using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class ActivityExecution
    {

        private List<ActivityNode> runningNodes = new List<ActivityNode>();
        public List<ActivityNode> RunningNodes
        {
            get { return runningNodes; }
            set { runningNodes = value; }
        }


        private List<ActionNode> toExecetuteNodes = new List<ActionNode>();
        public List<ActionNode> ToExecetuteNodes
        {
            get { return toExecetuteNodes; }
            set { toExecetuteNodes = value; }
        }


        private List<KeyValuePair<string, string>> actionsDone = new List<KeyValuePair<string, string>>();
        public List<KeyValuePair<string, string>> ActionsDone
        {
            get { return actionsDone; }
            set { actionsDone = value; }
        }


        private List<KeyValuePair<string, string>> actionsRunning = new List<KeyValuePair<string, string>>();
        public List<KeyValuePair<string, string>> ActionsRunning
        {
            get { return actionsRunning; }
            set { actionsRunning = value; }
        }


        private Activity activity;
        public Activity Activity
        {
            get { return activity; }
            set { activity = value; }
        }

        public ActivityExecution(Activity activity)
        {
            this.activity = activity;
            toExecetuteNodes = activity.Initial.getOutgoingActionNode();
        }

        public List<ActionNode> getNextActions(string roleName)
        {
            List<ActionNode> toExec = new List<ActionNode>();
            updateNodes();
            foreach (ActionNode currentNode in toExecetuteNodes)
            {
                if (currentNode.Partitions[0].name == roleName)
                    toExec.Add(currentNode);
            }

            return toExec;
        }

        //Ce n'est peut être pas très joli mais au moins ça fait ce qu'il faut :D
        protected string clean(string toClean)
        {
            return toClean.Replace("/", "").Replace("(", "").Replace(")", "").Replace("^", "");
        }

        public bool isFinished()
        {
            return (toExecetuteNodes.Count == 0 && runningNodes.Count == 0);
        }

        public void informActionRunning(string roleName, string actionName)
        {
            actionsRunning.Add(new KeyValuePair<string, string>(roleName, actionName));
        }

        public void informActionDone(string roleName, string actionName)
        {
            actionsDone.Add(new KeyValuePair<string, string>(roleName, actionName));
        }

        public void printState()
        {
            System.Console.WriteLine("Noeud a executer : ");
            foreach (ActionNode currentNode in toExecetuteNodes)
            {
                System.Console.WriteLine("-->" + currentNode.name + " par " + currentNode.Partitions[0].name);
            }
            System.Console.WriteLine("Noeud en cours d'execution : ");
            foreach (ActionNode currentNode in runningNodes)
            {
                System.Console.WriteLine("-->" + currentNode.name + " par " + currentNode.Partitions[0].name);
            }
            System.Console.WriteLine("Evenement running recu : ");
            foreach (KeyValuePair<string, string> currentNode in actionsRunning)
            {
                System.Console.WriteLine("-->" + currentNode.Key + " running " + currentNode.Value);
            }
            System.Console.WriteLine("Evenement done recu : ");
            foreach (KeyValuePair<string, string> currentNode in actionsDone)
            {
                System.Console.WriteLine("-->" + currentNode.Key + " have done " + currentNode.Value);
            }
        }

        public void updateNodes()
        {
            foreach (KeyValuePair<string, string> currentPair in actionsRunning)
            {
                int i = 0, j = 0;
                bool increase = true;
                foreach (ActionNode currentNode in toExecetuteNodes)
                {
                    if (currentPair.Key == currentNode.Partitions[0].name && currentPair.Value == clean(currentNode.name))
                    {
                        runningNodes.Add(currentNode);
                        toExecetuteNodes.RemoveRange(j, 1);
                        actionsRunning.RemoveRange(i, 1);
                        increase = false;
                        break;
                    }
                    else
                        j++;
                    j++;
                }
                if (increase)
                    i++;
                i++;
            }

            foreach (KeyValuePair<string, string> currentPair in actionsDone)
            {
                int i = 0, j = 0;
                bool increase = true;
                foreach (ActionNode currentNode in runningNodes)
                {
                    if (currentPair.Key == currentNode.Partitions[0].name && currentPair.Value == clean(currentNode.name))
                    {
                        runningNodes.RemoveRange(j, 1);
                        actionsDone.RemoveRange(i, 1);
                        break;
                    }
                    else
                        j++;
                    j++;
                }
                if (increase)
                    i++;
                i++;
            }

        }


    }
}
