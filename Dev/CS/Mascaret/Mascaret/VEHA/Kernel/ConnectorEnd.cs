using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    public class ConnectorEnd
    {
        protected Property partWithPort = null;
        public Property PartWithPort
        {
            get { return partWithPort; }
            set { partWithPort = value; }
        }

        protected Property role = null;
        public Property Role
        {
            get { return role; }
            set { role = value; }
        }


    }
}
