using System.Collections;

namespace Mascaret
{
    public abstract class HttpServlet
    {
        public abstract void manageRequest(HttpRequest request);
    }
}

