#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <limits>
#include <locale>
#include <time.h>
#include <QStringList>

#include "TimeClass.h"
#include "Utils.h"

static const unsigned int SECPERYEAR = SECPERDAY * 365; // seconds in a standard non-leap year: 31536000
static const int MAX_FIX = MAX_TIME_YEAR - MIN_TIME_YEAR + 1;

Seconds::~Seconds()
{
}

void Seconds::convert(double dtime)
{
	// maximum storage value of class is INT_MAX
	if (dtime >= LLONG_MAX)
	{
		m_seconds = LLONG_MAX;
		m_fraction = 0;
		return;
	}
	// minimum storage value of class is INT_MIN
	if (dtime <= LLONG_MIN)
	{
		m_seconds = LLONG_MIN;
		m_fraction = 0;
		return;
	}

	m_seconds = static_cast<int64_t>(dtime);
	m_fraction = (dtime < 0) ?
		static_cast<int>((dtime - m_seconds - INPUT_ROUND_UP_VALUE) * INPUT_CONV_FACTOR_PREC_LIMIT) :
		static_cast<int>((dtime - m_seconds + INPUT_ROUND_UP_VALUE) * INPUT_CONV_FACTOR_PREC_LIMIT);
	fix();
}

double Seconds::convert() const
{
	return (OUTPUT_CONV_FACTOR * m_fraction) + m_seconds;
}

void Seconds::fix()
{
	if ((m_fraction >= static_cast<int>(INPUT_CONV_FACTOR_PREC_LIMIT)) ||
		(m_fraction <= -static_cast<int>(INPUT_CONV_FACTOR_PREC_LIMIT)))
	{
		m_seconds += (m_fraction / static_cast<int>(INPUT_CONV_FACTOR_PREC_LIMIT));
		m_fraction = m_fraction % static_cast<int>(INPUT_CONV_FACTOR_PREC_LIMIT);
	}

	if (m_fraction < 0)
	{
		m_seconds--;
		m_fraction += static_cast<int>(INPUT_CONV_FACTOR_PREC_LIMIT);
	}
}

Seconds& Seconds::operator = (const Seconds& time)
{
	if (this != &time)
	{
		m_seconds = time.m_seconds;
		m_fraction = time.m_fraction;
	}
	return *this;
}

Seconds Seconds::operator + (const Seconds& time) const
{
	return Seconds(m_seconds + time.m_seconds, m_fraction + time.m_fraction);
}

Seconds Seconds::operator - (const Seconds& time) const
{
	return Seconds(m_seconds - time.m_seconds, m_fraction - time.m_fraction);
}

Seconds Seconds::operator * (const Seconds& time) const
{
	if (time == ZERO_SECONDS || (m_seconds == 0 && m_fraction == 0))
		return ZERO_SECONDS;

	return Seconds(Double() * time.Double());
}

Seconds Seconds::operator / (const Seconds& time) const
{
	// catch divide by zero
	if (time == ZERO_SECONDS)
		return ZERO_SECONDS;

	return Seconds(Double() / time.Double());
}

//////////////////////////////////////////////////////////////////////////

TimeCompVal Seconds::compare(const Seconds& time) const
{
	if (m_seconds > time.m_seconds)
		return TCV_GREATER;
	if (m_seconds < time.m_seconds)
		return TCV_LESS;
	if (m_fraction > (time.m_fraction + 1))
		return TCV_GREATER;
	if ((m_fraction + 1) < time.m_fraction)
		return TCV_LESS;
	return TCV_EQUAL;
}

TimeStamp::TimeStamp()
	: m_referenceYear(simCore::currentYear())
{
	fix_();
}

TimeStamp::TimeStamp(int refYear, const Seconds& secondsSinceRefYear)
{
	setTime(refYear, secondsSinceRefYear);
}

int TimeStamp::fixRequired_() const
{
	const double secondsSinceRefYear = m_secondsSinceRefYear.Double();
	if (secondsSinceRefYear < 0.)
		return 1;
	const double secondsInRefYear = SECPERDAY * simCore::daysPerYear(m_referenceYear);
	return (secondsSinceRefYear < secondsInRefYear) ? 0 : 2;
}

void TimeStamp::fix_()
{
	if (m_referenceYear == INFINITE_TIME_YEAR)
	{
		m_secondsSinceRefYear = ZERO_SECONDS;
		return;
	}
	if (m_referenceYear < MIN_TIME_YEAR)
	{
		*this = MIN_TIME_STAMP;
		return;
	}
	if (m_referenceYear > MAX_TIME_YEAR)
	{
		*this = MAX_TIME_STAMP;
		return;
	}
	if (m_secondsSinceRefYear == ZERO_SECONDS)
		return;
	if (0 == fixRequired_())
		return;

	// treat all intervening years as non-leap years
	const double years = m_secondsSinceRefYear.getSeconds() >= 0 ? floor(m_secondsSinceRefYear.Double() / SECPERYEAR) : ceil(m_secondsSinceRefYear.Double() / SECPERYEAR);
	if (fabs(years) > MAX_FIX)
	{
		if (years < 0)
		{
			*this = MIN_TIME_STAMP;
			return;
		}
		*this = MAX_TIME_STAMP;
		return;
	}

	const int64_t seconds = static_cast<int64_t>(years * SECPERYEAR);
	m_secondsSinceRefYear -= Seconds(seconds, 0);
	int newReferenceYear = m_referenceYear + static_cast<int>(years);

	const int leapDays = simCore::leapDays(newReferenceYear - 1900) - simCore::leapDays(m_referenceYear - 1900);
	m_secondsSinceRefYear -= Seconds(leapDays * SECPERDAY, 0);

	// leap days calculation may result in reference year needing to be corrected +/- 1
	const int validFixResult = fixRequired_();
	if (validFixResult == 1)
	{
		newReferenceYear--;
		const int secondsInRefYear = SECPERDAY * simCore::daysPerYear(newReferenceYear);
		m_secondsSinceRefYear += Seconds(secondsInRefYear, 0);
	}
	else if (validFixResult == 2)
	{
		const int secondsInRefYear = SECPERDAY * simCore::daysPerYear(newReferenceYear);
		newReferenceYear++;
		m_secondsSinceRefYear -= Seconds(secondsInRefYear, 0);
	}
	if (newReferenceYear < MIN_TIME_YEAR)
	{
		*this = MIN_TIME_STAMP;
		return;
	}
	if (newReferenceYear > MAX_TIME_YEAR)
	{
		*this = MAX_TIME_STAMP;
		return;
	}
	m_referenceYear = newReferenceYear;
	return;
}

Seconds TimeStamp::secondsSinceRefYear(int refYear) const
{
	if (m_referenceYear == refYear)
		return m_secondsSinceRefYear;
	const TimeStamp ref(refYear, ZERO_SECONDS);
	return *this - ref;
}

void TimeStamp::setTime(int refYear, const Seconds& secondsSinceRefYear)
{
	m_referenceYear = refYear;
	m_secondsSinceRefYear = secondsSinceRefYear;
	fix_();
}

TimeStamp& TimeStamp::operator = (const TimeStamp& time)
{
	if (this != &time)
	{
		m_referenceYear = time.m_referenceYear;
		m_secondsSinceRefYear = time.m_secondsSinceRefYear;
	}
	return *this;
}

Seconds TimeStamp::operator - (const TimeStamp& t) const
{
	// If either year presents a infinity return with zero seconds
	if ((m_referenceYear == INFINITE_TIME_YEAR) || (t.m_referenceYear == INFINITE_TIME_YEAR))
		return ZERO_SECONDS;

	const int yearDifference = (m_referenceYear - t.m_referenceYear);
	if (std::abs(yearDifference) > (MAX_TIME_STAMP.referenceYear() - MIN_TIME_STAMP.referenceYear()))
	{
		return ZERO_SECONDS;
	}

	Seconds secondsValue;

	if (yearDifference > 0)
	{
		secondsValue = Seconds(SECPERDAY * simCore::daysPerYear(t.m_referenceYear), 0) - t.m_secondsSinceRefYear;
		for (auto year = t.m_referenceYear + 1; year < m_referenceYear; ++year)
			secondsValue += Seconds(SECPERDAY * simCore::daysPerYear(year), 0);
		secondsValue += m_secondsSinceRefYear;
	}
	else if (yearDifference < 0)
	{
		secondsValue = (Seconds(-1, 0) * t.m_secondsSinceRefYear);
		for (auto year = t.m_referenceYear - 1; year > m_referenceYear; --year)
			secondsValue -= Seconds(SECPERDAY * simCore::daysPerYear(year), 0);
		secondsValue -= (Seconds(SECPERDAY * simCore::daysPerYear(m_referenceYear), 0) - m_secondsSinceRefYear);
	}
	else
	{
		secondsValue = m_secondsSinceRefYear - t.m_secondsSinceRefYear;
	}

	return secondsValue;
}

TimeStamp TimeStamp::operator - (const Seconds& s) const
{
	return TimeStamp(m_referenceYear, m_secondsSinceRefYear - s);
}

TimeStamp TimeStamp::operator + (const Seconds& s) const
{
	return TimeStamp(m_referenceYear, m_secondsSinceRefYear + s);
}

TimeCompVal TimeStamp::compare_(const TimeStamp& time) const
{
	if (m_referenceYear > time.m_referenceYear)
		return TCV_GREATER;
	if (m_referenceYear < time.m_referenceYear)
		return TCV_LESS;
	return m_secondsSinceRefYear.compare(time.m_secondsSinceRefYear);
}

double getFactor(const TimeStamp &lowVal, const TimeStamp &exactVal, const TimeStamp &highVal)
{
	// Perform bounds check and prevent divide by zero
	if (exactVal <= lowVal) return 0.;
	if (exactVal >= highVal || (highVal - lowVal) == 0) return 1.;
	return (exactVal - lowVal) / (highVal - lowVal);
}

void TimeStamp::getTimeComponents(unsigned int& day, unsigned int& hour, unsigned int& min, unsigned int& sec) const
{
	int64_t time = secondsSinceRefYear().getSeconds();
	// TimeStamp fix() always normalizes Seconds to a positive number.
	day = static_cast<unsigned int>(time / SECPERDAY);

	time -= (day*SECPERDAY);
	hour = static_cast<unsigned int>(time / SECPERHOUR);

	time -= (hour*SECPERHOUR);
	min = static_cast<unsigned int>(time / SECPERMIN);

	time -= (min*SECPERMIN);
	sec = static_cast<unsigned int>(time);
}

void TimeStamp::getTimeComponents(unsigned int& day, unsigned int& hour, unsigned int& min, float& fsec) const
{
	int64_t time = secondsSinceRefYear().getSeconds();
	// TimeStamp fix() always normalizes Seconds to a positive number.
	day = static_cast<unsigned int>(time / SECPERDAY);

	time -= (day*SECPERDAY);
	hour = static_cast<unsigned int>(time / SECPERHOUR);

	time -= (hour*SECPERHOUR);
	min = static_cast<unsigned int>(time / SECPERMIN);

	time -= (min*SECPERMIN);
	fsec = static_cast<unsigned int>(time) + secondsSinceRefYear().getFraction();
}

//////////////////////////////////////////////////////////////////////////

QString OrdinalTimeFormatter::toString(double secSinceRefYear, int refYear)
{
	QString timeString = nullptr;
	unsigned int days, hour, minute;
	float seconds;
	secondsConvertToTime(secSinceRefYear, refYear, days, hour, minute, seconds);
	timeString = QString("%1 %2 %3:%4:%5").
		arg(days, 3, 10, QChar('0')).
		arg(refYear).
		arg(hour, 2, 10, QChar('0')).
		arg(minute, 2, 10, QChar('0')).
		arg(seconds);
	return timeString;
}

bool OrdinalTimeFormatter::isValidIntNumber(QString str)
{
	bool ok;
	str.toInt(&ok);
	return ok;
}

bool OrdinalTimeFormatter::canConvert(const QString & timeString, QStringList &listTime)
{
	///< DDD YYYY HH:MM:SS.sssss
	QStringList strDate = timeString.split(' ', QString::SkipEmptyParts);
	if (strDate.size() != 3 || 
		isValidIntNumber(strDate.at(0)) == false ||
		isValidIntNumber(strDate.at(1)) == false)
		return false;

	listTime << strDate.at(0) << strDate.at(1);
	QStringList strTime = strDate.at(2).split(':');
	if (strTime.size() != 3)
	{
		return false;
	}

	listTime << strTime;
	return true;
}

void OrdinalTimeFormatter::convertToTime(QString timeString, int & days, int & referenceYear, int & hour, int & minute, double & seconds)
{
	QStringList timeList;
	if (canConvert(timeString, timeList))
	{
		days = timeList.at(0).toInt();
		referenceYear = timeList.at(1).toInt();
		hour = timeList.at(2).toInt();
		minute = timeList.at(3).toInt();
		seconds = timeList.at(4).toDouble();
	}
}

double OrdinalTimeFormatter::convertToSeconds(QString timeString, int referenceYear)
{
	int days, year, hour, minute;
	double seconds;
	double secondsSinceReferenceYear = 0.0;

	convertToTime(timeString, days, year, hour, minute, seconds);
	
	days += simCore::leapDays(year - 1900) - simCore::leapDays(referenceYear - 1900);
	secondsSinceReferenceYear = days * SECPERDAY + hour *SECPERHOUR + minute * SECPERMIN + seconds;
	return secondsSinceReferenceYear;
}

void OrdinalTimeFormatter::secondsConvertToTime(double secSinceRefYear, int refYear, unsigned int &days, unsigned int &hour, unsigned int &minute, float &fsec)
{
	Seconds secs(secSinceRefYear);
	TimeStamp timestamp(refYear, secs);
	timestamp.getTimeComponents(days, hour, minute, fsec);
}
