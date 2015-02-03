using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Mascaret;


public class UnityWindow3D : Windows3D {

	private UnityMascaretInterface inter;

	public UnityWindow3D()
	{
		GameObject mascaretO = GameObject.Find ("MascaretApplication");
		UnityMascaretApplication uma = (UnityMascaretApplication)mascaretO.GetComponent ("UnityMascaretApplication");
		VRApplication mascaret = uma.mascaret;
		inter = (UnityMascaretInterface)mascaretO.GetComponent ("UnityMascaretInterface");
	}

	public override Entity getSelectedEntity()
	{
		Ray ray = UnityEngine.Camera.main.ScreenPointToRay (Input.mousePosition);
		RaycastHit hit;
		if (Physics.Raycast (ray, out hit, 200.0f))
		{
			GameObject go = hit.collider.gameObject;
			EntityGo ego = go.GetComponent<EntityGo>();
			if (ego != null)
            {
			   return ego.entity;
            }
			else 
            {
                Debug.Log ("No selection");
                return null;
            }
		}
		else
        {
			return null;
        }
	}

	public void hideProperty(string instanceName)
	{
		inter.hideEntity (instanceName);
	}

	public void showProperty(string instanceName)
	{
		inter.printEntity (instanceName);
	}
}
