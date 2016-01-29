using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;


namespace Mascaret
{
    public class BehaviorPlanner
    {
        public static string BMLHEADER = "<bml xmlns=\"http://www.bml-initiative.org/bml/bml-1.0\" xmlns:ext=\"http://www.bml-initiative.org/bml/coreextensions-1.0\"";

        public Agent agent;

        public BehaviorPlanner(Agent agent)
        {
            this.agent = agent;
        }

        public virtual List<string> parseIntention(string fml)
        {
            // Do some interesting thing like 
            // * Send a message
            // * Start an action/behavior
            // * Modify internal (KB, Emotion,...)


            List<string> bmlList = new List<string>();
            return bmlList;

            /*
            string bml = BMLHEADER;
            string id = "id=\"bml1\"";
            string characterId = "characterId=\"" + agent.name + "\"";
            string composition = "composition=\"APPEND\"";

            bml += " " + id + " " + characterId + " " + composition + ">";


            XmlReader reader = XmlReader.Create(new StringReader(fml));
            reader.Read();
            string performative = "";
            string content = "";
            string receiver = "";
            string emotion = "";
            string ressource = "";
            if (reader.Name == "FML")
            {
                reader.Read();
                if (reader.Name == "Performative")
                {
                    performative = reader.ReadInnerXml();

                    reader.Read();
                    reader.Read();
                    receiver = reader.ReadInnerXml();

                    reader.Read();
                    reader.Read();
                    content = reader.ReadInnerXml();

                    reader.Read();
                    emotion = reader.ReadInnerXml();

                    reader.Read();
                    reader.Read();
                    ressource = reader.ReadInnerXml();
                    reader.Read();

                }

            }

            if (performative.ToLower() == "inform")
            {
                bml += "<faceLexeme id=\"f1\" lexeme=\"RAISE_BROWS\" amount=\"0.8\" start=\"0\" end=\"2\"/>";
                if (ressource != "")
                {
                    bml += "<gaze id=\"g1\" start=\"1\" end=\"2\" target=\"" + ressource + "\"/>";
                    bml += "<pointing id=\"p1\" target=\"" + ressource + "\" mode=\"RIGHT_HAND\" start=\"1\" end=\"4\"/>";
                }

                bml += "<speech id=\"s1\" start=\"0\">" +
                        "<text>" + content + "</text>" +
                        "<description  type=\"application/ssml+xml\">" +
                            "<speak xmlns=\"http://www.w3.org/2001/10/synthesis\">" +
                                content +
                            "</speak>" +
                        "</description>" +
                        "</speech>";
            }


            bml += "</bml>";
            bmlList.Add(bml);

            //bilal 19-10-15
            //foreach (string bmlI in bmlList)
                //this.behaviorRealizer.addBehavior(bmlI);
            //bilal 19-10-15
             * */
            
         
        }

    }
}
