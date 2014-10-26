using System;
using System.Xml.Linq;
using System.Collections.Generic;

namespace Mascaret
{

    public enum ACLPerformative { ACCEPT_PROPOSAL, AGREE, CANCEL, CFP, CONFIRM, DISCONFIRM, FAILURE, INFORM, INFORM_IF, INFORM_REF, NOT_UNDERSTOOD, PROPAGATE, PROPOSE, PROXY, QUERY_IF, QUERY_REF, REFUSE, REJECT_PROPOSAL, REQUEST, REQUEST_WHEN, REQUEST_WHENEVER, SUBSCRIBE, UNKNOWN };

    public class ACLMessage
    {
        private ACLPerformative performative;
        public ACLPerformative Performative
        {
            get { return performative; }
            set { performative = value; }
        }

        private List<AID> receivers = new List<AID>();
        public List<AID> Receivers
        {
            get { return receivers; }
            set { receivers = value; }
        }

        private List<AID> replyTo = new List<AID>();
        public List<AID> ReplyTo
        {
            get { return replyTo; }
            set { replyTo = value; }
        }

        private string content;
        public string Content
        {
            get { return content; }
            set { content = value; }
        }

        private string conversationID;
        public string ConversationID
        {
            get { return conversationID; }
            set { conversationID = value; }
        }

        private string language;
        public string Language
        {
            get { return language; }
            set { language = value; }
        }

        private string onthology;
        public string Onthology
        {
            get { return onthology; }
            set { onthology = value; }
        }

        private string protocol;
        public string Protocol
        {
            get { return protocol; }
            set { protocol = value; }
        }

        private string replyBy;
        public string ReplyBy
        {
            get { return replyBy; }
            set { replyBy = value; }
        }

        private AID sender;
        public AID Sender
        {
            get { return sender; }
            set { sender = value; }
        }

        private XElement xmlContent;
        public XElement XmlContent
        {
            get { return xmlContent; }
            set { xmlContent = value; }
        }

        private bool hasXMLContent;
        public bool HasXMLContent
        {
            get { return hasXMLContent; }
            set { hasXMLContent = value; }
        }

        private TimeExpression timestamp;
        public TimeExpression Timestamp
        {
            get { return timestamp; }
            set { timestamp = value; }
        }

        public ACLMessage(ACLPerformative performative)
        {
            this.performative = performative;
        }

        public string getPerformativeText()
        {
            string res;

            switch (performative)
            {
                case ACLPerformative.ACCEPT_PROPOSAL: res = "ACCEPT_PROPOSAL";
                    break;
                case ACLPerformative.AGREE: res = "AGREE";
                    break;
                case ACLPerformative.CANCEL: res = "CANCEL";
                    break;
                case ACLPerformative.CFP: res = "CFP";
                    break;
                case ACLPerformative.CONFIRM: res = "CONFIRM";
                    break;
                case ACLPerformative.DISCONFIRM: res = "DISCONFIRM";
                    break;
                case ACLPerformative.FAILURE: res = "FAILURE";
                    break;
                case ACLPerformative.INFORM: res = "INFORM";
                    break;
                case ACLPerformative.INFORM_IF: res = "INFORM_IF";
                    break;
                case ACLPerformative.INFORM_REF: res = "INFORM_REF";
                    break;
                case ACLPerformative.NOT_UNDERSTOOD: res = "NOT_UNDERSTOOD";
                    break;
                case ACLPerformative.PROPAGATE: res = "PROPAGATE";
                    break;
                case ACLPerformative.PROPOSE: res = "PROPOSE";
                    break;
                case ACLPerformative.PROXY: res = "PROXY";
                    break;
                case ACLPerformative.QUERY_IF: res = "QUERY_IF";
                    break;
                case ACLPerformative.QUERY_REF: res = "QUERY_REF";
                    break;
                case ACLPerformative.REFUSE: res = "REFUSE";
                    break;
                case ACLPerformative.REJECT_PROPOSAL: res = "REJECT_PROPOSAL";
                    break;
                case ACLPerformative.REQUEST: res = "REQUEST";
                    break;
                case ACLPerformative.REQUEST_WHEN: res = "REQUEST_WHEN";
                    break;
                case ACLPerformative.REQUEST_WHENEVER: res = "REQUEST_WHENEVER";
                    break;
                case ACLPerformative.SUBSCRIBE: res = "SUBSCRIBE";
                    break;
                case ACLPerformative.UNKNOWN: res = "UNKNOWN";
                    break;
                default: res = "UNKNOWN";
                    break;
            }
            return res;

        }

        public void setXMLContent(XElement contentNode)
        {
            hasXMLContent = true;
            xmlContent = contentNode;
        }

        //not implented right now
        public string toXML()
        {
            return "";
        }



    }

}
