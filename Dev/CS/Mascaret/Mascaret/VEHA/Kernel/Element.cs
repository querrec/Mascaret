using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class Element
    {
        public string name;

        private string description;
        public string Description
        {
            get { return description; }
            set { description = value; }
        }


        private string stereotype = "";
        public string Stereotype
        {
            get { return stereotype; }
            set { stereotype = value; }
        }

        private string summary;
        public string Summary
        {
            get { return summary; }
            set { summary = value; }
        }


        private string id;
        public string Id
        {
            get { return id; }
            set { id = value; }
        }


        private Dictionary<String, String> tags;
        public Dictionary<String, String> Tags
        {
            get { return tags; }
            set { tags = value; }
        }

        public void addTag(string tag, string tagValue)
        {
            tags.Add(tag, tagValue);
        }

        public void setTag(string tag, string tagValue)
        {
            tags.Add(tag, tagValue);
        }

        public string getTag(string name)
        {
            if (tags.ContainsKey(name))
                return tags[name];
            else
                return "";
        }

        public Element(string name)
        {
            this.name = name;
            tags = new Dictionary<string, string>();
        }

        public virtual string getFullName()
        {
            return name;
        }

    }
}

