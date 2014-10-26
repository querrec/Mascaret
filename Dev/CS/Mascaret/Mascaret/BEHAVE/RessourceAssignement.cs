using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class RessourceAssignement
    {


        private Entity entity;
        public Entity Entity
        {
            get { return entity; }
            set { entity = value; }
        }


        private Ressource ressource;
        public Ressource Ressource
        {
            get { return ressource; }
            set { ressource = value; }
        }

        public RessourceAssignement()
        {
            ressource = new Ressource();
            entity = new Entity();
        }



    }
}


