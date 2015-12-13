using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class OrganisationalEntity : Element
    {


        private List<RessourceAssignement> ressourcesAssignement = new List<RessourceAssignement>();
        public List<RessourceAssignement> RessourcesAssignement
        {
            get { return ressourcesAssignement; }
            set { ressourcesAssignement = value; }
        }


        private List<RoleAssignement> roleAssignement = new List<RoleAssignement>();
        public List<RoleAssignement> RoleAssignement
        {
            get { return roleAssignement; }
            set { roleAssignement = value; }
        }


        private OrganisationalStructure structure;
        public OrganisationalStructure Structure
        {
            get { return structure; }
            set { structure = value; }
        }

        public OrganisationalEntity(string name)
            : base(name)
        {
        }

        public RoleAssignement findRoleAssignement(string roleName)
        {
            foreach (RoleAssignement currentRA in roleAssignement)
            {
                if (currentRA.Role.name == roleName)
                    return currentRA;
            }

            return null;
        }

        public RessourceAssignement findRessourceAssignement(string resName)
        {
            
            foreach (RessourceAssignement currentRA in ressourcesAssignement)
            {
                MascaretApplication.Instance.VRComponentFactory.Log("FindRessource :" + currentRA.Ressource.name + " == " + resName);
                if (currentRA.Ressource.name == resName)
                {
                    MascaretApplication.Instance.VRComponentFactory.Log(".... Found");
                    return currentRA;
                }
            }
            return null;
        }

        public void stopProcedure(string procedureName)
        {
            foreach (RoleAssignement currentRA in roleAssignement)
            {
                ACLMessage message = new ACLMessage(ACLPerformative.REQUEST);
                message.Content = "STOP" + procedureName;
                message.Receivers.Add(currentRA.Agent);
                MascaretApplication.Instance.AgentPlateform.sendMessage(message);
            }
        }

        public void stopAllProcedures()
        {
            foreach (RoleAssignement currentRA in roleAssignement)
            {
                ACLMessage message = new ACLMessage(ACLPerformative.REQUEST);
                message.Content = "STOP ALL";
                message.Receivers.Add(currentRA.Agent);
                MascaretApplication.Instance.AgentPlateform.sendMessage(message);
            }
        }

    }
}


