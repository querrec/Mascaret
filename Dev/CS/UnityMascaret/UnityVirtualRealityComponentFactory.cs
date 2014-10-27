using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using Mascaret;


public class UnityVirtualRealityComponentFactory : VirtualRealityComponentFactory {

	public override ShapeSpecification createShape(string name, string url, bool movable=true,bool recursive =false,string shader="")
	{
		ShapeSpecification shape = new UnityShapeSpecification(name, url, movable, recursive, shader);
		return shape;
	}

	public override BehaviorExecution InstanciateOpaqueBehavior(Behavior behavior, string typeName, InstanceSpecification host, Dictionary<string, ValueSpecification> p)
	{
		Type type = Types.GetType( typeName,"Assembly-CSharp" );
		BehaviorExecution be = null;
		if (type != null)
			be =  (BehaviorExecution)(Activator.CreateInstance(type));
		else Debug.Log ("ERREUR : " + typeName + " not found");
		
		if (be != null)
		{
			be.init(behavior,host,p,false);
		}
		return be;
	}

}
