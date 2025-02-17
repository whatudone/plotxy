﻿
#ifndef SIMCORE_TIME_UTILS_H
#define SIMCORE_TIME_UTILS_H

#include "constdef.h"
#include <string>

#include <QColor>
#include <QPainterPath>
class TimeStamp;

namespace simCore
{
//------------------------------------------------------------------------
//
// <time.h> struct tm definition
//
//    int  tm_sec;   /* seconds after the minute - [0, 61] for leap seconds */
//    int  tm_min;   /* minutes after the hour - [0, 59] */
//    int  tm_hour;  /* hour since midnight - [0, 23] */
//    int  tm_mday;  /* day of the month - [1, 31] */
//    int  tm_mon;   /* months since January - [0, 11] */
//    int  tm_year;  /* years since 1900 */
//    int  tm_wday;  /* days since Sunday - [0, 6] */
//    int  tm_yday;  /* days since January 1 - [0, 365] */
//    int  tm_isdst; /* flag for alternate daylight savings time */
//
/**
  * The system's notion of the current time is obtained with the gettimeofday call.  The time is expressed in seconds and micro-
  * seconds since midnight (00:00) Coordinated Universal Time (UTC), January 1, 1970.
  * @return system time a double in seconds since midnight UTC Jan 1, 1970.
  */
double getSystemTime();

/**
  * The system's notion of the current time is obtained with the gettimeofday call.  The time is expressed in seconds and micro-
  * seconds since midnight (00:00) Coordinated Universal Time (UTC), January 1, 1970.
  * @return system time a double in seconds referenced to the beginning of the current Gregorian year (or "calendrical year").
  */
double systemTimeToSecsBgnYr();

/**
  * The system's notion of the current time is obtained with the gettimeofday call.  The time is expressed in seconds and micro-
  * seconds since midnight (00:00) Coordinated Universal Time (UTC), January 1, 1970.
  * @param[out] pSecs A pointer to an unsigned int.  Stores elapsed seconds referenced to the beginning of the current Gregorian year (or "calendrical year").
  * @param[out] pMillisec A pointer to an unsigned short.  Stores elapsed milliseconds referenced to the beginning of the current Gregorian year (or "calendrical year").
  * @pre valid seconds and milliseconds params
  */
void systemTimeToSecsBgnYr(unsigned int& pSecs, unsigned short& pMillisec);

/**
  * The system's notion of the current time is obtained with the gettimeofday call.  The time is expressed in seconds and micro-
  * seconds since midnight (00:00) Coordinated Universal Time (UTC), January 1, 1970.
  * @return system time a double in seconds referenced to the beginning of the current day.
  */
double systemTimeToSecsBgnDay();

/**
  * Converts a UTC system time broken into seconds and millisecs referenced to the beginning of the current Gregorian year (or "calendrical year").
  * @param[in ] timeSinceJan1970 A double containing time referenced to seconds since midnight (00:00) Coordinated Universal Time (UTC), January 1, 1970.
  * @param[out] pSecs A pointer to an unsigned int.  Stores seconds referenced to the beginning of the current Gregorian year (or "calendrical year").
  * @param[out] pMillisec A pointer to an unsigned short.  Stores millisec referenced to the beginning of the current Gregorian year (or "calendrical year").
  * @param[out] pRefyear A pointer to an unsigned int.  Stores year of data referenced to the Gregorian year (i.e. 2003 or "calendrical year").
  * @pre valid seconds, milliseconds and reference year params
  */
void timeSinceJan1970ToSecsBgnYr(double timeSinceJan1970,
                                 unsigned int& pSecs,
                                 unsigned short& pMillisec,
                                 unsigned int& pRefyear);

/**
  * Returns the current Gregorian year.  A Gregorian year (or "calendrical year") is the number of days in a given year of the Gregorian calendar (namely, 365 days in non-leap years and 366 days in a leap year)
  * @return current year an integer referenced to the Gregorian calendar.
  */
int currentYear();

/**
  *  Verify a given day, month and year is valid.
  * @param[in ] monthDay Day of month (1-31)
  * @param[in ] month Month of year (1-12)
  * @param[in ] year Gregorian calendar year (1900-)
  * @throw TimeException
  */
void checkValidDMY(unsigned int monthDay, unsigned int month, int year);

/**
  * Returns a boolean whether or not the indicated day, month, year is valid.
  * @param[in ] monthDay An integer containing the day in the range of [1-31]
  * @param[in ] month An integer containing the month in the range of [1-12]
  * @param[in ] year Gregorian calendar year [1900-)
  * @return boolean value indicating whether or not the given day, month, year was valid.
  */
bool isValidDMY(unsigned int monthDay, unsigned int month, int year);

//------------------------------------------------------------------------

/** @name TimeException types
  * TimeException types.
  */
//@{
///TimeException types.
/**
  * A list of handled time exception types.  Used to identify the exception
  */
static const int TIME_STRING_NOT_VALID = 1; /**< Input time string is not valid. */
static const int JULIANDAY_NOT_VALID = 2; /**< Input Julian day is not valid. */
static const int DAY_STRING_NOT_VALID = 3; /**< Input day string is not valid. */
static const int MONTH_NOT_VALID = 4; /**< Input month is not valid. */
static const int MONTHDAY_NOT_VALID = 5; /**< Input month day is not valid. */
static const int HOURS_NOT_VALID = 6; /**< Input hours is not valid. */
static const int YEAR_NOT_VALID = 7; /**< Input year is not valid. */
static const int WEEKDAY_NOT_VALID = 8; /**< Input weekday is not valid. */
static const int TOO_MANY_VALUES = 9; /**< Too many values input. */
static const int REFERENCE_YEAR_NOT_VALID = 10; /**< Input reference year is not valid. */
static const int SECONDS_SINCE_EPOCHTIME_NOT_VALID =
    11; /**< Input seconds since epoch time is not valid. */
static const int STRING_FORMAT_NOT_VALID = 12; /**< Input string format is not valid. */
static const int YEARDAY_NOT_VALID = 13; /**< Input year day is not valid. */
static const int MINUTES_NOT_VALID = 14; /**< Input minutes is not valid. */
static const int SECONDS_NOT_VALID = 15; /**< Input seconds is not valid. */
static const int DELTAT_NOT_VALID = 16; /**< Input Julian date is not valid. */
static const int GPS_WEEK_NOT_VALID = 17; /**< Input GPS week not valid. */
static const int GPS_EPOCH_NOT_VALID = 18; /**< Input GPS epoch not valid. */
static const int UTC_NOT_VALID_FOR_GPS = 19; /**< Input UTC not valid for GPS */
//@}

/**
  * Returns a tm time struct that corresponds to the input time referenced to the input epoch.
  * Note: the struct tm uses an int for storing the seconds value, so the tm struct that is returned
  * by this function is less accurate a time than the input seconds since epoch time.
  * @param[in ] secSinceBgnOfEpochTime A double containing seconds (>=0) referenced to the input input epoch.
  * @param[in ] yearsSince1900 year to use as the reference time.
  * @return A tm time struct referenced to the input time referenced to the input epoch.
  * @throw TimeException
  */
tm getTimeStruct(double secSinceBgnOfEpochTime, unsigned int yearsSince1900);

/**
  * Returns a tm time struct that corresponds to the input timeStamp.
  * Note: the struct tm uses an int for storing the seconds value, so the tm struct that is returned
  * by this function is less accurate a time than the input seconds since epoch time.
  * @param[in ] timeStamp A timestamp containing reference year and seconds since that reference year.
  * @return A tm time struct referenced to the input timeStamp.
  */
tm getTimeStruct(const TimeStamp& timeStamp);

/**
  * Returns the difference in seconds between two tm time structs
  * @param[in ] epochTime A tm time struct containing the reference time.
  * @param[in ] compareTime A tm time struct containing the comparison time.
  * @return the difference in seconds between two tm time structs as a double.
  * @throw TimeException
  */
double getTimeStructDifferenceInSeconds(const tm& epochTime, const tm& compareTime);

/**
  * Returns the # of days since the beginning of the Gregorian year ("calendrical year") that corresponds to the given month and monthDay values
  *   month values [0, 11]
  *   monthDay values [1, DaysPerMonth(year, month)]
  * @param[in ] month An integer containing the month in the range of [0-11]
  * @param[in ] monthDay An integer containing the day of the month
  * @param[in ] year Year value; values less than 1900 will be treated as 1900+year
  * @return integer containing the number of days since the beginning of the year for the specified input values.
  * @throw TimeException
  */
int getYearDay(int month, int monthDay, int year);

/**
  * Assigns the values of "month" and "monthDay" that correspond to the given year and the associated Gregorian year day value.
  *   month values [0, 11]
  *   monthDay values [1, DaysPerMonth(year, month)]
  *   yearDay values [0,DaysPerYear(year)]
  * @param[out] month An integer that is assigned the month in the range of [0-11]
  * @param[out] monthDay An integer that is assigned the day of the month
  * @param[in ] year Year value; values less than 1900 will be treated as 1900+year
  * @param[in ] yearDay An integer containing the number of days since the beginning of the specified Gregorian year [0-365]
  * @throw TimeException
  * @pre month and monthDay valid params
  */
void getMonthAndDayOfMonth(int& month, int& monthDay, int year, int yearDay);

/**
  * Returns the week day value [0-6] mapping to [Sunday,...,Saturday] that corresponds to the given years since 1900 and the associated Gregorian year day value.
  *   yearsSince1900 >= 0, i.e. 1999 would be represented as 99
  *   yearDay values [0,DaysPerYear(yearsSince1900)]
  * @param[in ] yearsSince1900 An integer containing the number of elapsed years since 1900, must be <= 200.
  * @param[in ] yearDay An integer containing the number of days since the beginning of the specified Gregorian year
  * @return integer containing the week day number [0-6] associated to the given input values.
  * @throw TimeException
  */
int getWeekDay(int yearsSince1900, int yearDay);

/**
  * Returns the week day value [0-6] ([Sunday,...,Saturday]) for Jan 01 of the most recent leap year that corresponds to the given yearsSince1900.
  *   yearsSince1900 >= 0, i.e. 1999 would be represented as 99
  * @param[in ] yearsSince1900 An integer containing the number of elapsed years since 1900, must be <= 200.
  * @return integer containing the week day [0-6] of the first day of the most recent leap year that corresponds to the given yearsSince1900.
  * @throw TimeException
  */
int getLeapDay(int yearsSince1900);

/**
  * Returns a boolean whether or not the indicated year was a leap year.
  * Year values less than 1900 will be treated as 1900+year
  * @param[in ] year An integer representing a Gregorian year (1970, 2000, etc.)
  * @return boolean value indicating whether or not the year was a leap year (true) or not.
  * @throw TimeException
  */
bool isLeapYear(int year);

/**
  * Returns the number of leap days from 1900 up to (but not including) the year specified by yearsSince1900.
  * @param[in ] yearsSince1900 An integer specifying the year, as a number of years since 1900.
  * @return value indicating number of leap days since 1900 up to the year specified by yearsSince1900.
  */
unsigned int leapDays(int yearsSince1900);

/**
  * Returns the number of days in the Gregorian year that corresponds to the given year.
  * @param[in ] year Year value; values less than 1900 will be treated as 1900+year
  * @return an integer containing the number of days in the Gregorian year that corresponds to the given years since 1900.
  * @throw TimeException
  */
int daysPerYear(int year);

/**
  * Returns the number of days in the specified month for the associated Gregorian year.
  *   month values [0, 11]
  *   monthDay values [1, DaysPerMonth(year, month)]
  * @param[in ] year Year value; values less than 1900 will be treated as 1900+year
  * @param[in ] month An integer containing the month in the range of [0-11]
  * @return integer containing the number of days in the specified month for the associated Gregorian year.
  * @throw TimeException
  */
int daysPerMonth(int year, int month);

/**
  * Breaks a time value referenced to a calendar year into individual components
  * @param[in ] time A double containing time referenced to seconds since midnight (00:00) Coordinated Universal Time (UTC), January 1, of a reference year.
  * @param[out] day Computed number of days starting on Jan 1, if ordinal flag is set, days range from [1, n] otherwise [0, n]
  * @param[out] hour Computed number of hours since midnight [0, 23]
  * @param[out] min Computed number of minutes after the hour [0, 59]
  * @param[out] sec Computed number of seconds after the minute [0, 59]
  * @param[out] tenthSec Computed number of tenth of seconds after the second [0, 9]
  * @param[in ] ordinal boolean that indicates reference range for days, true: [1, n], false: [0, n]
  * @return string of time components in the format of "%03i %02i:%02i:%02i"
  * @pre day, hour, min, sec and tenthSec valid params
  */
std::string getTimeComponents(double time,
                              unsigned int* day,
                              unsigned int* hour,
                              unsigned int* min,
                              unsigned int* sec,
                              unsigned int* tenthSec,
                              bool ordinal);

/**
   * Algorithm to get a new time step based on a step up or step down from a given step value
   * Calculates the proper step to use when stepping up or down from a time step
   * value in an application that uses SIMDIS time formats
   * @param faster If true, steps faster, else steps slower
   * @param lastStep Positive step value being used before the next time step is calculated
   * @return Positive step value to use after stepping in the direction indicated by faster, from lastStep
   */
double getNextTimeStep(bool faster, double lastStep);

} // namespace simCore

namespace math
{
// 判断两个double是否相等，按照默认精度
bool doubleEqual(double x, double y);

double rad(double d);

double realDistance(double lat1, double lng1, double lat2, double lng2);
} // namespace math

QString getDatasPath();
QString getIconsPath();

class SmoothCurveGenerator
{
public:
    /**
     * 传入曲线上的点的 list，创建平滑曲线
     * @param points - 曲线上的点
     * @return - 返回使用给定的点创建的 QPainterPath 表示的平滑曲线
     */
    static QPainterPath generateSmoothCurve(const QVector<QPointF>& points);

private:
    /**
     * Solves a tridiagonal system for one of coordinates (x or y)
     * of first Bezier control points.
     * @param result - Solution vector.
     * @param rhs - Right hand side vector.
     * @param n - Size of rhs.
     */
    static void calculateFirstControlPoints(double*& result, const double* rhs, int n);

    /**
     * Calculate control points of the smooth curve using the given knots.
     * @param knots - Points of the given curve.
     * @param firstControlPoints - Store the generated first control points.
     * @param secondControlPoints - Store the generated second control points.
     */
    static void calculateControlPoints(const QVector<QPointF>& knots,
                                       QList<QPointF>* firstControlPoints,
                                       QList<QPointF>* secondControlPoints);
};
namespace color_transfer
{
QColor QColorFromRGBAStr(const QString& rgbaStr);

QString QColorToRGBAStr(const QColor& color);

QColor QColorFromHexStr(const QString& hexStr);
} // namespace color_transfer

#endif /* SIMCORE_TIME_UTILS_H */
