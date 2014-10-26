using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class Region
    {

        private List<Transition> transitions = new List<Transition>();
        public List<Transition> Transitions
        {
            get { return transitions; }
            set { transitions = value; }
        }


        private List<Vertex> vertices = new List<Vertex>();
        public List<Vertex> Vertices
        {
            get { return vertices; }
            set { vertices = value; }
        }

        public Region()
        {
            //nothing to do
        }

    }
}