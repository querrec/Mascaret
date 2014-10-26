using System;
using System.Reflection;
using System.Collections.Generic;

namespace Mascaret
{
    public class BehaviorScheduler
    {
        [Serializable]
        public struct SchedInfo
        {
            public SchedInfo(BehaviorExecution b, TimeExpression time)
            {
                be = b;
                nextTime = time;
            }


            public BehaviorExecution be;


            public TimeExpression nextTime;

        }


        private int cycle;
        public int Cycle
        {
            get { return cycle; }
            set { cycle = value; }
        }


        private bool suspended;
        public bool Suspended
        {
            get { return suspended; }
            set { suspended = value; }
        }


        protected double virtualTimeSpeedFactor;
        public double VirtualTimeSpeedFactor
        {
            get { return virtualTimeSpeedFactor; }
            set { virtualTimeSpeedFactor = value; }
        }


        protected TimeExpression startRealTime;


        protected double totalSuspendedRealTime;


        protected TimeExpression lastRealTimeCheck;


        protected TimeExpression startVirtualTime;


        protected TimeExpression currentVirtualTime;


        protected TimeExpression startSuspendTime;


        public static BehaviorScheduler instance;
        public static BehaviorScheduler Instance
        {
            get
            {

                if (instance == null)
                {
                    instance = new BehaviorScheduler();
                }

                return instance;
            }
        }

        private BehaviorScheduler()
        {
            behaviors = new LinkedList<SchedInfo>();
            allBehaviors = new List<BehaviorExecution>();
            currentVirtualTime = new TimeExpression(2000, 1, 1, 0, 0, 0);
        }


        protected List<BehaviorExecution> _suspendedBehaviors;

        //STD::SET REMPLACER PAR LIST (DICTIONNAIRE TROP GOURMAND EN MEMOIRE)! 
        //CONVERSION PAS TOUT A FAIT JUSTE!!!!! SET N'AUTORISE QUE DES VALEURS UNIQUES

        protected List<BehaviorExecution> allBehaviors;
        public List<BehaviorExecution> AllBehaviors
        {
            get { return allBehaviors; }
            set { allBehaviors = value; }
        }

        //std::list<SchedInfo> _behaviors; NOT SERIALIZED BY UNITY. BUT IT IS NEEDED?

        protected LinkedList<SchedInfo> behaviors;
        public LinkedList<SchedInfo> Behaviors
        {
            get { return behaviors; }
            set { behaviors = value; }
        }

        public void destroyInstance()
        {
            instance = null;
        }


        public void Awake()
        {
            suspended = false;
            cycle = 0;
            virtualTimeSpeedFactor = 1.0;
            totalSuspendedRealTime = 0.00;

            TimeExpression currentRealTime = TimeExpression.createTimeOfDay();

            startRealTime = currentRealTime;
            lastRealTimeCheck = currentRealTime;
            startVirtualTime = currentRealTime;
            currentVirtualTime = currentRealTime;
        }

        //anciennement _activate de la classe c++
        public void Update()
        {
            //si on est suspendu on update rien

            //Debug.Log(behaviors.Count);
            //Debug.Log(allBehaviors.Count);

            LinkedList<SchedInfo> behaviorsToAdd = new LinkedList<SchedInfo>();
            if (!suspended)
            {

                _jumpToNext();
                _updateCurrentVirtualTime();
                //Debug.Log(" ############################################################################ ");
                while (behaviors.Count > 0)
                {

                    TimeExpression virtualTime = currentVirtualTime;
                    SchedInfo si = behaviors.First.Value;
                    //Debug.Log("Comparing : " + si.nextTime.TimeExp + " : " + virtualTime.TimeExp);
                    if (si.nextTime.TimeExp > virtualTime.TimeExp)
                        break;
                    else
                    {
                        if (si.be.IsSuspended)
                        {
                            suspendExecutionBehavior(si.be);
                            continue;
                        }
                        if (si.be.IsFinished)
                        {
                            si.be.Finish = virtualTime;
                            deleteExecutionBehavior(si.be);
                            continue;
                        }

                        double dt = (virtualTime.TimeExp - si.be.LastCalledTime.TimeExp) / 10000;
                        //Debug.Log(" DT reel d'execution : " + dt);
                        double next_dt = si.be.execute(dt);

                        si.be.LastCalledTime = virtualTime;


                        if (next_dt > 0.00)
                        {
                            si.nextTime.TimeExp = (long)(next_dt * 1000);
                            behaviorsToAdd.AddLast(si);
                        }
                        else
                        {
                            si.be.stop();
                            si.be.Finish = virtualTime;
                            allBehaviors.Remove(si.be);
                        }

                        behaviors.RemoveFirst();
                    }
                }
                foreach (SchedInfo info in behaviorsToAdd) _insert(info.be, (info.nextTime.TimeExp / 1000));

                cycle++;
            }

        }


        public void setSuspended(bool val)
        {
            TimeExpression currentRealTime = TimeExpression.createTimeOfDay();
            if (suspended && !val)
            {
                _updateCurrentVirtualTime();
                totalSuspendedRealTime += currentRealTime.TimeExp - startSuspendTime.TimeExp;
                suspended = false;
            }
            else if (suspended && val)
            {
                _updateCurrentVirtualTime();
                startSuspendTime = TimeExpression.createTimeOfDay();
                suspended = true;
            }
        }

        //sync =false
        public BehaviorExecution executeBehavior(Behavior behavior, InstanceSpecification host, Dictionary<string, ValueSpecification> p, bool sync)
        {
            _updateCurrentVirtualTime();
            BehaviorExecution newBe = behavior.createBehaviorExecution(host, p, sync);
            if (newBe != null)
            {
                newBe.Start = currentVirtualTime;
                newBe.LastCalledTime = currentVirtualTime;
                if (sync)
                {
                    newBe.execute(0.00);//remplacer le double par un timespan?
                }
                else
                {
                    //Debug.Log("Adding behavior : " + newBe.name);
                    allBehaviors.Add(newBe);
                    _insert(newBe, 0.00);
                }
            }
            else
            {
                System.Console.WriteLine("Error on adding behavior : " + behavior.name + " for " + host.name);
            }
            return newBe;
        }

        public void deleteExecutionBehavior(BehaviorExecution be)
        {
            if (be == null)
                return;
            allBehaviors.Remove(be);
            foreach (SchedInfo info in behaviors)
            {
                if (info.be == be)
                {
                    behaviors.Remove(info);
                    return;
                }
            }
        }

        public void suspendExecutionBehavior(BehaviorExecution be)
        {
            if (be != null)
                return;
            _suspendedBehaviors.Add(be);
            //addCallbackOnBehaviorRestarted
            foreach (SchedInfo info in behaviors)
            {
                if (info.be == be)
                {
                    behaviors.Remove(info);
                    return;
                }
            }

        }

        public void reActivate(BehaviorExecution be)
        {
            if (be != null)
                return;

            deleteExecutionBehavior(be);
            allBehaviors.Add(be);
        }

        public double getElapsedRealTime()
        {
            return TimeExpression.createTimeOfDay().TimeExp - startRealTime.TimeExp - totalSuspendedRealTime;
        }

        public double getElapsedVirtualTime()
        {
            return currentVirtualTime.TimeExp - startVirtualTime.TimeExp;
        }

        public void forceCurrentVirtualTime(TimeExpression exp)
        {
            long dt = currentVirtualTime.TimeExp - startVirtualTime.TimeExp;
            LinkedList<SchedInfo> new_behaviors = new LinkedList<SchedInfo>();
            foreach (SchedInfo info in behaviors)
            {
                dt = info.nextTime.TimeExp - currentVirtualTime.TimeExp;
                TimeExpression nextTime = exp;
                nextTime.TimeExp += dt;
                new_behaviors.AddLast(new SchedInfo(info.be, nextTime));
            }

            behaviors = new_behaviors;
            currentVirtualTime = exp;
            exp.TimeExp -= dt;
            startVirtualTime = exp;
        }

        public void _insert(BehaviorExecution be, double dt)
        {
            if (be == null) return;

            _updateCurrentVirtualTime();

            TimeExpression nextTime = new TimeExpression(2000, 1, 1, 0, 0, 0);
            nextTime.TimeExp = currentVirtualTime.TimeExp + (long)(dt * 1000);
            foreach (SchedInfo info in behaviors)
            {

                if (nextTime.TimeExp < info.nextTime.TimeExp)
                {
                    //behaviors.AddBefore(new LinkedListNode<SchedInfo>(info), new SchedInfo(be,nextTime));
                    behaviors.AddBefore(behaviors.Find(info), new SchedInfo(be, nextTime));
                    return;
                }
            }

            behaviors.AddLast(new SchedInfo(be, nextTime));

        }

        protected void _jumpToNext()
        {
            _updateCurrentVirtualTime();
            double next_dt = 0.02;
            if (behaviors.Count != 0)
            {
                double evaluate = (behaviors.First.Value.nextTime.TimeExp - currentVirtualTime.TimeExp) / virtualTimeSpeedFactor;
                if (evaluate < next_dt && evaluate > 0)
                    next_dt = evaluate;
                else if (evaluate < next_dt && evaluate < 0)
                    next_dt = 0.00;
            }
        }

        protected void _onBehaviorRestarted(BehaviorExecution be)
        {
            if (be == null)
                return;
            foreach (BehaviorExecution currentBe in _suspendedBehaviors)
            {
                _insert(currentBe, 0);
                //currentBe = _suspendedBehaviors.Last;
                //COMMENT ON TRADUIT CE QUI SUIT?
                //_suspendedBehaviors[i]=_suspendedBehaviors.back();
                //_suspendedBehaviors.pop_back();
            }

        }

        public TimeExpression getCurrentVirtualTime()
        {
            _updateCurrentVirtualTime();
            return currentVirtualTime;
        }

        protected void _updateCurrentVirtualTime()
        {

            TimeExpression currentRealTime = TimeExpression.createTimeOfDay();
            //Debug.Log(currentRealTime.TimeExp);
            if (!suspended)
            {
                //currentVirtualTime.TimeExp +=(currentRealTime.TimeExp-lastRealTimeCheck.TimeExp)*virtualTimeSpeedFactor;
                currentVirtualTime = currentRealTime;
            }

            lastRealTimeCheck = currentRealTime;
        }

        public Assembly plugin;
        public BehaviorExecution InstanciateOpaqueBehavior(Behavior behavior, string typeName, InstanceSpecification host, Dictionary<String, ValueSpecification> p)
        {
            //Assembly assembly = Assembly.GetExecutingAssembly();
            if (plugin != null)
            {
                Type type = plugin.GetType(typeName, true);

                BehaviorExecution be = null;
                if (type != null)
                    be = (BehaviorExecution)(Activator.CreateInstance(type));
                else
                    return MascaretApplication.Instance.VRComponentFactory.InstanciateOpaqueBehavior(behavior, typeName, host, p);

                if (be != null)
                    be.init(behavior, host, p, false);
                return be;
            }
            else return MascaretApplication.Instance.VRComponentFactory.InstanciateOpaqueBehavior(behavior, typeName, host, p);


        }



    }
}

