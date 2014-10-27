using UnityEngine;
using System.Collections;
using Mascaret;


public class UnityMouse : Peripheric {

	public UnityMouse() : base("mouse")
	{
		Button b;
		b = new Button("button1"); addButton(b);
		b = new Button("button2"); addButton(b);
		b = new Button("button3"); addButton(b);
	}
}
