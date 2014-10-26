using System.Collections;
using System.Net.Sockets;
using System.Net;

namespace Mascaret
{
    public class HttpConnection : TcpConnection
    {

        private HttpServer server;
        /*
        public HttpConnection(HttpServer server) : base()
        {
            this.server = server;
        }
        */

        /* WINPHONE */
        public HttpConnection(Socket socket, HttpServer server)
            : base(socket)
        {
            this.server = server;
        }


        public override void start()
        {
            HttpRequest request = null;
            /* WINPHONE */
            request = HttpRequest.createRequestFromSocket(socket);
            if (server != null)
            {
                server.manageRequest(request);
            }
        }

    }
}
