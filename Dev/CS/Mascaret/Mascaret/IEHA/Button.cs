using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class Button
    {

        public bool pressed;
        public string name;

        public Button(string name)
        {
            this.name = name;
            pressed = false;
        }

        public delegate void OnButtonPressedDelegate(Button be, bool pressed);
        private List<OnButtonPressedDelegate> cbButton = new List<OnButtonPressedDelegate>();
        public void addCallbackOnButtonPressed(OnButtonPressedDelegate fct)
        {
            cbButton.Add(fct);
        }

        public void setPressed(bool pressed)
        {
            if (this.pressed != pressed)
            {
                this.pressed = pressed;
                foreach (OnButtonPressedDelegate fct in cbButton)
                    fct(this, pressed);
            }
        }


    }
}
