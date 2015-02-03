using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Mascaret;
using System.IO;

public class UnityMascaretApplication : MonoBehaviour {

	public VRApplication mascaret;
	public Vector2 scrollPosition; 
	public bool debugMode = true;
	
	public string applicationFile;
	List<string> keyPressed = new List<string>();

	
	void Start()
	{
		mascaret = VRApplication.Instance;

		mascaret.window = new UnityWindow3D();
		mascaret.VRComponentFactory = new UnityVirtualRealityComponentFactory();
		mascaret.window.addPeripheric(new UnityKeyboard());
		mascaret.window.addPeripheric(new UnityMouse());
		mascaret.parse (applicationFile,Application.dataPath+"/StreamingAssets/");

	}
	
	void Update()
	{
		mascaret.step();
	}

	public void OnGUI()
	{
		Event current = Event.current;
		if (current.isMouse)
		{
			int buttonNumber = current.button +1;
			string buttonName = "button" + buttonNumber;
			Button b = this.mascaret.window.getPeripheric("mouse").getButton(buttonName);
			if (Input.GetMouseButtonDown(current.button))
				b.setPressed(true);
			else
				b.setPressed (false);
		}
		else if (current.isKey)
		{
			if (current.keyCode.ToString() != "None")
			{
				//Debug.Log(current.keyCode.ToString() + " / " + current.type + " : " + current.clickCount);
				Button b = this.mascaret.window.getPeripheric("keyboard").getButton(current.keyCode.ToString());
				if(b != null) 
				{
					if (current.type == EventType.keyDown) 
					{
						if (!keyPressed.Contains(current.keyCode.ToString()))
						{
							Debug.Log(current.keyCode.ToString() + " / " + current.type + " : " + current.clickCount);
							b.setPressed(true);
							keyPressed.Add(current.keyCode.ToString());
						}
					}
					else if (current.type == EventType.keyUp) 
					{
						b.setPressed(false);
						keyPressed.Remove(current.keyCode.ToString());
					}
				}
				//else Debug.Log("Button : " + current.keyCode.ToString() + " Not found");
			}
		}
	}

	
	
}

