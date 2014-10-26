using System.Collections;

namespace Mascaret
{
    public class HttpCommunicationManager : ExternalCommunicationManager
    {

        public HttpCommunicationManager(int port, string ressourcesDir, bool noServer)
        {
            httpServer = new HttpServer(8080);
            initServlets();
            //httpServer.Start();
        }

        public override bool sendDistantMessage(AID aid, ACLMessage msg)
        {
            return true;
        }

        public void addServlet(string url, HttpServlet servlet)
        {
        }


        private void initTypeMime()
        {
        }

        private void initServlets()
        {
            httpServer.addServlet("/", new ManageIndexServlet());
            httpServer.addServlet("/Head", new ManageHeadServlet());
            httpServer.addServlet("/Model", new ManageModelServlet());
            httpServer.addServlet("/Instances", new ManageInstancesServlet());
            httpServer.addServlet("/Entity", new ManageEntityServlet());
            httpServer.addServlet("/Organisations", new ManageOrganisationsServlet());
            httpServer.addServlet("/OrgStruct", new ManageOrganisationalStructureServlet());
            httpServer.addServlet("/Class", new ManageClassServlet());
            httpServer.addServlet("/Agents", new ManageAgentsServlet());
            httpServer.addServlet("/Agent", new ManageAgentServlet());
            httpServer.addServlet("/OrgEntity", new ManageOrganisationalEntityServlet());
            httpServer.addServlet("/Procedure", new ManageProcedureServlet());
            httpServer.addServlet("/launchProcedure", new ManageLaunchProcedureServlet());
            httpServer.addServlet("/speak", new ManageSpeakServlet());
        }

        public HttpServer httpServer;
    }
}
