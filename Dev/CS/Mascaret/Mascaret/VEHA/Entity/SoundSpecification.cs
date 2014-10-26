using System;
using System.Collections.Generic;

namespace Mascaret
{
    public abstract class SoundSpecification : VirtualRealitySpecification
    {
        public string name;

        private double gain;
        public double Gain
        {
            get { return gain; }
            set { gain = value; }
        }


        private double minDistance;
        public double MinDistance
        {
            get { return minDistance; }
            set { minDistance = value; }
        }


        private double maxDistance;
        public double MaxDistance
        {
            get { return maxDistance; }
            set { maxDistance = value; }
        }


        private bool cycle;
        public bool Cycle
        {
            get { return cycle; }
            set { cycle = value; }
        }

        //callbacks missing


        //pourquoi ne fait-on rien de l'url?
        //Default parameters gain =1.0; minDistance = 1.0, maxDistance = 100.0, cycle = false
        public SoundSpecification(string name, string url, double gain, double minDistance, double maxDistance, bool cycle)
            : base(MascaretApplication.Instance.Model.getBasicType("sound"))
        {
            this.name = name;
            this.gain = gain;
            this.minDistance = minDistance;
            this.maxDistance = maxDistance;
            this.cycle = cycle;
        }

        public override ValueSpecification clone()
        {
            throw new NotImplementedException();
        }

        //callbakcs operation


        public abstract void attachTo(PointSpecification point);
        //Default parameters gain =1.0; minDistance = 1.0, maxDistance = 100.0, cycle = false
        public abstract bool play(double gain, double minDistance, double maxDistance, bool cycle);

        //Default parameters gain = 1.0, cycle = false
        public abstract bool play(double gain, bool cycle);
        public abstract bool stop();

    }
}
