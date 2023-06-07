#ifndef CONST_DEF_H_
#define CONST_DEF_H_

#include <QColor>
#include <QString>

enum PlotType : int32_t
{
    Type_PlotUnknown = -1,
    Type_PlotScatter,
	Type_PlotAScope,
	Type_PlotRTI,
	Type_PlotText,
	Type_PlotLight,
	Type_PlotBar,
	Type_PlotDial,
	Type_PlotAttitude,
	Type_PlotPolar,
	Type_PlotTrack,
	Type_PlotDoppler
};

static const double OUTPUT_CONV_FACTOR = 1e-03;
static const double INPUT_CONV_FACTOR_PREC_LIMIT = 1e+03;
static const double INPUT_ROUND_UP_VALUE = 5e-4;

// Constants for various time representations
static const int SECPERMIN = 60; /**< # of seconds per minute */
static const int SECPERHOUR = 3600; /**< # of seconds per hour */
static const int SECPERDAY = 86400; /**< # of seconds per day */
static const int MINPERHOUR = 60; /**< # of minutes per hour */
static const int HOURPERDAY = 24; /**< # of hours per day */
static const int MONPERYEAR = 12; /**< # of months per year */

// Astronomical time constants
static const double JULIAN_DATE_J1900 =
    2415021.0; /**<  Julian date of the standard epoch of J1900.0 */
static const double JULIAN_DATE_J1970 =
    2440588.0; /**<  Julian date of the standard epoch of J1970.0 */
static const double JULIAN_DATE_J2000 =
    2451545.0; /**<  Julian date of the standard epoch of J2000.0 */

/** Number of days per month, in normal years [0], and in leap years [1] */
static const unsigned int DAYS_IN_MONTHS[2][13] = {
	/* 0, jan feb mar apr may jun jul aug sep oct nov dec */
	/* number of days per month for non-leap years */
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	/* number of days per month for leap years */
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

/** Cumulative number of days per year at the end of each month, in normal years [0], and in leap years [1] */
static const unsigned int DAYS_IN_YEAR[2][14] = {
	/* empty, 0, jan feb mar apr may jun jul aug sep oct nov dec */
	/* summation of days per month for non-leap years */
    {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
	/* summation of days per month for leap years */
    {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}};

/** Ordered list of all month names in English */
static const std::string MONTH_NAME[] = {"January",
                                         "February",
                                         "March",
                                         "April",
                                         "May",
                                         "June",
                                         "July",
                                         "August",
                                         "September",
                                         "October",
                                         "November",
                                         "December"};

///Based off abbreviations in __DATE__
static const std::string ABBREV_MONTH_NAME[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

///Monday listed first for convenience of getWeekday(), Julian Date 0 occurred on a Monday
static const std::string WEEKDAY_NAME[] = {
	"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

///Monday listed first for convenience of getWeekday(), Julian Date 0 occurred on a Monday
static const std::string ABBREV_WEEKDAY_NAME[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

///Time formats
enum TimeFormat
{
	TIMEFORMAT_SECONDS = 1, ///< SS.sssss
    TIMEFORMAT_MINUTES, ///< MM:SS.sssss
    TIMEFORMAT_HOURS, ///< HH:MM:SS.sssss
    TIMEFORMAT_ORDINAL, ///< DDD YYYY HH:MM:SS.sssss
    TIMEFORMAT_MONTHDAY, ///< MON MDY YYYY HH:MM:SS.sssss
    TIMEFORMAT_DTG, ///< MDYHHMM:SS.SSS Z MON YYYY
    TIMEFORMAT_ISO8601 ///< YYYY-MM-DD or YYYY-MM-DDTHH:MM:SS.sssZ, with optional [.sss]
};

/// Time sources
enum TimeZone
{
	TIMEZONE_UTC = 0,
	TIMEZONE_LOCAL
};

///Time directions
enum TimeDirection
{
	FORWARD = 0,
	REVERSE = 1,
	STOP = 2
};

///Real time modes
enum RealTimeMode
{
    NOT_REALTIME = 0, ///< Non real-time mode
    REALTIME = 1 ///< Real-time mode
};

//Grid Style
enum GridStyle
{
	SOLIDLINE = 0,
	DASHLINE,
	DOTLINE,
	DASHDOTLINE
};

//Grid Density
enum GridDensity
{
	LESS = 4,
	NORMAL = 8,
	MORE = 10
};

// 鼠标操作模式，每种模式对应一种鼠标控制行为，属于互斥行为
enum class MouseMode : uint8_t
{
    SelectPlot = 0, //鼠标切换PLOT
    Pan, //移动
    CenterPlot, //居中
    Zoom, //整体缩放
    BoxZoom, //框选缩放
    MeasureDistance, //测距
    CreatePlot, // 创建
    MovePlot // 移动+缩放

};

enum AxisType
{
    xAxis = 0,
    yAxis,
    xAxis2,
    yAxis2
};

struct DialColorInfo
{
    double start;
    double end;
    QColor clr;
    QColor outline;
    int width;
};

struct EventSettings
{
    QString m_eventFontFamily = "微软雅黑";
    int32_t m_eventFontSize = 8;
    QColor m_eventColor = Qt::red;
    QString m_eventStyle = "Small X";
    bool m_isIncludeTag = true;
    // 事件类型
    QString m_type;
    int32_t m_entityID;
    QString m_entityName;
    bool operator==(const EventSettings& e)
    {
        return (m_entityID == e.m_entityID) && (m_type == e.m_type);
    }
};

struct GenericData
{
    // 事件名称
    QString m_name;
    // 事件发生时间，以s为单位
    double m_relativeTime;
    // 时间偏移
    int32_t m_timeOffset;
};

enum OpsStatus
{
    IN_PORT,
    TRANSIT
};

enum Alliance
{
    M_BLUE = 0,
    M_RED,
    M_WHITE,
    M_GREEN,
    M_YELLOW,
    M_ORANE,
    M_BLACK,
    M_PURPLE,
    M_CYAN,
    M_BROWN,
    M_TOTAL
};

enum Operation_medium
{
    ME_AIR,
    ME_SURFACE,
    ME_SPACE,
    ME_LAND,
    ME_SUBSURFACE,
    ME_UNKOWN
};

enum IconType
{
    SURFACE1,
    SURFACE2,
    SURFACE3,
    SURFACE4,
    SURFACE5,
    SURFACE6,
    SURFACE7,
    SURFACE8,
    SURFACE9,
    SURFACE10,
    SURFACE11,

};

#pragma pack(push, 1)
struct Z_SendHeader
{
    unsigned short iMessageType; //信息标识
    double time; //时间 从0开始 的小时数
    int ilength; //本包信息长度
};

#define NLENTH 32

struct PlatInfoDataExcect
{
    double time;
    char platName[NLENTH]; //平台名称
    int uID; //平台ID
    double fuel; //油料
    double dammager; //生命值
    OpsStatus OpsStatus; //状态
    Alliance Alliance; //联盟方
    Operation_medium Operation_medium; //运动介质
    IconType Icon_Type; //类型
    float speed; //单位米/S
    float bearing; //航向度
    double lat; //经
    double lng; //纬度
    double Alt; //高度
    double CW; //首向角
    double pitch;
    double roll;
    char BasePlatName[NLENTH]; //类型名称
    char HullName[NLENTH]; //舷号
    char cStandBy[NLENTH]; //备用字
    int iConNUm; //标号
    int iVisible; //是否显示
    double maxSpeed; //最大速度
};

struct GenericDataExcect
{
    int uID; //平台ID
    char platName[NLENTH]; //平台名称
    double relativeTime; //偏移时间
    int timeOffset; //事件类型（-1）
};
#pragma pack(pop)

#endif // _CONST_DEF_H_
