using System;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Atn;
using Antlr4.Runtime.Misc;
using DFA = Antlr4.Runtime.Dfa.DFA;


namespace Mascaret
{
    public class Expression : ValueSpecification
    {

        private bool needUpdate;
        public bool NeedUpdate
        {
            get { return needUpdate; }
            set { needUpdate = value; }
        }


        private string expressionValue;
        public string ExpressionValue
        {
            get { return expressionValue; }
            set { expressionValue = value; }
        }

        //callback

        public Expression(string expression, Classifier returnType)
            : base(returnType)
        {
            expressionValue = expression;
        }

        public ValueSpecification evaluateExpression(InstanceSpecification owner)
        {
            return null;
        }

        public ValueSpecification evaluateExpression(Dictionary<string, ValueSpecification> c)
        {

            MascaretApplication.Instance.VRComponentFactory.Log("Evaluate Expression");

            foreach(KeyValuePair<string,ValueSpecification> val in c)
            {
                string valueS = "NotDef";
                ValueSpecification value = val.Value;
                if (value.GetType().ToString() == "Mascaret.InstanceValue")
                    valueS = ((InstanceValue)value).SpecValue.getFullName();
                else
                    MascaretApplication.Instance.VRComponentFactory.Log(value.GetType().ToString());
                    
                MascaretApplication.Instance.VRComponentFactory.Log("Context : " + val.Key + " = " + valueS);
            }

            OCLExpressionLexer lex = new OCLExpressionLexer(new AntlrInputStream(expressionValue));
            CommonTokenStream tokens = new CommonTokenStream(lex);
            OCLExpressionParser parser = new OCLExpressionParser(tokens);
            parser.context = c;
            parser.expression();
            MascaretApplication.Instance.VRComponentFactory.Log("Parsing  : " + expressionValue);
            MascaretApplication.Instance.VRComponentFactory.Log("Nb Erreur : " + parser.NumberOfSyntaxErrors);
            MascaretApplication.Instance.VRComponentFactory.Log("Result : " + parser.value);

            
            return (LiteralBoolean)(parser.value);
        }

        public override ValueSpecification clone()
        {
            return null;
        }

    }
}