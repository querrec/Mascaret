using System;
using System.Collections.Generic;


namespace Mascaret
{
public class Procedure:Element
{
	 
	private List <Ressource> ressources=new List<Ressource>();
	public List<Ressource> Ressources
	{
		get{return ressources;}
		set{ressources = value;}
	}
		
	 
	private Activity activity;
	public Activity Activity
	{
		get{return activity;}
		set{activity = value;}
	}

	public Procedure(string name) : base(name)

	{
	}
	
}
}

