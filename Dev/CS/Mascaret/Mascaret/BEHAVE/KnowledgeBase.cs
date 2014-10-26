using System;
using System.Collections.Generic;

namespace Mascaret
{

    public class KnowledgeBase : Element
    {

        private Environment environment;
        public Environment Environment
        {
            get { return environment; }
            set { environment = value; }
        }

        public KnowledgeBase(string name)
            : base(name)
        {

        }

    }
}

