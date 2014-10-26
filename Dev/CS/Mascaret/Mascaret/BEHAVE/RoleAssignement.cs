using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class RoleAssignement
    {


        private AID agent;
        public AID Agent
        {
            get { return agent; }
            set { agent = value; }
        }


        private Role role;
        public Role Role
        {
            get { return role; }
            set { role = value; }
        }


        private OrganisationalEntity organisation;
        public OrganisationalEntity Organisation
        {
            get { return organisation; }
            set { organisation = value; }
        }


        private List<RessourceAssignement> ressourceAssignements = new List<RessourceAssignement>();
        public List<RessourceAssignement> RessourceAssignements
        {
            get { return ressourceAssignements; }
            set { ressourceAssignements = value; }
        }

        public RoleAssignement(OrganisationalEntity organisation)
        {
            role = new Role("");
            agent = new AID("", "", 0);
            this.organisation = organisation;
        }



    }
}



