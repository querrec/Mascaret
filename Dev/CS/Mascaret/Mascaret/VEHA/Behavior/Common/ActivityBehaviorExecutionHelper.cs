using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public struct ActivityExecutionToken
    {
        public ActivityNode currentLocation; //can be action, fork or join
        public List<ActivityNode> forkStack; //forks in which the token currently is
        public List<LoopNode> loopNodeStack; //loop nodes in which the token currently is
        public ActivityEdge outgoingEdge; //used to know which branch was associated with this token
    };


    public class ActivityBehaviorExecutionHelper
    {

        protected Dictionary<string, ValueSpecification> _activityParams;
        protected Dictionary<string, InstanceSpecification> _affectations;

        public void buildAffectations(InstanceSpecification host, Activity activity)
        {

        }

        /*public Context _buildOclContext(MapStringValueSpecification parameters)
        {

            return null;
        }*/

    }
}
