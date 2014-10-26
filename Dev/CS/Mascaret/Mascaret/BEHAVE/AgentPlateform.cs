using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class AgentPlateform
    {
        public string name;

        private int portNumber = 0;
        public int PortNumber
        {
            get { return portNumber; }
            set { portNumber = value; }
        }


        private List<OrganisationalStructure> structures = new List<OrganisationalStructure>();
        public List<OrganisationalStructure> Structures
        {
            get { return structures; }
            set { structures = value; }
        }


        private List<OrganisationalEntity> organisations = new List<OrganisationalEntity>();
        public List<OrganisationalEntity> Organisations
        {
            get { return organisations; }
            set { organisations = value; }
        }

        /*	 
            private List<ExternalCommunicationManager> communicationManager = new List<ExternalCommunicationManager>();
            public List<ExternalCommunicationManager> CommunicationManager
            {
                get{return communicationManager;}
                set{communicationManager=value;}
            }*/

        [Serializable]
        public struct PairStringAID
        {

            public string str;

            public AID aid;
        }


        private List<PairStringAID> agentsToInform = new List<PairStringAID>();
        public List<PairStringAID> AgentsToInform
        {
            get { return agentsToInform; }
            set { agentsToInform = value; }
        }


        private Dictionary<String, Agent> agents;
        public Dictionary<String, Agent> Agents
        {
            get { return agents; }
            set { agents = value; }
        }

        //default parameters name = "", protNumber = 0, ressourcesDir = "../HttpServerBaseDir", noServer = false
        public AgentPlateform(string name, int portNumber, string ressourceDir, bool noServer)
        {
            System.Console.WriteLine("Instanciate Agent Plateform");
            this.name = name;
            this.portNumber = portNumber;
            //	communicationManager= (HttpCommunicationManager)ScriptableObject.CreateInstance("HttpCommunicationManager");
            //	communicationManager.instantiate(portNumber, ressourceDir, noServer);
            agents = new Dictionary<string, Agent>();
        }


        public void addAgent(Agent agent)
        {
            agents.Add(agent.Aid.toString(), agent);
        }

        public bool isLocalAgent(AID aid)
        {
            return (((aid.PlateformName == name || aid.PlateformName == "localhost") && aid.PlateformPort == portNumber));
        }

        public void addAgentToInform(AID agent, string convId)
        {
            PairStringAID pair = new PairStringAID();
            pair.str = convId;
            pair.aid = agent;
            agentsToInform.Add(pair);
        }

        //non implémentée pour le moment
        public void sendMessage(ACLMessage message)
        {
            List<AID> receivers = message.Receivers;
            for (int i = 0; i < receivers.Count; i++)
            {
                if (receivers[i].PlateformName == "localhost") receivers[i].PlateformName = this.name;
                if (isLocalAgent(receivers[i]))
                {
                    if (this.Agents.ContainsKey(receivers[i].toString()))
                    {
                        Agent receiver = this.Agents[receivers[i].toString()];
                        receiver.postMessage(message);
                        receiver.wakeup();
                    }
                }
                
            }

        }

    }
}
	


