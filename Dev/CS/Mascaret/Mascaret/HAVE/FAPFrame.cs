using System;
using System.Collections.Generic;

[Serializable]
public class FAPFrame
{
	[SerializeField]
	private Dictionary<int,float> fapFromFrame;
	public Dictionary<int,float> FapFromFrame
	{
		get{return fapFromFrame;}
		set{fapFromFrame = value;}
	}
	
	[SerializeField]
	private MapIntVector3 moveValueForFrame;
	public MapIntVector3 MoveValueForFrame
	{
		get{return moveValueForFrame;}
		set{moveValueForFrame = value;}
	}
	
	[SerializeField]
	private int index;
	public int Index
	{
		get{return index;}
		set{index = value;}
	}

	public void instantiate(int index)
	{
		this.index = index;
		moveValueForFrame = (MapIntVector3)ScriptableObject.CreateInstance<MapIntVector3>();
		fapFromFrame = (MapIntFloat)ScriptableObject.CreateInstance<MapIntFloat>();
	}		

	public void createAnimationFrame(Face face)
	{
		foreach(int currentFapIndex in fapFromFrame.KeysList)
		{				
			face.Faps[currentFapIndex].Move = fapFromFrame.getMap(currentFapIndex);
			
			foreach (FacialZone currentZone in face.getZoneByFap(currentFapIndex))
			{				
				foreach(KeyValuePair<int,Vector3> currentPoint in currentZone.calculatePose(face))
				{
					if(!moveValueForFrame.ContainsKey(currentPoint.Key))
						moveValueForFrame.Add(currentPoint.Key, currentPoint.Value);
				}
				
			}
		}

	}
}

