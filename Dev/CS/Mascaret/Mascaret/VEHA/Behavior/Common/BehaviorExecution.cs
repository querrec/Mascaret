using System;
using System.Collections.Generic;

namespace Mascaret
{
    public abstract class BehaviorExecution : System.IEquatable<BehaviorExecution>
    {
        private Behavior specification;
        public Behavior Specification
        {
            get { return specification; }
            set { specification = value; }
        }

        private InstanceSpecification host;
        public InstanceSpecification Host
        {
            get { return host; }
            set { host = value; }
        }

        private bool sync;
        public bool Sync
        {
            get { return sync; }
            set { sync = value; }
        }

        private TimeExpression start;
        public TimeExpression Start
        {
            get { return start; }
            set { start = value; }
        }


        private TimeExpression finish;
        public TimeExpression Finish
        {
            get { return finish; }
            set { finish = value; }
        }


        private bool isFinished;
        public bool IsFinished
        {
            get { return isFinished; }
            set { isFinished = value; }
        }


        private bool isSuspended;
        public bool IsSuspended
        {
            get { return isSuspended; }
            set { isSuspended = value; }
        }


        private TimeExpression lastCalledTime;
        public TimeExpression LastCalledTime
        {
            get { return lastCalledTime; }
            set { lastCalledTime = value; }
        }

        public Dictionary<String, ValueSpecification> parameters;


        public List<ValueSpecification> result;

        public delegate void OnBehaviorStopDelegate(BehaviorExecution be);
        private List<OnBehaviorStopDelegate> cbStop = new List<OnBehaviorStopDelegate>();
        public void addCallbackOnBehaviorStop(OnBehaviorStopDelegate fct)
        {
            cbStop.Add(fct);
        }

        protected void onBehaviorStop()
        {
            foreach (OnBehaviorStopDelegate fct in cbStop)
                fct(this);
        }

        public BehaviorExecution(Behavior behavior, InstanceSpecification host, Dictionary<string, ValueSpecification> p, bool sync)
        {
            specification = behavior;
            this.host = host;
            this.sync = sync;
            isFinished = false;
            isSuspended = false;
            parameters = p;
            result = new List<ValueSpecification>();
        }

        public BehaviorExecution()
        {

        }

        public virtual void init(Behavior behavior, InstanceSpecification host, Dictionary<string, ValueSpecification> p, bool sync)
        {
            specification = behavior;
            this.host = host;
            this.sync = sync;
            isFinished = false;
            isSuspended = false;
            parameters = p;
            result = new List<ValueSpecification>();
        }

        public bool Equals(BehaviorExecution other)
        {
            return this == other;
        }

        public virtual void stop()
        {

            if (!isFinished)
            {
                isFinished = true;
                onBehaviorStop();
            }

        }
        public virtual void pause()
        {
            if (!isSuspended)
            {
                isSuspended = true;
                //cbOnBehaviorRestarted
            }

        }

        public virtual void restart()
        {
            if (isSuspended)
            {
                isSuspended = false;
                //cbOnBehaviorRestarted
            }
        }

        public abstract double execute(double dt);

        //callbacks functions
    }
}

