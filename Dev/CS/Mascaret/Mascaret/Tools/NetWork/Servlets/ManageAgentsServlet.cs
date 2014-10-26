using System;
using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ManageAgentsServlet : HttpServlet
    {
        public override void manageRequest(HttpRequest req)
        {
            req.response.write("<html>");
            req.response.write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
            req.response.write("<body>");
            req.response.write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
            req.response.write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");
            req.response.write("<div class=\"tabs\">");
            req.response.write("<ul>");
            req.response.write("<li><a href=\"Model\"><span>Model</span></a></li>");
            req.response.write("<li class=\"current\"><a href=\"Instances\"><span>Entites</span></a></li>");
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
            req.response.write("<H2>Humains Virtuels</H2>");
            Environment env = MascaretApplication.Instance.getEnvironment();
            req.response.write("<ul>");
            foreach (KeyValuePair<string, InstanceSpecification> instance in env.InstanceSpecifications)
            {
                VirtualHuman human = null;
                try
                {
                    human = (VirtualHuman)(instance.Value);
                }
                catch (InvalidCastException e) { }

                if (human != null)
                {
                    req.response.write("<li><a href=\"Agent?alias=");
                    req.response.write(instance.Key);
                    req.response.write("\" target = \"Body\">");
                    req.response.write(instance.Key);
                    req.response.write("</a></li>");
                }
            }
            req.response.write("</ul>");

            req.response.write("</body>");
            req.response.write("</html>");
        }

    }
}
