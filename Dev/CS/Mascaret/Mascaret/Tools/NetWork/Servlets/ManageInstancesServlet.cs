using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ManageInstancesServlet : HttpServlet
    {

        public override void manageRequest(HttpRequest req)
        {
            string id = "";
            if (req.parameters.ContainsKey("agent"))
                id = req.parameters["agent"];

            Environment env = MascaretApplication.Instance.getEnvironment();
            InstanceSpecification entity = null;
            if (env.InstanceSpecifications.ContainsKey(id))
                entity = env.InstanceSpecifications[id];


            VirtualHuman human = null;
            if (entity != null) human = (VirtualHuman)(entity);
            if (human != null) env = human.KnowledgeBase.Environment;


            req.response.write("<html>");
            req.response.write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
            req.response.write("<body>");
            req.response.write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
            req.response.write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");
            req.response.write("<div class=\"tabs\">");
            req.response.write("<ul>");
            if (human == null)
                req.response.write("<li class=\"current\"><a href=\"Model\"><span>Model</span></a></li>");
            else
            {
                req.response.write("<li><a href=\"Model?agent=");
                req.response.write(human.name);
                req.response.write("\"><span>Model</span></a></li>");
            }
            if (human == null)
                req.response.write("<li class=\"current\"><a href=\"Instances\"><span>Entites</span></a></li>");
            else
            {
                req.response.write("<li class=\"current\"><a href=\"Instances?agent=");
                req.response.write(human.name);
                req.response.write("\"><span>Entites</span></a></li>");
            }

            req.response.write("<li><a href=\"Agents\"><span>Humains Virtuels</span></a></li>");
            req.response.write("<li><a href=\"Organisations\"><span>Organisations</span></a></li>");
            req.response.write("<li><a href=\"Scenario\"><span>Scenario</span></a></li>");
            req.response.write("<li><a href=\"Application\"><span>Application</span></a></li>");
            req.response.write("<li><a href=\"FIPASL\"><span>FIPA SL</span></a></li>");
            req.response.write("<li><a href=\"XOCL\"><span>XOCL</span></a></li>");
            req.response.write("<li><a href=\"messages\"><span>Mes messages</span></a></li>");

            req.response.write("</ul>");
            req.response.write("</div>");
            req.response.write("<HR>");

            req.response.write("<HR>");
            req.response.write("<H2>Entites</H2>");

            if (env != null)
            {
                req.response.write("<ul>");

                //   for (unsigned int iEnt = 0; iEnt<ents.size(); iEnt++)
                foreach (KeyValuePair<string, InstanceSpecification> instance in env.InstanceSpecifications)
                {
                    req.response.write("<li><a href=\"Entity?alias=");
                    req.response.write(instance.Key);
                    if (human != null)
                    {
                        req.response.write("&agent=");
                        req.response.write(human.name);
                    }
                    req.response.write("\" target = \"Body\">");
                    req.response.write(instance.Key);
                    req.response.write("</a></li>");
                }
                req.response.write("</ul>");
            }

            req.response.write("</body>");
            req.response.write("</html>");
        }
    }
}
