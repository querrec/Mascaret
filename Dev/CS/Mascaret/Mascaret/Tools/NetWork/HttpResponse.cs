using System.Collections;
using System.Net.Sockets;
using System.Net;
using System.IO;

namespace Mascaret
{
    public class HttpResponse
    {

        public Socket socket;
        public int status;

        private string buffer;

        public void write(string content)
        {
            buffer += content;
        }

        public void flushBuffer()
        {
            /*WINPHONE*/
            string message = getFormattedStringResponse();
            System.Console.WriteLine(message);

            StreamWriter sw = new StreamWriter(new NetworkStream(socket));
            sw.Write(message);
            sw.Flush();
            socket.Close();
            sw.Close();
        }

        private string getFormattedStringResponse()
        {
            string ostream = "";
            ostream += "HTTP/1.1 " + status + " " + getMessageFromStatus() + "\r\n";

            ostream += "\r\n";
            ostream += buffer;

            return ostream;
        }

        string getMessageFromStatus()
        {
            switch (status)
            {
                case 200:
                    return "OK";
                case 201:
                    return "CREATED";
                case 202:
                    return "CREATED";
                case 203:
                    return "PARTIAL INFORMATION";
                case 204:
                    return "NO RESPONSE";
                case 205:
                    return "RESET CONTENT";
                case 206:
                    return "PARTIAL CONTENT";
                case 301:
                    return "MOVED";
                case 302:
                    return "FOUND";
                case 303:
                    return "METHOD";
                case 304:
                    return "NOT MODIFIED";
                case 400:
                    return "BAD REQUEST";
                case 401:
                    return "UNAUTHORIZED";
                case 402:
                    return "PAYMENT REQUIRED";
                case 403:
                    return "FORBIDDEN";
                case 404:
                    return "NOT FOUND";
                case 500:
                    return "INTERNAL ERROR";
                case 501:
                    return "NOT IMPLEMENTED";
                case 502:
                    return "BAD GATEWAY";
                case 503:
                    return "SERVICE UNAVAILABLE";
                case 504:
                    return "GATEWAY TIMEOUT";
            }
            return "UNKNOW";
        }

    }
}
