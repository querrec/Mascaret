using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Mascaret;
using System.IO;


public class UnityMascaretInterface : MonoBehaviour {
	
	public VRApplication mascaret;
	private Dictionary<string, OSDWindow> windows = new Dictionary<string, OSDWindow> ();

	public void printEntity(string entityName)
	{
		OSDWindow window = new OSDWindow (entityName);
		windows.Add (entityName, window);
	}
	
	public void hideEntity(string entityName)
	{
		windows.Remove (entityName);
		Debug.Log ("TOPRINT == FALSE");
	}

	void Start()
	{
		mascaret = VRApplication.Instance;
	}
	
	public void OnGUI()
	{
		int indice = 0;
		foreach (KeyValuePair<string, OSDWindow> win in windows) 
		{
			OSDWindow w = win.Value;
			w.show(indice);
			indice ++;
		}
	}
}

