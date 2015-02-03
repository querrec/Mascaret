using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Mascaret;
using System.IO;

public class OSDWindow {
	public string value ="";
	const int kPropertyNameColumnWidth = 150;
	const int kValueColumnWidth = 50;
	const int kBoxHeight = 100;
	const int kBoxWidth = kPropertyNameColumnWidth + kValueColumnWidth + 30;
	public Rect windowRect; 
	public Vector2 scrollPosition;
	private int indice;
	private string entityName;
	public VRApplication mascaret;
	bool first = true;
	string[] values;

	public OSDWindow(string entityName)
	{
		this.indice = indice;
		this.entityName = entityName;
		mascaret = VRApplication.Instance;
		Environment env = mascaret.getEnvironment ();
		InstanceSpecification instance = env.getInstance (this.entityName);
		Dictionary<string,Slot> ss = instance.Slots;
		first = true;
		values = new string[ss.Count];
	}

	public void show(int indice)
	{
		windowRect = new Rect(5, 5 + (indice*130), kBoxWidth + 20, kBoxHeight + 20);
		windowRect = GUI.Window (indice, windowRect, DoMyWindow, entityName);

	}

	void DoMyWindow(int windowID) {
		const int kPropertyNameColumnWidth = 150;
		const int kValueColumnWidth = 50;
		const int kBoxHeight = 100;
		const int kBoxWidth = kPropertyNameColumnWidth + kValueColumnWidth + 30;
		Environment env = mascaret.getEnvironment ();
		InstanceSpecification instance = env.getInstance (this.entityName);
		Dictionary<string,Slot> ss = instance.Slots;

		// Wrap everything in the designated GUI Area
		GUILayout.BeginArea (new Rect (15, 17, kBoxWidth, kBoxHeight));
		scrollPosition = GUILayout.BeginScrollView (scrollPosition, "", GUILayout.Width (kBoxWidth), GUILayout.Height (kBoxHeight));
		GUILayout.BeginVertical();
		int xx = 0;
		foreach (KeyValuePair<string,Slot> s in ss) 
		{
			string propertyName = s.Value.DefiningProperty.name;
			//values[xx] = ""; 
			
			string type = s.Value.DefiningProperty.Type.name;
			
			Debug.Log(propertyName + " is " + type);
			if (type == "real" || type == "string" || type == "integer" || type =="boolean")
			{
				if (type == "real")
				{
					LiteralReal r = (LiteralReal) (s.Value.getValue());
					double v = r.RValue;
					if (first) values[xx] = v.ToString();
				}
				else if (type == "string")
				{
					LiteralString st = (LiteralString) (s.Value.getValue());
					string v = st.SValue;
					if (first) values[xx] = v;
				}
				else if (type == "integer")
				{
					LiteralInteger i = (LiteralInteger) (s.Value.getValue());
					int v = i.IValue;
					if (first) values[xx] = v.ToString();
				}
				else if (type == "boolean")
				{
					LiteralBoolean b = (LiteralBoolean) (s.Value.getValue());
					bool v = b.BValue;
					if (first) values[xx] = v.ToString();
				}
			
				GUILayout.BeginHorizontal();
				GUILayout.Label(propertyName, GUILayout.Width(kPropertyNameColumnWidth));
				values[xx] = GUILayout.TextField(values[xx], 25, GUILayout.Width(kValueColumnWidth));
				if (type == "real") s.Value.addValue(new LiteralReal(values[xx]));
				xx++;
				GUILayout.EndHorizontal();
			}
		}	
		GUILayout.EndVertical();
		GUILayout.EndScrollView();
		GUILayout.EndArea();
		
		GUI.DragWindow(new Rect(0, 0, 10000, 10000));
		first = false;
	}
}






