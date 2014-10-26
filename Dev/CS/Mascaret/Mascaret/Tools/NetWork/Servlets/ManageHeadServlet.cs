using System.Collections;

namespace Mascaret
{
    public class ManageHeadServlet : HttpServlet
    {

        public override void manageRequest(HttpRequest req)
        {
            Package package = MascaretApplication.Instance.Model.Package;


            req.response.write("<html>");
            req.response.write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
            req.response.write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
            req.response.write("<H1>");
            if (package != null)
                req.response.write(package.getFullName());
            req.response.write("</H1>");
            req.response.write("<H1>");
            if (package != null)
                req.response.write(package.Summary);
            req.response.write("</H1>");
            req.response.write("<H2>");
            if (package != null)
                req.response.write(package.Description);
            req.response.write("</H2>");
            req.response.write("<img src=\"cerv.png\" align=\"center\">");
            req.response.write("</html>");
        }
    }
}
