using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class AID
    {
        public string name;

        private int plateformPort = 0;
        public int PlateformPort
        {
            get { return plateformPort; }
            set { plateformPort = value; }
        }


        private string plateformName;
        public string PlateformName
        {
            get { return plateformName; }
            set { plateformName = value; }
        }

        //default parameter plateformName = "", portNumber =0
        public AID(string name, string plateformName, int portNumber)
        {
            this.name = name;
            this.plateformName = plateformName;
            this.PlateformPort = portNumber;
        }

        public string toString()
        {
            return (name + "@" + plateformName + ":" + plateformPort);
        }

        public AID parseFromString(string aidStr)
        {
            char[] param = { '@', ':' };
            string[] splitedStr = aidStr.Split(param);
            AID newAID = new AID(splitedStr[0], splitedStr[1], int.Parse(splitedStr[2]));

            if (splitedStr.Length != 3)
                System.Console.WriteLine("AID STRING " + aidStr + "IS NOT CORRECTLY FORMATED");

            return newAID;

        }
    }
}

