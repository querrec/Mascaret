using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    class CallBehaviorBehaviorExecution: BehaviorExecution
    {
        public CallBehaviorAction action;
        int step = 0;

        OrganisationalEntity organisation = null;
        Procedure procedure = null;

        public CallBehaviorBehaviorExecution(CallBehaviorAction paction, InstanceSpecification host, Dictionary<string, ValueSpecification> p)
            : base(paction, host, p, false)
        {
            this.action = paction;
        }

        public override double execute(double dt)
        {
            if (step == 0)
            {
               // StreamWriter file = new StreamWriter("CallBehaviorExecution.txt");
               // file.AutoFlush = true;
               // file.WriteLine("CallBehaviorExecution"); file.Flush();
                if (action.Behavior.GetType().ToString() == "Mascaret.Activity")
                {
                    string procedureName = action.Behavior.name;
                    Agent agent = (Agent)Host;
                    string owner = action.Owner.name;

               //     file.WriteLine("Agent : " + agent.name); file.Flush();
               //     file.WriteLine("ProcedureToDo : " + procedureName); file.Flush();
               //     file.WriteLine("From Action : " + action.name); file.Flush();
               //     file.WriteLine("From Procedure : " + owner); file.Flush();

                    ProceduralBehavior pbehavior = (ProceduralBehavior)(agent.getBehaviorExecutingByName("ProceduralBehavior"));
                    List<ProcedureExecution> runningProcedures = pbehavior.runningProcedures;

               //     file.WriteLine("NB Running procs : " + runningProcedures.Count); file.Flush();
                    for (int i = 0; i < runningProcedures.Count; i++)
                    {
                      //  file.WriteLine("Running proc : " + runningProcedures[i].procedure.name); file.Flush();
                        if (runningProcedures[i].procedure.name == owner)
                        {
                            organisation = runningProcedures[i].organisation;
                          //  file.WriteLine("Organisation : " + organisation.name); file.Flush();
                        }
                    }

                    OrganisationalStructure os = organisation.Structure;
                    List<Procedure> procs = os.Procedures;
                    for (int iP = 0; iP < procs.Count; iP++)
                    {
                        if (procs[iP].name == procedureName)
                        {
                            procedure = procs[iP];
                        }
                    }

                    List<RoleAssignement> assigns = organisation.RoleAssignement;
            //        file.WriteLine("Assigns : " + assigns.Count); file.Flush();
                    for (int iAss = 0; iAss < assigns.Count; iAss++)
                    {
                        Agent agt = MascaretApplication.Instance.AgentPlateform.Agents[assigns[iAss].Agent.toString()];
                        AgentBehaviorExecution pbehavior2 = agt.getBehaviorExecutingByName("ProceduralBehavior");

                        if (pbehavior2 != null)
                        {
                         //   file.WriteLine("Procedure launched for " + agt.name); file.Flush();
                            ProceduralBehavior procBehave = (ProceduralBehavior)(pbehavior2);

                            Dictionary<string, ValueSpecification> procParams = new Dictionary<string, ValueSpecification>();

                            procBehave.pushProcedureToDo(procedure, organisation, assigns[iAss].Role, procParams);
                        }
                    }

                }
             //   file.Close();
                step++;
            }
            else
            {
                OrganisationalStructure os = organisation.Structure;
                List<RoleAssignement> assigns = organisation.RoleAssignement;
                for (int iAss = 0; iAss < assigns.Count; iAss++)
                {
                    Agent agt = MascaretApplication.Instance.AgentPlateform.Agents[assigns[iAss].Agent.toString()];
                    AgentBehaviorExecution pbehavior = agt.getBehaviorExecutingByName("ProceduralBehavior");

                    if (pbehavior != null)
                    {
                        ProceduralBehavior procBehave = (ProceduralBehavior)(pbehavior);
                        List<ProcedureExecution> runningProcedures = procBehave.runningProcedures;
                        bool foundP = false;
                        for (int iP = 0; iP < runningProcedures.Count; iP++)
                        {
                            if (runningProcedures[iP].procedure.name == procedure.name)
                            {
                                foundP = true;
                            }
                        }
                        if (!foundP) return 0;

                    }
                }
            }
            return 0.1;
        }
    
    }
}
