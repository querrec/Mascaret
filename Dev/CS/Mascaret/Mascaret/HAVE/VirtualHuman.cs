using System;
using System.Collections.Generic;
using TextToSpeech;

[Serializable]
public class VirtualHuman : EmbodiedAgent
{	
	protected TTSChannel tc;
	protected string textToSpeak ="";
	System.Threading.Thread newThread;
	SpeechData currentSD;

	
	public VirtualHuman(AgentPlateform ap, string name, AgentClass agentclass) : base(ap,name,agentclass)

	{
		System.Console.WriteLine("VIRTUAL HUMAN");		
	}
	
	public bool playAnimation(string animationName, int sens, double animationSpeed, bool cycle)// int sens = 1, double animationSpeed = 1.0, bool cycle = false
	{
		//return Body.playAnimation(animationName,sens,animationSpeed, cycle);
		return false;
	}
	
	public bool stopAnimation(string animationName)
	{
		//return Body.stopAnimation(animationName);
		return false;
	}
		
	public bool stopAllAnimations()
	{
		//return Body.stopAllAnimations();
		return false;
	}
		
	public bool playSound(string soundName, double pitch, bool cycle) // double pitch = 1.0, bool cycle = false
	{
		//return Body.playSound(soundName,pitch, cycle);
		return false;
	}
		
	bool stopSound(string soundName)
	{
		//return Body.stopSound(soundName);
		return false;
	}

		
	public float prepareSpeak(string text)
	{
        this.currentSD = this.tc.speak(text,true);
		float duration = 0;
		SpeechData.Phoneme [] phonemes = currentSD.phonemes;
		if (phonemes.Length == 0)
			return 0;

		for (int i = 0; i < phonemes.Length; i++)
		{
			duration += phonemes[i].end - phonemes[i].start;
		}
		return duration;

	}
	public bool speak(string text)
	{
		
		//UnityShapeSpecification shape = (UnityShapeSpecification)(this.Body.ActiveShape);
		/*
		AudioSource audioS = (AudioSource)shape.EntityGO.gameObject.GetComponent("AudioSource");
		audioS.clip = AudioClip.Create ("text",500000,1,22050,true,false);
		float [] buffer = new float[currentSD.audiobuf.Length];
		for (int iPCM = 0; iPCM < currentSD.audiobuf.Length; iPCM++)
		{
			float f;
			int i = (int)currentSD.audiobuf[iPCM];
			f = ((float) i) / (float) 32768;
			if( f > 1 ) f = 1;
			if( f < -1 ) f = -1;
			buffer[iPCM] = f;
		}
		audioS.clip.SetData(buffer,0);
		audioS.Play();
		*/
		//	this.Body.Face.readFapFile("c:/data/Prudence/animation.fap");
		//	this.Body.Face.applyFap();
		return true;
	}
	/*
    public bool bilalspeak(string text)
    {

        AudioSource audsrc;
        GameObject test = new GameObject();
        audsrc=(AudioSource)test.AddComponent("AudioSource");
        //audsrc.clip=AudioClip.Create("voice",220500,1,22050,true,false);
        audsrc.clip = AudioClip.Create ("text",500000,1,22050,true,false);
        audsrc.volume=1;
        tc = new TTSChannel(MascaretApplication.Instance.ttsEngine,0);//bilal
        SpeechData sd = this.tc.speak(text,false);
        int length = sd.audiobuf.Length;
        float [] buffer = new float[length];
        for (int i = 0 ; i <length; i++)
        {
            float f;
            int val = sd.audiobuf[i];
            f = ((float) val) / (float) 32768;
            if( f > 1 ) f = 1;
            if( f < -1 ) f = -1;
            buffer[i] = f;
        }
        audsrc.clip.SetData( buffer,0);
        audsrc.Play();
        
        //  this.Body.Face.readFapFile("c:/data/Prudence/animation.fap");
        //  this.Body.Face.applyFap();
        return true;
    }
    */
	
	public override void start()
	{
	//	base.start ();
	//	tc = new TTSChannel(MascaretApplication.Instance.ttsEngine,0);
	//	this.Body.Face.loadData();
	}
}

