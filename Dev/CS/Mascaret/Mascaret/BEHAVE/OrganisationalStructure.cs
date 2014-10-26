using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class OrganisationalStructure : Element
    {

        private List<OrganisationalEntity> entities = new List<OrganisationalEntity>();
        public List<OrganisationalEntity> Entities
        {
            get { return entities; }
            set { entities = value; }
        }

        public void addOrganisationalEntity(OrganisationalEntity entity)
        {
            entities.Add(entity);
        }


        private List<Procedure> procedures = new List<Procedure>();
        public List<Procedure> Procedures
        {
            get { return procedures; }
            set { procedures = value; }
        }

        public void addProcedure(Procedure proc)
        {
            procedures.Add(proc);
        }


        private List<Ressource> ressources = new List<Ressource>();
        public List<Ressource> Ressources
        {
            get { return ressources; }
            set { ressources = value; }
        }

        public void addResource(Ressource res)
        {
            ressources.Add(res);
        }


        private List<Role> roles = new List<Role>();
        public List<Role> Roles
        {
            get { return roles; }
            set { roles = value; }
        }

        public void addRole(Role role)
        {
            System.Console.WriteLine(" Adding Role : " + role.name + " in " + this.name);
            roles.Add(role);
        }

        public Role getRole(string roleName)
        {
            foreach (Role currentRole in roles)
            {
                if (currentRole.name == roleName)
                    return currentRole;
            }

            return null;
        }

        public OrganisationalStructure(string name)
            : base(name)
        {
        }

    }
}
