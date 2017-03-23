//
// Created by matthew on 2/28/2017.
//

#ifndef DATETIME_H
#define DATETIME_H
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "TimeSpan.h"

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
        fastForward(Years, 0, 0, 0, 0, 0, 0);
    }
    void addMonths(int Months) {
        fastForward(0, Months, 0, 0, 0, 0, 0);
    }
    void addWeeks(int Weeks) {
        fastForward(0, 0, Weeks * 7, 0, 0, 0, 0);
    }
    void addDays(int Days) {
        fastForward(0, 0, Days, 0, 0, 0, 0);
    }
    void addHours(int Hours) {
        fastForward(0, 0, 0, Hours, 0, 0, 0);
    }
    void addMinutes(int Minutes) {
        fastForward(0, 0, 0, 0, Minutes, 0, 0);
    }
    void addSeconds(int Seconds) {
        fastForward(0, 0, 0, 0, 0, Seconds, 0);
    }
    void addMilliseconds(int millis) {
        fastForward(0, 0, 0, 0, 0, 0, millis);
    }

    void fastForward(int Years, int Months, int Days, int Hours, int Minutes, int Seconds, int Milliseconds) {
        years += Years;
        months += Months;
        days += Days;
        hours += Hours;
        minutes += Minutes;
        seconds += Seconds;
        milliseconds += Milliseconds;
        adjustTime();
    }

    void fastForward(TimeSpan span) {
        fastForward(span.Year(), span.Month(), span.Day(), span.Hour(), span.Minute(), span.Second(), span.Millisecond());
    }

    void rewind(int Years, int Months, int Days, int Hours, int Minutes, int Seconds, int Milliseconds) {
        years -= Years;
        months -= Months;
        days -= Days;
        hours -= Hours;
        minutes -= Minutes;
        seconds -= Seconds;
        milliseconds -= Milliseconds;
        adjustTimeBackwards();
    }

    void rewind(TimeSpan span) {
        rewind(span.Year(), span.Month(), span.Day(), span.Hour(), span.Minute(), span.Second(), span.Millisecond());
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

    void toLocalTime() {
        time_t rawtime;
        time(&rawtime);
        *this = parseTimeInfo(localtime(&rawtime));
    }

    static DateTime Now() {
        return DateTime(1970,1,1,0,0,time(NULL),0);
    }

    static DateTime LocalNow() {
        DateTime currentTime = Now();
        currentTime.toLocalTime();
        return currentTime;
    }

    friend ostream& operator>>(ostream& out, DateTime date) {
        out << date.Date();
        return out;
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

    string toLower(string str) {
        string result = "";
        for(int i = 0; i < str.length(); i++) {
            result += ((int) str[i]) >= 97 ? str[i] : ((char) (((int)str[i]) + 32));
        }
        return result;
    }

    int convertMonth(string month) {
        month = toLower(month);
        if(month == "jan" || month == "january") return 1;
        if(month == "feb" || month == "february") return 2;
        if(month == "mar" || month == "March") return 3;
        if(month == "apr" || month == "april") return 4;
        if(month == "may") return 5;
        if(month == "jun" || month == "june") return 6;
        if(month == "jul" || month == "july") return 7;
        if(month == "aug" || month == "august") return 8;
        if(month == "sept" || month == "september") return 9;
        if(month == "oct" || month == "october") return 10;
        if(month == "nov" || month == "november") return 11;
        if(month == "dec" || month == "december") return 12;
    }

    string convertMonth(int month) {
        switch(month) {
            case 1: return "January";
            case 2: return "February";
            case 3: return "March";
            case 4: return "April";
            case 5: return "May";
            case 6: return "June";
            case 7: return "July";
            case 8: return "August";
            case 9: return "September";
            case 10: return "October";
            case 11: return "November";
            case 12: return "December";
            default: return "InvalidMonth";
        }
    }


    DateTime parseTimeInfo(tm* info) {
        string time = asctime(info);
        stringstream stream;
        stream << time;

        string dayOfWeek, month;
        char colon;
        int  day, hour, minute, second, year;

        stream >> dayOfWeek >> month >> day >> hour >> colon >> minute >> colon >> second >> year;
        return DateTime(year, convertMonth(month), day, hour, minute, second,0);
    }

    int abs(int x) { return x >= 0 ? x : -1*x; }

    void adjustTimeBackwards() {
        seconds -= milliseconds / 1000;
        milliseconds = 1000 - (milliseconds % 1000);

        if(seconds < 0) {
            minutes -= abs(seconds / 60);
            seconds = 60 - abs(seconds % 60);
        }

        if(minutes < 0) {
            hours -= abs(minutes / 60);
            minutes = 60 - abs(minutes % 60);
        }

        int previousDays = days;
        if(hours < 0) {
            days -= abs(hours / 24);
            hours = 24 - abs(hours % 24);
        }

        if(days < 0) {
            days = abs(days);
            days -= previousDays;
            if(months - 1 < 1) {
                months = 12;
                years--;
            } else {
                months--;
            }
            do {
                if(daysInTheMonth() - days < 0) {
                    days -= daysInTheMonth();
                    if(months - 1 < 1) {
                        months = 12;
                        years--;
                    } else {
                        months--;
                    }
                }
            } while(days > daysInTheMonth());
            days = daysInTheMonth() - days;
        }
    }

    int years, months, days, hours, minutes, seconds, milliseconds;
};
#endif //DATETIME_H
