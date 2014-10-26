using System;

namespace Mascaret
{
    public class TimeExpression
    {

        private long timeExp;
        public long TimeExp
        {
            get { return timeExp; }
            set { timeExp = value; }
        }

        public TimeExpression(int year, int month, int day, int hour, int min, int sec)
        {
            DateTime time = new DateTime(year, month, day, hour, min, sec);
            if (time != null) timeExp = time.Ticks / 10000;
        }

        /*public void instantiate(double dt)
        {
		
        }
		
        public void setValue(double time)
        {
			
        }
	
        public string getAsString()
        {
            return timeExp.ToString();
        }*/

        public static TimeExpression createTimeOfDay()
        {
            TimeExpression time = new TimeExpression(2014, 5, 1, 0, 0, 0);
            //time.TimeExp = DateTime.Now.ToOADate();
            time.TimeExp = DateTime.Now.Ticks / 10000;
            return time;
        }
    }
}


