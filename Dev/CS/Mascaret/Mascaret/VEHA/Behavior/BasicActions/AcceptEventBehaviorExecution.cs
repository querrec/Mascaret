using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    class AcceptEventBehaviorExecution : BehaviorExecution
    {
        public AcceptEventBehaviorExecution(AcceptEventAction action, InstanceSpecification host, Dictionary<string, ValueSpecification> p)
            : base(action, host, p, false)
        {
            _action = action;
            _timeSpent = false;
        }

        public override double execute(double dt)
        {

            Dictionary<string, ValueSpecification> context = _action.context;
            
            bool result = false;
            double time = 0.2;

            if (_action.getTrigger().MEvent.Type == "ChangeEvent")
            {
                ChangeEvent changeEvent = (ChangeEvent)(_action.getTrigger().MEvent);
                Expression expr = (Expression)(changeEvent.ChangeExpression);
                ValueSpecification resultVS = expr.evaluateExpression(context);
                result = ((LiteralBoolean)(resultVS)).BValue;
                MascaretApplication.Instance.VRComponentFactory.Log("Checking context of AcceptEventAction " + changeEvent.Type);
            }
                /*
            else if (_action->getTrigger()->getEvent()->getType() == "TimeEvent")
            {
                if (!_timeSpent)
                {
                    shared_ptr<TimeEvent> timeEvent = shared_dynamic_cast<TimeEvent>(_action->getTrigger()->getEvent());
                    timeEvent->getWhen()->getDoubleValue(time);
                    if (time != 0)
                        _timeSpent = true;
                    cerr << "Checking context of AcceptEventAction " << timeEvent->getType() << " time to spend : " << time << endl;
                }
                else
                {
                    time = 0;
                    cerr << "TimeEvent is now checked." << endl;
                }
            }*/
            else
                MascaretApplication.Instance.VRComponentFactory.Log("ERROR: This trigger event is not an AcceptEventAction event. This event should be a ChangeEvent. (Is type is " + _action.getTrigger().MEvent.Type + ")");

            if (result)
                return 0;
            else
                return time;
        }

        protected AcceptEventAction _action;
        protected bool _timeSpent;
    }
}
