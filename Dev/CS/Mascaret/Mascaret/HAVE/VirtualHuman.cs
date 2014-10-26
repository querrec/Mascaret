using System;
using System.Collections.Generic;
//using TextToSpeech;

namespace Mascaret
{
    public class VirtualHuman : EmbodiedAgent
    {
        //protected TTSChannel tc;
        //protected string textToSpeak ="";
        //System.Threading.Thread newThread;
        //SpeechData currentSD;


        public VirtualHuman(AgentPlateform ap, string name, AgentClass agentclass)
            : base(ap, name, agentclass)
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


        public double prepareSpeak(string text)
        {
            return Body.prepareSpeak(text);
        }
        public bool speak(string text)
        {
            return Body.speak(text);
           
        }

    }
}

