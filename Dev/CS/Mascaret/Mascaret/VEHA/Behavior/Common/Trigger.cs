using System;
using System.Collections.Generic;
using System.IO;

namespace Mascaret
{
    public class Trigger : Element
    {

        private MascaretEvent mEvent;
        public MascaretEvent MEvent
        {
            get { return mEvent; }
            set { mEvent = value; }
        }

        public Trigger(string name)
            : base(name)
        {
        }

        public bool equals(Trigger trigger)
        {
            if (mEvent.Type != trigger.mEvent.Type)
                return false;
            if (mEvent.Type == "SignalEvent")
            {
               // StreamWriter file = MascaretApplication.Instance.logfile;
                if (mEvent as SignalEvent != null && ((SignalEvent)mEvent).name == ((SignalEvent)trigger.mEvent).Signal.Classifier.name)
                {
                    return true;
                }
                else
                    return false;
            }
            else if (mEvent.Type == "ChangeEvent")
            {
                Expression exp1 = ((Expression)((ChangeEvent)mEvent).ChangeExpression);
                Expression exp2 = ((Expression)((ChangeEvent)trigger.MEvent).ChangeExpression);
                if (exp1.ExpressionValue == exp2.ExpressionValue)
                    return true;
                else return false;
            }
            else if (mEvent.Type == "TimeEvent")
            {
                LiteralReal value1 = (LiteralReal)((TimeEvent)mEvent).When;
                LiteralReal value2 = (LiteralReal)((TimeEvent)trigger.MEvent).When;

                if (value1.RValue == value2.RValue)
                    return true;
                else return false;
            }
            else
                return false;
        }
    }
}

