using System.Collections;
/* WINPHONE */
using System.Net.Sockets;
using System.Net;

namespace Mascaret
{
    public abstract class TcpConnection
    {
        /* WINPHONE */
        protected Socket socket;


        /* WINPHONE */
        public TcpConnection(Socket socket)
        {
            this.socket = socket;
        }


        public abstract void start();
    }
}
