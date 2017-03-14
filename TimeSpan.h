//
// Created by molberding on 3/13/2017.
//

#ifndef TIMESPAN_H
#define TIMESPAN_H

class TimeSpan {
public:
    TimeSpan() : years(0), months(1), days(1), hours(0),
                 minutes(0), seconds(0), milliseconds(0) { }
    TimeSpan(int Year, int Month, int Day, int Hours,
             int Minutes, int Seconds, int Milliseconds) :
            years(Year), months(Month), days(Day), hours(Hours),
            minutes(Minutes), seconds(Seconds), milliseconds(Milliseconds)
    {
        sanitizeData();
    }

    TimeSpan(int Year, int Month, int Day) : TimeSpan(Year, Month, Day, 0, 0, 0, 0) {}
    TimeSpan(int Year, int Month, int Day, int Hour, int Minute, int Second) :
            TimeSpan(Year, Month, Day, Hour, Minute, Second, 0) {}

private:

    void sanitizeData() {
        if(months < 1) months = 1;
        if(days < 1) days = 1;
        if(hours < 0) hours = 0;
        if(minutes < 0) minutes = 0;
        if(seconds < 0) seconds = 0;
        if(milliseconds < 0) milliseconds = 0;
    }

    int years, months, days, hours, minutes, seconds, milliseconds;
};

#endif //TIMESPAN_H
