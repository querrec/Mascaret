using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class Agent : InstanceSpecification
    {
        public string emotion;

        private List<AgentBehavior> behaviors = new List<AgentBehavior>();
        public List<AgentBehavior> Behaviors
        {
            get { return behaviors; }
            set { behaviors = value; }
        }


        private List<AgentBehaviorExecution> behaviorsExecution = new List<AgentBehaviorExecution>();
        public List<AgentBehaviorExecution> BehaviorsExecution
        {
            get { return behaviorsExecution; }
            set { behaviorsExecution = value; }
        }


        private AID aid;
        public AID Aid
        {
            get { return aid; }
            set { aid = value; }
        }


        private AgentPlateform plateform;
        public AgentPlateform Plateform
        {
            get { return plateform; }
            set { plateform = value; }
        }


        private Mailbox mailbox;
        public Mailbox Mailbox
        {
            get { return mailbox; }
            set { mailbox = value; }
        }


        private KnowledgeBase knowledgeBase;
        public KnowledgeBase KnowledgeBase
        {
            get { return knowledgeBase; }
            set { knowledgeBase = value; }
        }


        private bool controlledByHuman;
        public bool ControlledByHuman
        {
            get { return controlledByHuman; }
            set { controlledByHuman = value; }
        }

        public BehaviorPlanner behaviorPlanner;

        //default parameters KBName = "default"
        public Agent(AgentPlateform ap, string name, AgentClass agentclass, string KBName)
            : base(name, agentclass)
        {

            this.aid = new AID(name, ap.name, ap.PortNumber);
            this.plateform = ap;
            mailbox = new Mailbox();
            knowledgeBase = new KnowledgeBase(KBName);
            //behaviorPlanner = new BehaviorPlanner(this);
            //behaviorRealizer = new BehaviorRealizer();
            emotion = "joyStrong";
        }

        //default parameters interval = -1, start = true
        public void addBehavior(string behaviorName, double interval, bool start)
        {
            if (getBehaviorByBame(behaviorName) != null)
                return;

            AgentBehavior behavior = new AgentBehavior(behaviorName);
            Dictionary<string, ValueSpecification> param = new Dictionary<string, ValueSpecification>();


            BehaviorExecution be = behavior.createBehaviorExecution((InstanceSpecification)this, param, false);
            //if(be as OneShotBehaviorExecution !=null && start)
            {
                be = BehaviorScheduler.Instance.executeBehavior((Behavior)behavior, (InstanceSpecification)this, param, false);
                if (interval > 0.00)
                {
                    SimpleBehaviorExecution sbe = (SimpleBehaviorExecution)be;
                    if (sbe != null)
                        sbe.Interval = interval;
                }
                behaviorsExecution.Add((AgentBehaviorExecution)be);
            }
            behaviors.Add(behavior);

        }

        public AgentBehavior getBehaviorByBame(string behaviorName)
        {
            foreach (AgentBehavior currentAgentBehavior in behaviors)
            {
                if (currentAgentBehavior.name.ToLower() == behaviorName.ToLower())
                    return currentAgentBehavior;
            }
            return null;
        }

        public AgentBehaviorExecution executeOneShotBehavior(string behaviorName)
        {
            AgentBehavior behavior = getBehaviorByBame(behaviorName);
            Dictionary<string, ValueSpecification> param = new Dictionary<string, ValueSpecification>();

            BehaviorExecution be = behavior.createBehaviorExecution((InstanceSpecification)this, param, false);
            if (be as OneShotBehaviorExecution != null)
            {
                be = BehaviorScheduler.Instance.executeBehavior((Behavior)behavior, (InstanceSpecification)this, param, false);
                return (AgentBehaviorExecution)be;
            }
            return null;
        }

        public AgentBehaviorExecution getBehaviorExecutingByName(string behaviorName)
        {
            System.Console.WriteLine(this.name + " : " + behaviorsExecution.Count);
            foreach (AgentBehaviorExecution currentBe in behaviorsExecution)
            {
                if (currentBe != null)
                    if (currentBe.Specification.name == behaviorName)
                        return currentBe;
            }
            return null;
        }

        public void postMessage(ACLMessage message)
        {
            this.mailbox.postMessage(message);
        }


        public ACLMessage receive()
        {
            return this.Mailbox.check();
        }


        public void send(ACLMessage message)
        {
            message.Sender = this.aid;
            this.plateform.sendMessage(message);
            this.mailbox.send(message);
        }

        public List<ACLMessage> getMessageQueue()
        {
            return mailbox.MessagesQueue;
        }

        public List<ACLMessage> getChecked()
        {
            return mailbox.MessagesChecked;
        }

        public List<ACLMessage> getSent()
        {
            return mailbox.MessagesSent;
        }

        //bilal 19-10-15
        public virtual void addIntention(string fml)
        {
            behaviorPlanner.parseIntention(fml);
        }
        //bilal 19-10-15

        public void wakeup()
        {
        }

        public virtual void start()
        {
            Dictionary<string, ValueSpecification> param = new Dictionary<string, ValueSpecification>();
            foreach (Behavior behavior in this.Behaviors)
            {
                BehaviorExecution be = BehaviorScheduler.Instance.executeBehavior((Behavior)behavior, (InstanceSpecification)this, param, false);
                behaviorsExecution.Add((AgentBehaviorExecution)be);
            }

        }



    }
}

