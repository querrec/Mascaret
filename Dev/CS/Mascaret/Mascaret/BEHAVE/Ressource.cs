using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class Ressource : Element
    {


        private EntityClass entityClass;
        public EntityClass EntityClass
        {
            get { return entityClass; }
            set { entityClass = value; }
        }

        public Ressource(string name)
            : base(name)
        {
        }

        public Ressource()
            : base("")
        {
        }

    }
}


