#ifndef _v2_VEHA_Behavior_Time_TimeExpression_H
#define _v2_VEHA_Behavior_Time_TimeExpression_H

#include "Tools/veha_plateform.h"

#include <string>

namespace VEHA
{

class VEHA_API TimeExpression
{
    public:
        TimeExpression();
        explicit TimeExpression(double);
        TimeExpression(int year, int month, int day, int hour, int min, int sec);
        virtual ~TimeExpression();

        double getValue() const;
        void setValue(double time);
       
        /** Return TimeExpression as a double in seconds since EPOCH */ 
        double getAsDouble() const;

        std::string getAsString() const;

        bool operator==(TimeExpression const &other) const;
        bool operator!=(TimeExpression const &other) const;
        bool operator<(TimeExpression const &other) const;
        bool operator<=(TimeExpression const &other) const;
        bool operator>(TimeExpression const &other) const;
        bool operator>=(TimeExpression const &other) const;
        double operator-(TimeExpression const &other) const;
        TimeExpression operator+(double const &delta) const;
        TimeExpression operator+=(double const &delta);
        TimeExpression operator-=(double const &delta);

        static TimeExpression createTimeOfDay();
    protected:
        double _time;
};

inline bool TimeExpression::operator==(TimeExpression const &other) const
{
    return _time == other._time;
}

inline bool TimeExpression::operator!=(TimeExpression const &other) const
{
    return !(*this == other);
}

inline bool TimeExpression::operator<(TimeExpression const &other) const
{
    return _time < other._time;
}

inline bool TimeExpression::operator<=(TimeExpression const &other) const
{
    return _time <= other._time;
}

inline bool TimeExpression::operator>(TimeExpression const &other) const
{
    return _time > other._time;
}

inline bool TimeExpression::operator>=(TimeExpression const &other) const
{
    return _time >= other._time;
}

inline TimeExpression TimeExpression::operator+(double const &delta) const
{
    return TimeExpression(_time + delta);
}
        
inline double TimeExpression::operator-(TimeExpression const &other) const
{
    return (double)(_time - other._time);
}

inline TimeExpression TimeExpression::operator+=(double const &delta)
{
    _time += delta;
    return (*this);
}

inline TimeExpression TimeExpression::operator-=(double const &delta)
{
    _time -= delta;
    return (*this);
}

inline double TimeExpression::getValue() const
{
    return _time;
}

}
#endif
