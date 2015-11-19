using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class PedagogicalScenario : Procedure
    {
        private string scene = "";
        public string Scene
        {
            get { return scene; }
            set { scene = value; }
        }

        public PedagogicalScenario(string name) : base(name)
	    {
	    }
    }
}
