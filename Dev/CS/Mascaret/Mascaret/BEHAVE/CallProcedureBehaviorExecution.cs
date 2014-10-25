using System.Collections;
using System.Collections.Generic;


public class CallProcedureBehaviorExecution : BehaviorExecution
{
	public CallProcedureAction action;
	//public BehaviorExecution behaviorExecution;
	

	public CallProcedureBehaviorExecution(CallProcedureAction paction, InstanceSpecification host, Dictionary<string,ValueSpecification> p) : base(paction, host,p, false)
	{
		this.action = paction;
		System.Console.WriteLine("CallProcedureAction : " + action.Procedure);
		//behaviorExecution=action.Operation.Method.createBehaviorExecution(this.Host, p,false);
	}

	public override void stop()
	{
		base.stop();
		//behaviorExecution.stop();
	}
	
	public override void restart()
	{
		base.restart();
		//behaviorExecution.restart();
	}
	
	public override void pause()
	{
		base.pause();
		//behaviorExecution.pause();
	}

	public override double execute(double dt)
	{
		MascaretApplication appli = MascaretApplication.Instance;
		
		//bool found = false;
		OrganisationalEntity askedOrg = null;
		Procedure askedProc = null;
		Role askedRole = null;

		List<OrganisationalEntity> orgs = appli.AgentPlateform.Organisations;

		for (int iOrg = 0; iOrg < orgs.Count; iOrg++)
		{
			if (orgs[iOrg].name == action.OrganisationalEntity)
			{
				OrganisationalStructure os = orgs[iOrg].Structure;
				List<Procedure> procs = os.Procedures;
				askedOrg = orgs[iOrg];
				
				for (int iP = 0; iP < procs.Count; iP++)
				{
					System.Console.WriteLine(action.Procedure + " / " + procs[iP].name);
					if (procs[iP].name == action.Procedure)
					{
						askedProc = procs[iP];
						List<RoleAssignement> assigns = orgs[iOrg].RoleAssignement;
						
						for (int iAss = 0; iAss < assigns.Count;iAss++)
						{
							Agent agt =  appli.AgentPlateform.Agents[assigns[iAss].Agent.toString()];
							askedRole = assigns[iAss].Role;

							AgentBehaviorExecution pbehavior = agt.getBehaviorExecutingByName("ProceduralBehavior");
							
							if (pbehavior != null)
							{
								ProceduralBehavior procBehave  = (ProceduralBehavior) (pbehavior);
								
								Dictionary<string,ValueSpecification> procParams = new Dictionary<string,ValueSpecification>();
								
								
								procBehave.pushProcedureToDo(askedProc, askedOrg, askedRole, procParams);
							}
						}
					}
				}
			}
		}

		return 0;
	}
}
