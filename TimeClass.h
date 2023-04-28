#pragma once
#include "constdef.h"

///Time comparison values
enum TimeCompVal
{
	TCV_LESS,
	TCV_EQUAL,
	TCV_GREATER
};
class Seconds
{
public:
    Seconds()
        : m_seconds(0)
        , m_fraction(0)
    {}
    Seconds(double time)
    {
        convert(time);
    }
    Seconds(int64_t sec, int frac)
        : m_seconds(sec)
        , m_fraction(frac)
    {
        fix();
    }
    Seconds(int64_t sec, double frac)
    {
        convert(frac);
        m_seconds += sec;
    }
    Seconds(const Seconds& time)
        : m_seconds(time.m_seconds)
        , m_fraction(time.m_fraction)
    {}
	~Seconds();

    int64_t getSeconds() const
    {
        return m_seconds;
    }
    double getFraction() const
    {
        return (m_fraction * OUTPUT_CONV_FACTOR);
    }
    int getFractionLong() const
    {
        return m_fraction;
    }

    operator double() const
    {
        return convert();
    }
    double Double() const
    {
        return convert();
    }

    Seconds& operator=(const Seconds& time);
    Seconds& operator=(double time)
    {
        convert(time);
        return *this;
    }
    Seconds& operator++()
    {
        m_seconds += 1;
        return *this;
    }
    Seconds& operator--()
    {
        m_seconds -= 1;
        return *this;
    }
    Seconds& operator+=(const Seconds& t)
    {
        *this = *this + t;
        return *this;
    }
    Seconds& operator-=(const Seconds& t)
    {
        *this = *this - t;
        return *this;
    }
    Seconds& operator*=(const Seconds& t)
    {
        *this = *this * t;
        return *this;
    }
    Seconds& operator/=(const Seconds& t)
    {
        *this = *this / t;
        return *this;
    }
    Seconds& scale(double scl)
    {
        *this *= (Seconds)scl;
        return *this;
    }
    Seconds operator+(const Seconds& t) const;
    Seconds operator-(const Seconds& t) const;
    Seconds operator*(const Seconds& t) const;
    Seconds operator/(const Seconds& t) const;
	TimeCompVal compare(const Seconds& time) const;

	void convert(double dtime);
	double convert() const;
	void fix();

private:
	int64_t m_seconds;
    int m_fraction;
};

class TimeStamp
{
public:
	TimeStamp();
	TimeStamp(int refYear, const Seconds& secs);
    int referenceYear() const
    {
        return m_referenceYear;
    }
    Seconds secondsSinceRefYear() const
    {
        return m_secondsSinceRefYear;
    }
	Seconds secondsSinceRefYear(int refYear) const;
	void setTime(int refYear, const Seconds& secs);

    TimeStamp& operator=(const TimeStamp& time);
    TimeStamp& operator++()
    {
        m_secondsSinceRefYear += Seconds(1, 0);
        fix_();
        return *this;
    }
    TimeStamp& operator--()
    {
        m_secondsSinceRefYear -= Seconds(1, 0);
        fix_();
        return *this;
    }
    TimeStamp& operator+=(const Seconds& t)
    {
        *this = (*this + t);
        return *this;
    }
    TimeStamp& operator-=(const Seconds& t)
    {
        *this = (*this - t);
        return *this;
    }
    Seconds operator-(const TimeStamp& t) const;
    TimeStamp operator-(const Seconds& s) const;
    TimeStamp operator+(const Seconds& s) const;
    bool operator==(const TimeStamp& t) const
    {
        return (compare_(t) == TCV_EQUAL);
    }
    bool operator!=(const TimeStamp& t) const
    {
        return (compare_(t) != TCV_EQUAL);
    }
    bool operator<(const TimeStamp& t) const
    {
        return (compare_(t) == TCV_LESS);
    }
    bool operator>(const TimeStamp& t) const
    {
        return (compare_(t) == TCV_GREATER);
    }
    bool operator<=(const TimeStamp& t) const
    {
        return (compare_(t) != TCV_GREATER);
    }
    bool operator>=(const TimeStamp& t) const
    {
        return (compare_(t) != TCV_LESS);
    }

	/// Breaks a TimeStamp value into individual components relative to the TimeStamp's reference year.
	/**
	* @param[out] day Computed number of days in the TimeStamp's reference year [0, 364/365] 365 only on leap years
	* @param[out] hour Computed number of hours since midnight [0, 23]
	* @param[out] min Computed number of minutes after the hour [0, 59]
	* @param[out] sec Computed number of seconds after the minute [0, 59]
	*/
    void getTimeComponents(unsigned int& day,
                           unsigned int& hour,
                           unsigned int& min,
                           unsigned int& sec) const;
    void
    getTimeComponents(unsigned int& day, unsigned int& hour, unsigned int& min, float& fsec) const;

private:
	int m_referenceYear;
	Seconds m_secondsSinceRefYear;
	int fixRequired_() const;
	void fix_();
	TimeCompVal compare_(const TimeStamp& time) const;
};

static const Seconds ZERO_SECONDS;
static const int INFINITE_TIME_YEAR = 16384;
static const TimeStamp INFINITE_TIME_STAMP(INFINITE_TIME_YEAR, ZERO_SECONDS);
static const int MIN_TIME_YEAR = 1970;
static const TimeStamp MIN_TIME_STAMP(MIN_TIME_YEAR, ZERO_SECONDS);
static const int MAX_TIME_YEAR = 2200;
static const TimeStamp MAX_TIME_STAMP(MAX_TIME_YEAR, Seconds(365 * SECPERDAY - 1, 999999));

class OrdinalTimeFormatter
{
public:
    static QString toString(double secSinceRefYear, int refYear);
    static bool isValidIntNumber(const QString& str);
    static bool isValidDoubleNumber(const QString& str);
    static bool canConvert(const QString& timeString, QStringList& listTime);
    static void convertToTime(
        QString timeString, int& days, int& refYear, int& hour, int& minute, double& seconds);
    static double convertToSeconds(const QString& timeString, int refYear);
    static void secondsConvertToTime(double secSinceRefYear,
                                     int refYear,
                                     unsigned int& days,
                                     unsigned int& hour,
                                     unsigned int& minute,
                                     float& fsec);
    static double getSecondsFromTimeStr(const QString& str, int32_t refYear);
};
