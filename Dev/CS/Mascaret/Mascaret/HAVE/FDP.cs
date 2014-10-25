
using System;
using System.Collections.Generic;




[Serializable]
public class FDP
{
	public float X
	{
		get{return coordonees.x;}
		set{coordonees.x = value;}
	}
	
	public float Y
	{
		get{return coordonees.y;}
		set{coordonees.y = value;}
	}
	
	public float Z
	{
		get{return coordonees.z;}
		set{coordonees.z = value;}
	}
	
	[SerializeField]
	private int groupIndex;
	public int GroupIndex
	{
		get{return groupIndex;}
		set{groupIndex = value;}
	}
	
	[SerializeField]
	private int index;
	public int Index
	{
		get{return index;}
		set{index = value;}
	}
	
	[SerializeField]
	private int refVertex;
	public int RefVertex
	{
		get{return refVertex;}
		set{refVertex = value;}
	}
	
	[SerializeField]
	private Face face;
	public Face Face
	{
		get{return face;}
		set{face = value;}
	}
	
	[SerializeField]
	private Vector3 coordonees;
	public Vector3 Coordonees
	{
		get{return coordonees;}
		set{coordonees = value;}
	}
	
	public FDP()
	{}
	
	public FDP(int groupIndex, int index, int refVertex)
	{
		this.groupIndex = groupIndex;
		this.index = index;
		this.refVertex = refVertex;
		
	}		
	
	public void setCoordonees()
	{
		coordonees = face.Vertices[refVertex];
	}
	
}
