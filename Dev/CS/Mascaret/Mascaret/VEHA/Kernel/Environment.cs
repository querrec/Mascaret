using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class Environment
    {

        private Zone mainZone;
        public Zone MainZone
        {
            get { return mainZone; }
            set { mainZone = value; }
        }


        private Model model = null;
        public Model Model
        {
            get { return model; }
            set { model = value; }
        }


        private Dictionary<String, InstanceSpecification> instances;
        public Dictionary<String, InstanceSpecification> InstanceSpecifications
        {
            get { return instances; }
            set { instances = value; }
        }

        public void addInstance(InstanceSpecification spec)
        {
            instances.Add(spec.name/*.ToLower()*/, spec);
        }

        public void removeInstance(InstanceSpecification spec)
        {
            instances.Remove(spec.name);
        }

        public InstanceSpecification getInstance(string name)
        {
            if (instances.ContainsKey(name/*.ToLower()*/))
                return instances[name/*.ToLower()*/];
            else
                return null;
        }


        protected string url;
        public string Url
        {
            get { return url; }
            set { url = value; }
        }

        public List<Entity> getEntities()
        {
            List<Entity> entities = new List<Entity>();
            foreach (InstanceSpecification curEntity in instances.Values)
            {
                if (curEntity is Entity)
                    entities.Add(curEntity as Entity);
            }
            return entities;
        }

        public Entity getEntity(string name)
        {
            if (instances.ContainsKey(name))
                return ((Entity)instances[name]);
            else
                return null;
        }

        public List<VirtualHuman> getVirtualHumans()
        {
            List<VirtualHuman> entities = new List<VirtualHuman>();
            foreach (InstanceSpecification curVirtualHuman in instances.Values)
            {
                if (curVirtualHuman is VirtualHuman)
                    entities.Add(curVirtualHuman as VirtualHuman);
            }
            return entities;
        }

        public VirtualHuman getVirtualHuman(string name)
        {
            if (instances.ContainsKey(name))
                return ((VirtualHuman)instances[name]);
            else
                return null;
        }

        public void renameEntities(List<Entity> entities, List<string> newName)
        {
            for (int i = 0; i < entities.Count; i++)
            {
                instances.Remove(entities[i].name);
                entities[i].rename(newName[i]);
                addInstance(entities[i]);
            }

            foreach (InstanceSpecification instance in instances.Values)
            {
                instance.checkSlots();
            }
        }


        public Environment(Model model)
        {
            instances = new Dictionary<string, InstanceSpecification>();
            this.model = model;
        }

        public void print()
        {
            //for (int i = 0; i < this.InstanceSpecifications.Keys.Count; i++)
            //	InstanceSpecifications.Values[i].print();
        }

    }
}

