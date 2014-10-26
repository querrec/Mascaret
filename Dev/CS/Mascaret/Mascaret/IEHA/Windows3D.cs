using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    public abstract class Windows3D
    {

        protected List<Peripheric> _peripherics = new List<Peripheric>();

        public Peripheric getPeripheric(string name)
        {
            foreach (Peripheric p in _peripherics)
            {
                if (p.name == name) return p;
            }
            return null;
        }

        public void addPeripheric(Peripheric p)
        {
            _peripherics.Add(p);
        }

        public abstract Entity getSelectedEntity();

    }
}
