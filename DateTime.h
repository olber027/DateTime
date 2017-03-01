//
// Created by matthew on 2/28/2017.
//

#ifndef DATETIME_H
#define DATETIME_H
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class DateTime {
public:
    DateTime() : years(0), months(1), days(1), hours(0),
                 minutes(0), seconds(0), milliseconds(0) { }
    DateTime(int Year, int Month, int Day, int Hours,
             int Minutes, int Seconds, int Milliseconds) :
            years(Year), months(Month), days(Day), hours(Hours),
            minutes(Minutes), seconds(Seconds), milliseconds(Milliseconds)
    {
        sanitizeData();
        adjustTime();
    }

    DateTime(int Year, int Month, int Day) : DateTime(Year, Month, Day, 0, 0, 0, 0) {}
    DateTime(int Year, int Month, int Day, int Hour, int Minute, int Second) :
            DateTime(Year, Month, Day, Hour, Minute, Second, 0) {}

    int Year() { return years; }
    int Month() { return months; }
    int Day() { return days; }
    int Hour() { return hours; }
    int Minute() { return minutes; }
    int Second() { return seconds; }
    int Millisecond() { return milliseconds; }

    void addYears(int Years) {
        addTime(Years,0,0,0,0,0,0);
    }
    void addMonths(int Months) {
        addTime(0,Months,0,0,0,0,0);
    }
    void addWeeks(int Weeks) {
        addTime(0,0,Weeks*7,0,0,0,0);
    }
    void addDays(int Days) {
        addTime(0,0,Days,0,0,0,0);
    }
    void addHours(int Hours) {
        addTime(0,0,0,Hours,0,0,0);
    }
    void addMinutes(int Minutes) {
        addTime(0,0,0,0,Minutes,0,0);
    }
    void addSeconds(int Seconds) {
        addTime(0,0,0,0,0,Seconds,0);
    }
    void addMilliseconds(int millis) {
        addTime(0,0,0,0,0,0,millis);
    }


    void addTime(int Years, int Months, int Days, int Hours, int Minutes, int Seconds, int Milliseconds) {
        years += Years;
        months += Months;
        days += Days;
        hours += Hours;
        minutes += Minutes;
        seconds += Seconds;
        milliseconds += Milliseconds;
        adjustTime();
    }

    string Date() {
        stringstream stream;
        stream << setfill('0') << setw(2) << months << "/";
        stream << setfill('0') << setw(2) << days << "/";
        stream << setfill('0') << setw(4) << years << " ";
        stream << setfill('0') << setw(2) << hours << ":";
        stream << setfill('0') << setw(2) << minutes << ":";
        stream << setfill('0') << setw(2) << seconds << ":";
        stream << setfill('0') << setw(4) << milliseconds;

        string date, time;
        stream >> date >> time;
        time = " " + time;
        return date + time;
    }

    string DateOnly() {
        stringstream stream;
        stream << setfill('0') << setw(2) << months << "/";
        stream << setfill('0') << setw(2) << days << "/";
        stream << setfill('0') << setw(4) << years << " ";

        string date;
        stream >> date;
        return date;
    }

    string Time() {
        stringstream stream;
        stream << setfill('0') << setw(2) << hours << ":";
        stream << setfill('0') << setw(2) << minutes << ":";
        stream << setfill('0') << setw(2) << seconds << ":";

        string time;
        stream >> time;
        return time;
    }


    int daysInTheYear() {
        if(isLeapYear()) {
            return 366;
        }
        return 365;
    }

    int daysInTheMonth() {
        switch(months) {
            case 4 :
            case 6 :
            case 9 :
            case 11 :
                return 30;
            case 1 :
            case 3 :
            case 5 :
            case 7 :
            case 8 :
            case 10 :
            case 12 :
                return 31;
            case 2 :
                if(isLeapYear()) {
                    return 29;
                }
                return 28;
        }
    }

    bool isLeapYear() {
        if((years % 4) == 0) {
            if((years % 100) == 0) {
                if((years % 400) == 0) {
                    return true;
                }
            } else {
                return true;
            }
        }
        return false;
    }

private:

    void sanitizeData() {
        if(months < 1) months = 1;
        if(days < 1) days = 1;
        if(hours < 0) hours = 0;
        if(minutes < 0) minutes = 0;
        if(seconds < 0) seconds = 0;
        if(milliseconds < 0) milliseconds = 0;
    }

    void adjustTime() {
        seconds += milliseconds / 1000;
        milliseconds = milliseconds % 1000;

        minutes += seconds / 60;
        seconds = seconds % 60;

        hours += minutes / 60;
        minutes = minutes % 60;

        days += hours / 24;
        hours = hours % 24;

        years += months / 12;
        months = months%12==0 ? 12 : months%12;

        while(days > daysInTheMonth()) {
            days -= daysInTheMonth();
            months++;
            if(months > 12) {
                years++;
                months = 1;
            }
        }
    }

    int years, months, days, hours, minutes, seconds, milliseconds;
};
#endif //DATETIME_H
