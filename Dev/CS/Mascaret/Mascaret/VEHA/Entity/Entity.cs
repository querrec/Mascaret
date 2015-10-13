using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class Entity : InstanceSpecification
    {

        private ShapeSpecification activeShape = null;
        public ShapeSpecification ActiveShape
        {
            get { return activeShape; }
            set { activeShape = value; }
        }


        private PointSpecification referentialPoint = null;
        public PointSpecification ReferentialPoint
        {
            get { return referentialPoint; }
            //set{referentialPoint=value;}
        }

        protected void setReferentialPoint(PointSpecification point)
        {
            referentialPoint = point;
        }


        private Dictionary<String, PointSpecification> viewPoints;
        public Dictionary<String, PointSpecification> ViewPoints
        {
            get { return viewPoints; }
            set { viewPoints = value; }
        }


        private Entity parent = null;
        public Entity Parent
        {
            get { return parent; }
            set { parent = value; }
        }


        private List<Entity> children = new List<Entity>();
        public List<Entity> Children
        {
            get { return children; }
            set { children = value; }
        }

        //missing callback

        public Entity(string name, EntityClass classe)
            : base(name, classe)
        {
            viewPoints = new Dictionary<string, PointSpecification>();
            //	referentialPoint= new  (UnityPointSpecification)ScriptableObject.CreateInstance("UnityPointSpecification");
            //	((UnityPointSpecification)referentialPoint).instantiate();//getGlobalPosition(),getGlobalRotation());

        }
        public Entity()
            : base("", null)
        {
            viewPoints = new Dictionary<string, PointSpecification>();
            //	referentialPoint= new  (UnityPointSpecification)ScriptableObject.CreateInstance("UnityPointSpecification");
            //	((UnityPointSpecification)referentialPoint).instantiate();//getGlobalPosition(),getGlobalRotation());

        }

        //Missing callback operations

        public void addChild(Entity element)
        {
            if (!children.Contains(element))
            {
                element.parent = this;
                children.Add(element);
            }
        }

        public bool hasChild(Entity element)
        {
            foreach (Entity child in children)
            {
                if (child == element)
                    return true;
            }

            foreach (Entity child in children)
            {
                return child.hasChild(element);
            }

            return false;
        }

        public Entity getChild(string name)
        {
            foreach (Entity child in children)
            {
                if (child.name.CompareTo(name) == 0)
                    return child;
            }
            return null;
        }

        public void setActiveShape(string name)
        {
            if (Classifier.hasAttribute(name))
            {
                Slot s = getProperty(name);
                if (s != null)
                {
                    activeShape = (ShapeSpecification)s.getValue();
                    if (activeShape != null)
                        activeShape.setEntity(this);
                    else
                        System.Console.WriteLine(this.name + ".setActiveShape(" + name + ") : the property is undefined");

                }
            }
        }

        public void setVisibility(bool v)
        {
            activeShape.setVisibility(v);
        }

        public bool getVisibility()
        {
            return activeShape.getVisibility();
        }

        public void translate(Vector3 pos)
        {
            //callback operation
            referentialPoint.translate(pos);
        }

        public void setLocalPosition(Vector3 pos)
        {
            //callback operation
            if (referentialPoint != null)
                referentialPoint.setLocalPosition(pos);
        }

        public Vector3 getLocalPosition()
        {
            return referentialPoint.getLocalPosition();
        }

        public void setGlobalPosition(Vector3 pos)
        {
            //callback operation
            referentialPoint.setGlobalPosition(pos);
        }

        public Vector3 getGlobalPosition()
        {
            return referentialPoint.getGlobalPosition();
        }

        public void setLocalRotation(RotationVector orientation)
        {
            //callback operation
          //  referentialPoint.setLocalRotation(orientation);
        }

        public RotationVector getLocalRotation()
        {
            return referentialPoint.getLocalRotation();
        }

        public void setGlobalRotation(RotationVector orientation)
        {
            //callback operation
            referentialPoint.setGlobalRotation(orientation);
        }

        public RotationVector getGlobalRotation()
        {
            return referentialPoint.getGlobalRotation();
        }

        public void setLocalOrientation(Vector3 pos)
        {
            //callback operation
            if (referentialPoint != null)
                referentialPoint.setLocalOrientation(pos);
        }

        public Vector3 getLocalOrientation()
        {
            //return referentialPoint.getLocalOrientation();
            return null;
        }

        public void setGlobalOrientation(Vector3 pos)
        {
            //callback operation
            referentialPoint.setGlobalOrientation(pos);
        }

        public Vector3 getGlobalOrientation()
        {
            return referentialPoint.getGlobalOrientation();
        }

        public List<ShapeSpecification> getShapes()
        {
            List<ShapeSpecification> vect = new List<ShapeSpecification>();
            foreach (Slot slot in Slots.Values)
            {
                Dictionary<string, ValueSpecification> values = slot.Values;
                if (values.Count == 1) //etrange ca aussi comme condition
                {
                    foreach (ValueSpecification valueSpec in values.Values)
                    {
                        if (valueSpec as ShapeSpecification != null)
                            vect.Add((ShapeSpecification)valueSpec);
                    }
                }
            }
            return vect;

        }

        public ShapeSpecification getShape(string name)
        {
            if (Classifier.hasAttribute(name))
            {
                Slot prop = getProperty(name);
                if (prop != null)
                    return (ShapeSpecification)prop.getValue();
            }

            return null;
        }

        public List<AnimationSpecification> getAnimations()
        {
            List<AnimationSpecification> vect = new List<AnimationSpecification>();
            foreach (Slot slot in Slots.Values)
            {
                Dictionary<string, ValueSpecification> values = slot.Values;
                if (values.Count == 1) //etrange ca aussi comme condition
                {
                    foreach (ValueSpecification valueSpec in values.Values)
                    {
                        if (valueSpec as AnimationSpecification != null)
                            vect.Add((AnimationSpecification)valueSpec);
                    }
                }
            }
            return vect;
        }

        public AnimationSpecification getAnimation(string name)
        {
            if (Classifier.hasAttribute(name))
            {
                Slot prop = getProperty(name);
                if (prop != null)
                    return (AnimationSpecification)prop.getValue();
            }
            return null;
        }

        public List<SoundSpecification> getSounds()
        {
            List<SoundSpecification> vect = new List<SoundSpecification>();
            foreach (Slot slot in Slots.Values)
            {
                Dictionary<string, ValueSpecification> values = slot.Values;
                if (values.Count == 1) //etrange ca aussi comme condition
                {
                    foreach (ValueSpecification valueSpec in values.Values)
                    {
                        if (valueSpec as SoundSpecification != null)
                            vect.Add((SoundSpecification)valueSpec);
                    }
                }
            }
            return vect;
        }

        public SoundSpecification getSound(string name)
        {
            if (Classifier.hasAttribute(name))
            {
                Slot prop = getProperty(name);
                if (prop != null)
                    return (SoundSpecification)prop.getValue();
            }
            return null;
        }

        public List<TopologicalSpecification> getTopologicals()
        {
            List<TopologicalSpecification> vect = new List<TopologicalSpecification>();
            foreach (Slot slot in Slots.Values)
            {
                Dictionary<string, ValueSpecification> values = slot.Values;
                if (values.Count == 1) //etrange ca aussi comme condition
                {
                    foreach (ValueSpecification valueSpec in values.Values)
                    {
                        if (valueSpec as TopologicalSpecification != null)
                            vect.Add((TopologicalSpecification)valueSpec);
                    }
                }
            }
            return vect;
        }

        public TopologicalSpecification getTopological(string name)
        {
            if (Classifier.hasAttribute(name))
            {
                Slot prop = getProperty(name);
                if (prop != null)
                    return (TopologicalSpecification)prop.getValue();
            }
            return null;
        }

        public List<PointSpecification> getPoints()
        {
            List<PointSpecification> vect = new List<PointSpecification>();
            foreach (Slot slot in Slots.Values)
            {
                Dictionary<string, ValueSpecification> values = slot.Values;
                if (values.Count == 1) //etrange ca aussi comme condition
                {
                    foreach (ValueSpecification valueSpec in values.Values)
                    {
                        if (valueSpec as PointSpecification != null)
                            vect.Add((PointSpecification)valueSpec);
                    }
                }
            }
            return vect;
        }

        public PointSpecification getPoint(string name)
        {
            if (Classifier.hasAttribute(name))
            {
                Slot prop = getProperty(name);
                if (prop != null)
                    return (PointSpecification)prop.getValue();
            }
            return null;
        }

        public List<PathSpecification> getPaths()
        {
            List<PathSpecification> vect = new List<PathSpecification>();
            foreach (Slot slot in Slots.Values)
            {
                Dictionary<string, ValueSpecification> values = slot.Values;
                if (values.Count == 1) //etrange ca aussi comme condition
                {
                    foreach (ValueSpecification valueSpec in values.Values)
                    {
                        if (valueSpec as PathSpecification != null)
                            vect.Add((PathSpecification)valueSpec);
                    }
                }
            }
            return vect;
        }

        public PathSpecification getPath(string name)
        {
            if (Classifier.hasAttribute(name))
            {
                Slot prop = getProperty(name);
                if (prop != null)
                    return (PathSpecification)prop.getValue();
            }
            return null;
        }

        //Default parameters sens = 1, animationSpeed = 1.0, cycle = true
        public bool playAnimation(string animationName, int sens, double animationSpeed, bool cycle)
        {
            AnimationSpecification anim = getAnimation(animationName);
            if (anim != null)
            {
                anim.setShape(activeShape);
                return anim.play(animationSpeed, sens, cycle);
            }
            else
                return false;
        }

        public bool playSound(string name, double speed, bool cycle)
        {
            SoundSpecification sound = getSound(name);
            if (sound != null)
            {
                //	sound.attachTo(referentialPoint);
                return sound.play(speed, cycle);
            }
            else
                return false;
        }

        public bool stopAnimation(string animationName)
        {
            AnimationSpecification anim = getAnimation(animationName);
            if (anim != null)
            {
                return anim.stop();
            }
            else
                return false;
        }

        public bool stopAllAnimations()
        {
            foreach (AnimationSpecification anim in getAnimations())
            {
                anim.stop();
            }
            return true;
        }

        public bool stopSound(string name)
        {
            SoundSpecification sound = getSound(name);
            if (sound != null)
            {
                return sound.stop();
            }
            else
                return false;
        }

        public void _addChildren(Entity valueEntity)
        {

            if (children.Contains(valueEntity))
                children.Add(valueEntity);
        }
        public void _removeChildren(Entity valueEntity)
        {
            if (children.Contains(valueEntity))
                children.Remove(valueEntity);

        }

        public PointSpecification getViewPoint(string name)
        {
            System.Console.WriteLine(this.name + " has " + viewPoints.Keys.Count + " viewpoints");

            if (viewPoints.ContainsKey(name))
                return viewPoints[name];

            foreach (Entity child in children)
            {
                PointSpecification vp = child.getViewPoint(name);
                if (vp != null)
                {
                    return vp;
                }
            }
            return null;
        }

        public void setParent(Entity e)
        {
            if (parent != null && parent.children.Contains(e))
            {
                parent.children.Remove(this);

            }
            //referentialPoint.Parent = null;
            parent = e;
            if (parent != null)
            {
                //	referentialPoint.Parent = parent.referentialPoint;

                if (parent.children.Contains(this))
                    parent.children.Add(this);
            }
        }

        public Entity Clone()
        {
            MascaretApplication.Instance.VRComponentFactory.Log(" ######## Cloning : " + this.name);

            Entity clone = new Entity(this.name, (EntityClass)(this.Classifier));
            
            /*foreach (Slot slot in this.Slots.Values)
            {

                //clone.Slots.Add(slot.getValue().getStringFromValue(), slot);

                //Slot s = new Slot();

                foreach (KeyValuePair<string, ValueSpecification> pair in slot.Values)
                {
                    //clone.Slots.Add(pair.Value.getStringFromValue(), slot);
                    ValueSpecification val = pair.Value.clone();
                    //s.addValue(val);
                }
                clone.Slots.Add(slot.getValue().getStringFromValue(), s);
            }*/

            foreach (KeyValuePair<string,Slot> slot in this.Slots)
            {
                
                MascaretApplication.Instance.VRComponentFactory.Log(" ######## Add Value for : " + slot.Value.DefiningProperty.name);
               
                //clone.addSlot(slot.Value.DefiningProperty);
                Slot s = clone.getProperty(slot.Value.DefiningProperty.name);
                
                foreach (KeyValuePair<string, ValueSpecification> pair in slot.Value.Values)
                {
                    ValueSpecification val = pair.Value.clone();
                    s.addValue(val);
                }
                
            }

            return clone;
        }

        
    }
}

