using System;
using System.Collections.Generic;

[Serializable]
public class Comment 
{

	 
	protected string body;
	public string Body
	{
		get{return body;}
		set{body = value;}
	}
		
	public  Comment()
	{
	}
}
