using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Net.Sockets;
using System.Net;

namespace Mascaret
{
    public class HttpRequest
    {

        public string method;
        public string requestURI;
        public Dictionary<string, string> parameters;

        /* WINPHONE */
        public Socket socket;


        public HttpResponse response;

        public HttpRequest()
        {
            response = new HttpResponse();
            parameters = new Dictionary<string, string>();
        }

        /* WINPHONE */
        public static HttpRequest createRequestFromSocket(Socket socket)
        {
            StreamReader sr = new StreamReader(new NetworkStream(socket));

            string header = readHeader(sr);

            HttpRequest req = createRequestFromHeader(header);
            req.readParameters();

            req.response.socket = socket;
            req.socket = socket;

            //Debug.Log(header);

            return req;
        }


        private static string readHeader(StreamReader sr)
        {
            string header = "";

            while (true)
            {
                string line = sr.ReadLine();
                if (line == "") break;
                else header += line + '\n';
            }

            return header;

        }

        public static HttpRequest createRequestFromHeader(string request)
        {
            HttpRequest req = new HttpRequest();
            string[] mots = request.Split(' ');
            req.method = mots[0];
            req.requestURI = mots[1];

            return req;
        }

        public void readParameters()
        {
            string urlEncodedParams = "";
            if (method == "GET")
            {
                int pos = requestURI.IndexOf("?");
                if (pos != -1)
                {
                    urlEncodedParams = requestURI.Substring(pos + 1);
                }
            }

            if (urlEncodedParams != "")
            {
                string[] ps = urlEncodedParams.Split('&');

                for (int i = 0; i < ps.Length; i++)
                {
                    string param = ps[i];
                    int pos = param.IndexOf('=');
                    if (pos != -1)
                    {
                        parameters.Add(param.Substring(0, pos), param.Substring(pos + 1));
                    }
                }
            }
        }

    }
}
