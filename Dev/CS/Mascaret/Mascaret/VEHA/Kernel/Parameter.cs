
using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class Parameter : Element
    {

        private Classifier type;
        public Classifier Type
        {
            get { return type; }
            set { type = value; }
        }


        private string direction = "";
        public string Direction
        {
            get { return direction; }
            set { direction = value; }
        }


        public Parameter(string name, string direction, Classifier type)
            : base(name)
        {
            this.type = type;
            this.direction = direction;
        }

    }
}