using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ActivityExecutionToken
    {
        public ActivityNode currentLocation; //can be action, fork or join
        public List<ActivityNode> forkStack ; //forks in which the token currently is
        public List<LoopNode> loopNodeStack ; //loop nodes in which the token currently is
        public ActivityEdge outgoingEdge; //used to know which branch was associated with this token

        public ActivityExecutionToken()
        {
            forkStack = new List<ActivityNode>();
            loopNodeStack = new List<LoopNode>();
        }
    };


    public class ActivityBehaviorExecutionHelper
    {
        

        protected Dictionary<string, ValueSpecification> _activityParams = new Dictionary<string,ValueSpecification>();
        protected Dictionary<string, InstanceSpecification> _affectations = new Dictionary<string,InstanceSpecification>();

        public void buildAffectations(InstanceSpecification host, Activity activity)
        {

        }

        /*public Context _buildOclContext(MapStringValueSpecification parameters)
        {

            return null;
        }*/

    }
}
