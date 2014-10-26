using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class OpaqueExpression : ValueSpecification
    {

        private string body;
        public string Body
        {
            get { return body; }
            set { body = value; }
        }


        private string language;
        public string Language
        {
            get { return language; }
            set { language = value; }
        }

        public OpaqueExpression()
            : base(null)
        {
        }

        public override ValueSpecification clone()
        {
            OpaqueExpression oe = new OpaqueExpression();
            oe.Language = language;
            oe.Body = body;
            return oe;
        }
    }
}
