using System.Collections;

public abstract class ExternalCommunicationManager 
{

	public abstract bool sendDistantMessage(AID aid, ACLMessage msg);

}
