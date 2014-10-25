using System.Collections;
using System.Net.Sockets;
using System.Net;

public abstract class TcpConnectionFactory 
{
	/* WINPHONE */
	public abstract TcpConnection create(Socket socket);
	
}
