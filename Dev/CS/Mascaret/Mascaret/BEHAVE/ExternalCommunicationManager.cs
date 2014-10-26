using System.Collections;

namespace Mascaret
{
    public abstract class ExternalCommunicationManager
    {

        public abstract bool sendDistantMessage(AID aid, ACLMessage msg);

    }
}
