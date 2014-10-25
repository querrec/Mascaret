using System.Collections;
using System.Collections.Generic;

public class Interaction {

	public List <ButtonActionInfo> interactions;

	public Interaction()
	{
		this.interactions = new List<ButtonActionInfo>();
	}
	
	public void addButtonAction(Button button, Action action, string target,bool pressed)
	{
		ButtonActionInfo bI = new ButtonActionInfo();
		bI.action = action;
		bI.button = button;
		bI.target = target;
		bI.pressed = pressed;
		
		interactions.Add(bI);
	}
}
