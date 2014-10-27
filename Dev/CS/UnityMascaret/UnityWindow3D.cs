using UnityEngine;
using System.Collections;
using Mascaret;


public class UnityWindow3D : Windows3D {

	public override Entity getSelectedEntity()
	{
		Ray ray = UnityEngine.Camera.main.ScreenPointToRay (Input.mousePosition);
		RaycastHit hit;
		if (Physics.Raycast (ray, out hit, 100.0f))
		{
			GameObject go = hit.collider.gameObject;
			EntityGo ego = go.GetComponent<EntityGo>();
			return ego.entity;
		}
		else
			return null;
	}
}
