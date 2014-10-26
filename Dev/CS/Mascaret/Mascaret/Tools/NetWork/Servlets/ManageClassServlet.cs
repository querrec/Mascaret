using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ManageClassServlet : HttpServlet
    {

        public override void manageRequest(HttpRequest req)
        {
            Class classe = null;

            string id = "";
            if (req.parameters.ContainsKey("agent"))
                id = req.parameters["agent"];


            Environment env = MascaretApplication.Instance.getEnvironment();
            InstanceSpecification entity = null;
            if (env.InstanceSpecifications.ContainsKey(id))
                entity = env.InstanceSpecifications[id];

            VirtualHuman human = null;
            if (entity != null) human = (VirtualHuman)(entity);


            if (human != null)
            {
                env = human.KnowledgeBase.Environment;
                if (env != null) classe = _findClasse(env.Model.Package, req.parameters["alias"]);
            }
            else
                classe = _findClasse(MascaretApplication.Instance.Model.Package, req.parameters["alias"]);

            req.response.write("<html>");
            req.response.write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
            req.response.write("<body>");
            req.response.write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
            req.response.write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

            req.response.write("<H2>Description</H2>");
            req.response.write("<ul>");
            req.response.write("<li>");
            req.response.write(classe.name);
            req.response.write("</li>");
            req.response.write("<li>");
            req.response.write(classe.getFullName());
            req.response.write("</li>");
            req.response.write("<li>");
            req.response.write(classe.Summary);
            req.response.write("</li>");
            req.response.write("<li>");
            req.response.write(classe.Description);
            req.response.write("</li>");
            req.response.write("</ul>");

            req.response.write("<HR>");
            req.response.write("<H2>Hierarchie</H2>");

            req.response.write("<H3>Parent</H3>");
            req.response.write("<ul>");
            List<Classifier> parents = classe.Parents;
            for (int iC = 0; iC < parents.Count; iC++)
            {
                Classifier parent = classe.Parents[iC];
                req.response.write("<li><a href=\"Class?alias=");
                req.response.write(parent.name);
                req.response.write("\" target = \"Body\">");
                req.response.write(parent.name);
                req.response.write("</a></li>");
            }
            req.response.write("</ul>");
            //req.response.write("</ul>");


            req.response.write("<HR>");
            req.response.write("<H2>Attributs</H2>");
            req.response.write("<ul>");
            Dictionary<string, Property> attributes = classe.Attributes;
            foreach (KeyValuePair<string, Property> attr in attributes)
            {
                req.response.write("<li>");
                req.response.write(attr.Key);
                req.response.write(" : ");
                req.response.write(attr.Value.Type.name);
                req.response.write("</li>");
            }
            req.response.write("</ul>");

            req.response.write("<HR>");
            req.response.write("<H2>Operations</H2>");
            Dictionary<string, Operation> operations = classe.Operations;

            foreach (KeyValuePair<string, Operation> oper in operations)
            {
                req.response.write("<li>");
                req.response.write(oper.Key);
                req.response.write("( ");
                List<Parameter> parameters = oper.Value.Parameters;
                for (int iP = 0; iP < parameters.Count; iP++)
                {
                    Parameter attr = parameters[iP];
                    req.response.write(attr.name);
                    req.response.write(" : <a href=\"Class?alias=");
                    req.response.write(attr.Type.name);
                    req.response.write("\" target = \"Body\">");
                    req.response.write(attr.Type.name);
                    req.response.write("</a>");
                    if (iP != parameters.Count - 1)
                        req.response.write(", ");
                }
                req.response.write(" )");
                req.response.write(" : ");
                if (oper.Value.Type != null)
                    req.response.write(oper.Value.Type.name);
                else
                    req.response.write("undefined");
                req.response.write("</li>");
            }

            req.response.write("<HR>");
            req.response.write("<H2>Instances</H2>");
            Dictionary<string, InstanceSpecification> instances = classe.Instances;
            foreach (KeyValuePair<string, InstanceSpecification> instance in instances)
            {
                req.response.write("<li>");
                req.response.write("<a href=\"Entity?alias=");
                req.response.write(instance.Key);
                req.response.write("\" target = \"Body\">");
                req.response.write(instance.Key);
                req.response.write("</a>");

            }

            req.response.write("</body>");
            req.response.write("</html>");

        }

        Class _findClasse(Package package, string id)
        {
            if (package.Classes.ContainsKey(id))
                return package.Classes[id];
            else
            {
                Dictionary<string, Package> packages = package.Packages;
                Class c = null;
                foreach (KeyValuePair<string, Package> pkg in packages)
                {
                    c = _findClasse(pkg.Value, id);
                    if (c != null) break;
                }
                return c;
            }
        }

    }
}
