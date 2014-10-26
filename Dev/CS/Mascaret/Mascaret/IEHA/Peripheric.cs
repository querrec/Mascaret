using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    public class Peripheric
    {
        protected List<Button> _buttons = new List<Button>();
        public string name;

        public Peripheric(string name)
        {
            this.name = name;
        }

        public void addButton(Button button)
        {
            _buttons.Add(button);
        }

        public void removeButton(Button button)
        {
            _buttons.Remove(button);
        }

        public void removeButton(string buttonName)
        {
            foreach (Button b in _buttons)
                if (b.name == buttonName) _buttons.Remove(b);
        }

        public Button getButton(string name)
        {
            foreach (Button b in _buttons)
                if (b.name == name) return b;

            return null;
        }

        public List<Button> getButtons()
        {
            return _buttons;
        }

    }
}