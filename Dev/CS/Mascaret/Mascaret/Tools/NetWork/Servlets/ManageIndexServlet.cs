using System.Collections;

namespace Mascaret
{
    public class ManageIndexServlet : HttpServlet
    {

        public override void manageRequest(HttpRequest req)
        {
            System.Console.WriteLine("Manage Index");
            req.response.write("<html>");
            req.response.write("<head>");
            req.response.write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
            req.response.write("<title>");
            req.response.write("MASCARET : Multi Agents System for Collaborative Adaptive Environment for Traning");
            req.response.write("</title>");
            req.response.write("</head>");
            req.response.write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
            req.response.write("<frameset rows=\"175,*\">");
            req.response.write("<frame src=\"Head\" name=\"Head\">");
            req.response.write("<frameset cols=\"250,*\">");
            req.response.write("<frame src=\"Model\" name=\"Model\">");
            req.response.write("<frame src=\"Body\" name=\"Body\">");
            req.response.write("</frameset>");
            req.response.write("</frameset>");
            req.response.write("</html>");
        }

    }
}
