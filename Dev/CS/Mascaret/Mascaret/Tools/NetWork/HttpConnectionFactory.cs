using System.Collections;
/* WINPHONE */
using System.Net.Sockets;
using System.Net;

namespace Mascaret
{
    public class HttpConnectionFactory : TcpConnectionFactory
    {
        /* WINPHONE */
        public override TcpConnection create(Socket socket)
        {

            return new HttpConnection(socket, null);
        }

        public TcpConnection create(Socket socket, HttpServer server)
        {
            return new HttpConnection(socket, server);
        }

    }
}

