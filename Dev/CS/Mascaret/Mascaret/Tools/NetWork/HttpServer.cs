using System.Collections;
using System.Collections.Generic;
/* WINPHONE */
using System.Net.Sockets;
using System.Net;

namespace Mascaret
{
    public class HttpServer : TcpServer
    {
        public HttpServer(int port)
            : base(port, new HttpConnectionFactory())
        {
            servlets = new Dictionary<string, HttpServlet>();
        }

        /* WINPHONE */
        public override TcpConnection createConnection(Socket ss)
        {
            return ((HttpConnectionFactory)connectionFactory).create(ss, this);
        }


        public void manageRequest(HttpRequest request)
        {
            /* WINPHONE */
            string url = request.requestURI;
            int pos = url.IndexOf('?');
            if (pos != -1)
            {
                url = url.Substring(0, pos);
            }

            //Debug.Log("Looking for url : " + url);
            HttpServlet servlet = getServlet(url);
            if (servlet == null)
            {
                request.response.status = 404;
                request.response.write("<html><head><title>Error 404</title></head><body><h1>Error 404</h1><p>" + request.requestURI + " not found</p></body></html>");
                request.response.flushBuffer();
            }
            else
            {
                request.response.status = 200;
                servlet.manageRequest(request);
                request.response.flushBuffer();
            }
        }

        public bool addServlet(string url, HttpServlet servlet)
        {
            servlets.Add(url, servlet);
            return true;
        }

        private HttpServlet getServlet(string url)
        {
            if (servlets.ContainsKey(url))
                return servlets[url];
            return null;
        }

        private Dictionary<string, HttpServlet> servlets;

    }
}
