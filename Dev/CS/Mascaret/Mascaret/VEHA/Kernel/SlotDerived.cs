using System;

using System.Collections.Generic;


namespace Mascaret
{
    public class SlotDerived : Slot
    {

        private Expression expression;
        public Expression Expression
        {
            get { return expression; }
            set { expression = value; }
        }

        public SlotDerived(InstanceSpecification instance, Property property)
            : base(instance, property)
        {
            if (DefiningProperty != null)
            {
                System.Console.WriteLine("Slot deriver : " + ((Expression)DefiningProperty.DefaultValue).ExpressionValue + " from" + instance.name);
                expression = (Expression)DefiningProperty.DefaultValue.clone();
                if (expression == null)
                {
                    expression = new Expression("No expression", DefiningProperty.Type);
                }
            }
        }

        public new bool _canRemoveValue()
        {
            return false;
        }

        public new void addValue(ValueSpecification val)
        {
            System.Console.WriteLine("Can't add values in a derived slot");
        }

        public void notifyMaybeChanged()
        {
            //cbOnChange()
        }

        protected new void _update()
        {
            if (expression.NeedUpdate)
            {
                Values.Clear();
                //List<ValueSpecification> values = expression.checkAndGetValues(expression.evaluateExpression(OwningInstance));
                foreach (ValueSpecification valueSpec in Values.Values)
                {
                    _addValue(valueSpec);
                }
            }
        }

    }
}


