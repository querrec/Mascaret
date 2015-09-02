using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    public class Connector
    {

        protected List<ConnectorEnd> ends = new List<ConnectorEnd>();

        public List<ConnectorEnd> getEnds()
        {
            return ends;
        }

        public void addEnd(Property prop)
        {
            ConnectorEnd ce = new ConnectorEnd();
            ce.PartWithPort = prop;
            ends.Add(ce);
        }

        public void addEnd(ConnectorEnd ce)
        {
            ends.Add(ce);
        }

    }
}
