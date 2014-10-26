using System.Collections;
using System;

namespace Mascaret
{
    public class ManageSpeakServlet : HttpServlet
    {
        public override void manageRequest(HttpRequest req)
        {
            string id = req.parameters["alias"];

            Environment env = MascaretApplication.Instance.getEnvironment();

            if (!env.InstanceSpecifications.ContainsKey(id))
            {
                req.response.write("<html>");
                req.response.write("<body>");
                req.response.write("Can't find entity: " + id);
                req.response.write("</body>");
                req.response.write("</html>");
                return;
            }
            InstanceSpecification entity = env.InstanceSpecifications[id];

            VirtualHuman human = null;
            try
            {
                human = (VirtualHuman)(entity);
            }
            catch (InvalidCastException e)
            {
                req.response.write("<html>");
                req.response.write("<body>");
                req.response.write("Entity: " + id + " is not an agent");
                req.response.write("</body>");
                req.response.write("</html>");
                return;
            }

            string texte = req.parameters["text"];

            texte = texte.Replace("+", " ");
            texte = texte.Replace("%2C", ",");
            texte = texte.Replace("%3F", "?");
            texte = texte.Replace("%C3%A7", "ç");

            //		human.speak(texte);	

            req.response.write("<html>");
            req.response.write("<body>");
            req.response.write("Entity: " + id + " is speaking : " + texte);
            req.response.write("</body>");
            req.response.write("</html>");
            return;
        }

    }
}
