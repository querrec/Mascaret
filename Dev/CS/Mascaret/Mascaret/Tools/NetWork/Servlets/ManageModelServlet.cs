using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ManageModelServlet : HttpServlet
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


            req.response.write("<html>");
            req.response.write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
            req.response.write("<body>");
            req.response.write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
            req.response.write("<link href=\"styles.css\" rel=\"stylesheet\" type=\"text/css\">");
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
                req.response.write("<li><a href=\"Instances\"><span>Entites</span></a></li>");
            else
            {
                req.response.write("<li><a href=\"Instances?agent=");
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

            req.response.write("<ul>");

            Model model;
            if (human != null)
            {
                KnowledgeBase kb = human.KnowledgeBase;
                Environment envKB = kb.Environment;
                if (envKB != null)
                    _printPackage(0, req, env.Model.Package, human);

            }
            else
                _printPackage(0, req, MascaretApplication.Instance.Model.Package, null);

            req.response.write("</ul>");

            req.response.write("</body>");
            req.response.write("</html>");
        }


        private void _printPackage(int level, HttpRequest req, Package package, VirtualHuman human)
        {
            if (package == null) return;

            req.response.write("<div class=\"node\">");

            req.response.write("<img src = em_package.png>");
            req.response.write(" ");
            req.response.write(package.name);
            level++;
            req.response.write("<ul>");
            foreach (KeyValuePair<string, Class> cl in package.Classes)
            {
                req.response.write("<P><img src =EndL.png>");
                req.response.write("<img src =em_class.png>");
                req.response.write(" <a href=\"Class?alias=");
                req.response.write(cl.Value.name);
                if (human != null)
                {
                    req.response.write("&agent=");
                    req.response.write(human.name);
                }
                req.response.write("\" target = \"Body\">");
                req.response.write(cl.Value.name);
                req.response.write("</a>");
                req.response.write("</P>");
            }
            foreach (KeyValuePair<string, Package> pkg in package.Packages)
                _printPackage(level, req, pkg.Value, human);
            req.response.write("</ul>");
            req.response.write("</div>");
        }

    }
}
