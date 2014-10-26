using System.Collections;


using System.Net.Sockets;
using System.Net;

namespace Mascaret
{
    public class TcpServer
    {
        private int port = 8080;

        /* WINPHONE */
        private IPAddress ipAd;
        private Socket listenSocket;
        private TcpListener tcp_Listener = null;


        protected TcpConnectionFactory connectionFactory;

        public TcpServer(int port, TcpConnectionFactory connectionFactory)
        {
            this.port = port;
            this.connectionFactory = connectionFactory;
        }

        public void Start()
        {
            /* WINPHONE */
            tcp_Listener = new TcpListener(IPAddress.Parse("127.0.0.1"), port);
            tcp_Listener.Start();

        }

        public void Update()
        {
            /* WINPHONE */
            if (tcp_Listener.Pending())
            {
                System.Console.WriteLine("Nouvelle Connection");
                Socket ss = tcp_Listener.AcceptSocket();
                TcpConnection tcpConnection = createConnection(ss);
                handleAccept(tcpConnection);
                //Debug.Log (" Fin Handle Connection ...");
            }
        }

        public void handleAccept(TcpConnection connection)
        {
            connection.start();
        }

        /* WINPHONE */
        public virtual TcpConnection createConnection(Socket ss)
        {
            return connectionFactory.create(ss);
        }


    }
}
