using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class Role : Element
    {


        private RoleClass roleClass;
        public RoleClass RoleClass
        {
            get { return roleClass; }
            set { roleClass = value; }
        }


        private List<Ressource> ressources = new List<Ressource>();
        public List<Ressource> Ressources
        {
            get { return ressources; }
            set { ressources = value; }
        }

        public Role(string name)
            : base(name)
        {
            roleClass = new RoleClass();
        }

    }
}

