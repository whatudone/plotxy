#include "DataPair.h"
#include "DataManager.h"
#include "PlotXYDemo.h"

#include <QDebug>
#include <QFileInfo>
#include <QPainter>
#include <QTransform>
#include <QUuid>

DataPair::DataPair(QObject* parent)
	: QObject(parent)
{
	m_isDraw = true;
	m_color = Qt::white;
    m_isLineMode = true;
    m_dataWidth = 2;
	m_matchColor = false;

	m_iconDraw = false;

    m_iconRotation = no_rotation;
	m_iconFlipHorz = false;
	m_iconFlipVert = false;
	m_iconColor = Qt::white;
	m_iconSize = QSize(64, 64);

    m_format = format_default;

	m_labelColor = Qt::green;
    m_labelBackground = Qt::transparent;
	m_backTransparent = true;
	m_labelFontSize = 12;
	m_labelFont.setFamily("Microsoft YaHei");

    m_labelFont.setPixelSize(m_labelFontSize);
    m_textPosition = center;
    m_labelPrec_x = 6;
    m_labelPrec_y = 6;
    m_labelText_show = false;
	m_prefix_show = true;
	m_object_show = true;

	m_attr_show = true;
	m_data_show = true;

	m_unit_show = true;

    m_uuid = QUuid::createUuid().toString();
}

DataPair::DataPair(int32_t xEntityID,
                   const QString& xAttrName,
                   const QString& xAttrUnitName,
                   int32_t yEntityID,
                   const QString& yAttrName,
                   const QString& yAttrUnitName)
    : DataPair()

{
    m_entityIDX = xEntityID;
    m_entityIDY = yEntityID;
    m_entity_x = DataManagerInstance->getEntityNameByID(xEntityID);
    m_entity_y = DataManagerInstance->getEntityNameByID(yEntityID);
    m_attr_x = xAttrName;
    m_attr_y = yAttrName;
    m_unit_x = xAttrUnitName;
    m_unit_y = yAttrUnitName;
}

DataPair::~DataPair() {}

QString DataPair::getUuid() const
{
    return m_uuid;
}

void DataPair::setUuid(const QString& uuid)
{
    m_uuid = uuid;
}

QString DataPair::getXEntityAttrPair()
{
    return QString("%1 %2").arg(m_entity_x).arg(m_attr_x);
}

QString DataPair::getYEntityAttrPair()
{
    return QString("%1 %2").arg(m_entity_y).arg(m_attr_y);
}

int32_t DataPair::getEntityIDX() const
{
    return m_entityIDX;
}

void DataPair::setEntityIDX(const int32_t& entityIDX)
{
    m_entityIDX = entityIDX;
    m_entity_x = DataManagerInstance->getEntityNameByID(entityIDX);
}

int32_t DataPair::getEntityIDY() const
{
    return m_entityIDY;
}

void DataPair::setEntityIDY(const int32_t& entityIDY)
{
    m_entityIDY = entityIDY;
    m_entity_y = DataManagerInstance->getEntityNameByID(entityIDY);
}

QString DataPair::getEntity_x() const
{
    return m_entity_x;
}

void DataPair::setEntity_x(const QString& entity_x)
{
    m_entity_x = entity_x;
}

QString DataPair::getEntity_y() const
{
    return m_entity_y;
}

void DataPair::setEntity_y(const QString& entity_y)
{
    m_entity_y = entity_y;
}

QString DataPair::getAttr_x() const
{
    return m_attr_x;
}

void DataPair::setAttr_x(const QString& attr_x)
{
    m_attr_x = attr_x;
}

QString DataPair::getAttr_y() const
{
    return m_attr_y;
}

void DataPair::setAttr_y(const QString& attr_y)
{
    m_attr_y = attr_y;
}

QString DataPair::getUnit_x() const
{
    return m_unit_x;
}

void DataPair::setUnit_x(const QString& unit_x)
{
    m_unit_x = unit_x;
}

QString DataPair::getUnit_y() const
{
    return m_unit_y;
}

void DataPair::setUnit_y(const QString& unit_y)
{
    m_unit_y = unit_y;
}

QString DataPair::getDesc() const
{
    return m_desc;
}

void DataPair::setDesc(const QString& desc)
{
    m_desc = desc;
}

QList<std::tuple<QString, double, QColor>> DataPair::getColorRanges() const
{
    return m_colorInfoList;
}

QString DataPair::colorRangesToString() const
{
    QString colorRanges;
    for(const auto& tuple : m_colorInfoList)
    {
        QString name = std::get<0>(tuple);
        double value = std::get<1>(tuple);
        QColor color = std::get<2>(tuple);
        colorRanges += QString("%1|%2|%3;").arg(name).arg(value).arg(color.name());
    }
    return colorRanges;
}

void DataPair::colorRangesFromString(const QString& str)
{
    if(str.isEmpty())
    {
        return;
    }
    m_colorInfoList.clear();
    QStringList list = str.split(";", QString::SkipEmptyParts);
    for(const auto& colorRange : list)
    {
        QStringList rangeList = colorRange.split("|", QString::SkipEmptyParts);
        if(rangeList.size() == 3)
        {
            QString name = rangeList.at(0);
            double value = rangeList.at(1).toDouble();
            QColor color = QColor(rangeList.at(2));
            m_colorInfoList.append(std::make_tuple(name, value, color));
        }
    }
}

void DataPair::setColorRanges(const QList<std::tuple<QString, double, QColor>>& colorInfoList)
{
    if(m_colorInfoList != colorInfoList)
    {
        m_colorInfoList = colorInfoList;
        emit dataUpdate();
    }
}

QString DataPair::processLabelText(double xData, double yData)
{
    QString labelText;
    if(DataPair::format_default == getTextFormat())
    { //default
        QString prefix_x, prefix_y;
        QString object_x, object_y, attr_x, attr_y;
        QString data_x, data_y, unit_x, unit_y, Left_bracket, right_bracket;

        //考虑仅显示实体名时的操作
        if(this->isObjectShow() && !this->isPrefixShow() && !this->isAttrShow() &&
           !this->isDataShow() && !this->isUnitShow())

        {
            object_x = this->getEntity_x();
            object_y = this->getEntity_y();
            //实体名相同时，仅显示一个实体名
            if(0 == object_x.compare(object_y) && !object_x.isEmpty() && !object_y.isEmpty())
            {
                labelText = object_x;
            }
            else if(object_x.isEmpty() && object_y.isEmpty())
            {
                labelText = "Time";
            }
            else
            {
                labelText = QString("%1\n%2").arg(object_x).arg(object_y);
            }
        }
        else
        {
            if(this->isPrefixShow())
            {
                prefix_x = "X:";
                prefix_y = "Y:";
            }

            if(this->isObjectShow())

            {
                object_x = this->getEntity_x();
                object_y = this->getEntity_y();
            }

            if(this->isAttrShow())
            {
                attr_x = this->getAttr_x();
                attr_y = this->getAttr_y();
            }

            if(this->isDataShow())
            {
                data_x = QString::number(xData, 10, this->getLabelPrecision_x());
                data_y = QString::number(yData, 10, this->getLabelPrecision_y());
                Left_bracket = "(";
                right_bracket = ")";
            }

            if(this->isUnitShow())
            {
                unit_x = this->getUnit_x();
                unit_y = this->getUnit_y();
                Left_bracket = "(";
                right_bracket = ")";
            }
            labelText = QString("%1%2 %3%4%5 %6%7\n%8%9 %10%11%12 %13%14")
                            .arg(prefix_x)
                            .arg(object_x)
                            .arg(attr_x)
                            .arg(Left_bracket)
                            .arg(data_x)
                            .arg(unit_x)
                            .arg(right_bracket)
                            .arg(prefix_y)
                            .arg(object_y)
                            .arg(attr_y)
                            .arg(Left_bracket)
                            .arg(data_y)
                            .arg(unit_y)
                            .arg(right_bracket);
        }
    }
    else if(DataPair::format_custom == this->getTextFormat())
    { //custom
        labelText = this->getCustomText();
    }
    else if(DataPair::format_script == this->getTextFormat()) //script
    {}
    return labelText;
}

QPixmap DataPair::processIcon()
{
    if(!QFileInfo(m_iconName).exists())
    {
        return QPixmap();
    }
    QPixmap pix(m_iconName);
    pix = pix.scaled(iconSize(), Qt::IgnoreAspectRatio);
    QTransform trans;
    auto rotationIndex = iconRotation();
    switch(rotationIndex)
    {
    case DataPair::no_rotation:
        trans.rotate(0);
        break;
    case DataPair::rotation_90:
        trans.rotate(90);
        break;
    case DataPair::rotation_180:
        trans.rotate(180);
        break;
    case DataPair::rotation_270:
        trans.rotate(270);
        break;
    case DataPair::FollowYaw: {
        double yaw = DataManagerInstance->getEntityAttrValueByMaxTime(
            m_entityIDX, "Yaw", PlotXYDemo::getSeconds());
        trans.rotate(yaw);
    }
    break;
    case DataPair::FollowPitch: {
        double pitch = DataManagerInstance->getEntityAttrValueByMaxTime(
            m_entityIDX, "Pitch", PlotXYDemo::getSeconds());
        trans.rotate(pitch);
    }
    break;
    case DataPair::FollowRoll: {
        double roll = DataManagerInstance->getEntityAttrValueByMaxTime(
            m_entityIDX, "Roll", PlotXYDemo::getSeconds());
        trans.rotate(roll);
    }
    break;
    default:
        trans.rotate(0);
        break;
    }
    pix = pix.transformed(trans);
    //水平镜像
    if(iconFlipHorz())
    {
        QImage oldImage = pix.toImage();
        QImage newImage = oldImage.mirrored(true, false);
        pix = QPixmap::fromImage(newImage);
    }
    //垂直镜像
    if(iconFlipVert())

    {
        QImage oldImage = pix.toImage();
        QImage newImage = oldImage.mirrored(false, true);
        pix = QPixmap::fromImage(newImage);
    }
    // 覆盖颜色
    //    QImage image = pix.toImage(); // 将 QPixmap 转换为 QImage

    //    // 遍历图像的每个像素
    //    for(int y = 0; y < image.height(); ++y)
    //    {
    //        for(int x = 0; x < image.width(); ++x)
    //        {
    //            QColor pixelColor = image.pixelColor(x, y);
    //            if(pixelColor == Qt::black)
    //            {
    //                // 将黑色背景像素设置为透明
    //                pixelColor.setAlpha(0);
    //                image.setPixelColor(x, y, pixelColor);
    //            }
    //        }
    //    }

    //    pix = QPixmap::fromImage(image);
    return pix;
}

void DataPair::setWidth(int width)
{
    if(m_dataWidth != width)
    {
        m_dataWidth = width;
        emit dataUpdate();
    }
}

void DataPair::setDraw(bool on)
{
    if(m_isDraw != on)
    {
        m_isDraw = on;
        emit dataUpdate();
    }
}

void DataPair::setColor(const QColor& color)
{
    if(m_color != color)
	{

        m_color = color;
        if(m_matchColor)
        {
            m_iconColor = m_color;
            m_labelColor = m_color;
        }
        emit dataUpdate();
	}
}

void DataPair::setLineMode(bool on)
{
    if(m_isLineMode != on)
    {
        m_isLineMode = on;
        emit dataUpdate();
    }
}

void DataPair::setMatchColor(bool on)
{
    if(m_matchColor != on)
	{
        m_matchColor = on;
        if(m_matchColor)
        {
            m_iconColor = m_color;
            m_labelColor = m_color;
        }
	}
}

void DataPair::setIconDraw(bool on)
{
    if(m_iconDraw != on)
    {
        m_iconDraw = on;
        emit dataUpdate();
    }
}

void DataPair::setIconName(QString name)
{
    if(m_iconName != name)
    {
        m_iconName = name;
        emit dataUpdate();
    }
}

void DataPair::setIconSize(QSize size)
{
    if(m_iconSize != size)
    {
        m_iconSize = size;
        emit dataUpdate();
    }
}

void DataPair::setIconWidth(int w)
{
    if(m_iconSize.width() != w)
    {
        m_iconSize.setWidth(w);
        emit dataUpdate();
    }
}

void DataPair::setIconHeight(int h)
{
    if(m_iconSize.height() != h)
    {
        m_iconSize.setHeight(h);
        emit dataUpdate();
    }
}

void DataPair::setIconRotation(ICON_ROTATION rotate)
{
    if(m_iconRotation != rotate)
    {
        m_iconRotation = rotate;
        emit dataUpdate();
    }
}

void DataPair::setIconFlipHorz(bool on)
{
    if(m_iconFlipHorz != on)
    {
        m_iconFlipHorz = on;
        emit dataUpdate();
    }
}

void DataPair::setIconFlipVert(bool on)
{
    if(m_iconFlipVert != on)
    {
        m_iconFlipVert = on;
        emit dataUpdate();
    }
}

void DataPair::setIconColor(const QColor& color)
{
    if(!m_matchColor && m_iconColor != color)
    {
		m_iconColor = color;
        emit dataUpdate();
    }
}

QPixmap DataPair::rotateIcon(const QPixmap& pix, float angle)
{
	QTransform trans;
	trans.rotate(angle);
	return pix.transformed(trans);
}

void DataPair::setLabelText(const QString& text)
{
    if(m_labelText != text)
    {
        m_labelText = text;
        emit dataUpdate();
    }
}

void DataPair::setTextFormat(TEXT_FROMAT format)
{
    if(m_format != format)
    {
        m_format = format;
        emit dataUpdate();
    }
}

void DataPair::setCustomText(const QString& text)
{
    if(m_customText != text)
    {
        m_customText = text;
        emit dataUpdate();
    }
}

void DataPair::setLabelTextShow(bool show)
{
    if(m_labelText_show != show)
    {
        m_labelText_show = show;
        emit dataUpdate();
    }
}

void DataPair::setLabelColor(const QColor& color)
{
    if(!m_matchColor && m_labelColor != color)
    {
		m_labelColor = color;
        emit dataUpdate();
    }
}

void DataPair::setLabelBackground(const QColor& color)
{
    if(m_labelBackground != color)
    {
        m_labelBackground = color;
        emit dataUpdate();
    }
}

void DataPair::setLabelBackTransparent(bool on)
{
    if(m_backTransparent != on)
    {
        m_backTransparent = on;
        emit dataUpdate();
    }
}

void DataPair::setLabelFont(const QFont& font)
{
    if(m_labelFont != font)
    {
        m_labelFont = font;
        emit dataUpdate();
    }
}

void DataPair::setLabelFontSize(int size)
{
    if(m_labelFontSize != size)
    {
        m_labelFontSize = size;
        m_labelFont.setPixelSize(size);
        emit dataUpdate();
    }
}

void DataPair::setLabelPosition(TEXT_POSITION pos)
{
    if(m_textPosition != pos)
    {
        m_textPosition = pos;
        emit dataUpdate();
    }
}

void DataPair::setLabelPrecision_x(int prec)
{
    if(m_labelPrec_x != prec)
    {
        m_labelPrec_x = prec;
        emit dataUpdate();
    }
}

void DataPair::setLabelPrecision_y(int prec)
{
    if(m_labelPrec_y != prec)
    {
        m_labelPrec_y = prec;
        emit dataUpdate();
    }
}

void DataPair::setPrefixShow(bool show)
{
    if(m_prefix_show != show)
    {
        m_prefix_show = show;
        emit dataUpdate();
    }
}

void DataPair::setObjectShow(bool show)
{
    if(m_object_show != show)
    {
        m_object_show = show;
        emit dataUpdate();
    }
}

void DataPair::setAttrShow(bool show)
{
    if(m_attr_show != show)
    {
        m_attr_show = show;
        emit dataUpdate();
    }
}

void DataPair::setDataShow(bool show)
{
    if(m_data_show != show)
    {
        m_data_show = show;
        emit dataUpdate();
    }
}

void DataPair::setUnitShow(bool show)
{
    if(m_unit_show != show)
    {
        m_unit_show = show;
        emit dataUpdate();
    }
}

Qt::Alignment DataPair::getLabelTextAlign()
{
    Qt::Alignment flag;
    switch(getLabelPosition())
    {
    case DataPair::left_top: //left-top
        flag = Qt::AlignTop | Qt::AlignLeft;
        break;
    case DataPair::top: //top
        flag = Qt::AlignTop;
        break;
    case DataPair::right_top: //right-top
        flag = Qt::AlignTop | Qt::AlignRight;
        break;
    case DataPair::left: //left
        flag = Qt::AlignLeft;
        break;
    case DataPair::center: //center
        flag = Qt::AlignCenter;
        break;
    case DataPair::right: //right
        flag = Qt::AlignRight;
        break;
    case DataPair::left_bottom: //left-bottom
        flag = Qt::AlignBottom | Qt::AlignLeft;
        break;
    case DataPair::bottom: //bottom
        flag = Qt::AlignBottom;
        break;
    case DataPair::right_bottom: //right-bottom
        flag = Qt::AlignBottom | Qt::AlignRight;
        break;
    default: //right
        flag = Qt::AlignRight;
        break;
    }
    return flag;
}

QPointF DataPair::processLabelPosition(const QPointF& lastPointPosition, const QString& labelText)
{
    QFont labelFont = getLabelFont();

    QFontMetricsF fm(labelFont);
    auto labelSize = fm.size(Qt::TextWordWrap, labelText);
    double defaultSpace = 50.0;
    QPointF labelPos;
    switch(getLabelPosition())
    {
    case DataPair::left_top: //left-top
        labelPos.setX(lastPointPosition.x() - defaultSpace - labelSize.width());
        labelPos.setY(lastPointPosition.y() - defaultSpace - labelSize.height());
        break;
    case DataPair::top: //top
        labelPos.setX(lastPointPosition.x() - labelSize.width() / 2);
        labelPos.setY(lastPointPosition.y() - defaultSpace - labelSize.height());
        break;
    case DataPair::right_top: //right-top
        labelPos.setX(lastPointPosition.x() + defaultSpace);
        labelPos.setY(lastPointPosition.y() - defaultSpace - labelSize.height());
        break;
    case DataPair::left: //left
        labelPos.setX(lastPointPosition.x() - defaultSpace - labelSize.width());
        labelPos.setY(lastPointPosition.y() - labelSize.height() / 2);
        break;
    case DataPair::center: //center
        labelPos.setX(lastPointPosition.x() - labelSize.width() / 2);
        labelPos.setY(lastPointPosition.y() - labelSize.height() / 2);
        break;
    case DataPair::right: //right
        labelPos.setX(lastPointPosition.x() + defaultSpace);
        labelPos.setY(lastPointPosition.y() - labelSize.height() / 2);
        break;
    case DataPair::left_bottom: //left-bottom
        labelPos.setX(lastPointPosition.x() - defaultSpace - labelSize.width());
        labelPos.setY(lastPointPosition.y() + defaultSpace);
        break;
    case DataPair::bottom: //bottom
        labelPos.setX(lastPointPosition.x() - labelSize.width() / 2);
        labelPos.setY(lastPointPosition.y() + defaultSpace);
        break;
    case DataPair::right_bottom: //right-bottom
        labelPos.setX(lastPointPosition.x() + defaultSpace);
        labelPos.setY(lastPointPosition.y() + defaultSpace);
        break;
    default: //right
        labelPos.setX(lastPointPosition.x() + defaultSpace);
        labelPos.setY(lastPointPosition.y() - labelSize.height() / 2);
        break;
    }
    return labelPos;
}

QColor DataPair::getLabelSecColor() const
{
    return m_labelSecColor;
}

void DataPair::setLabelSecColor(const QColor& labelSecColor)
{
    if(m_labelSecColor != labelSecColor)
    {
        m_labelSecColor = labelSecColor;
        emit dataUpdate();
    }
}

bool DataPair::getIsStippleEnable() const
{
    return m_isStippleEnable;
}

void DataPair::setIsStippleEnable(bool isStippleEnable)
{
    if(m_isStippleEnable != isStippleEnable)
    {
        m_isStippleEnable = isStippleEnable;
        emit dataUpdate();
    }
}

Qt::PenStyle DataPair::getStipplePattern() const
{
    return m_stipplePattern;
}

void DataPair::setStipplePattern(const Qt::PenStyle& stipplePattern)
{
    if(m_stipplePattern != stipplePattern)
    {
        m_stipplePattern = stipplePattern;
        emit dataUpdate();
    }
}

QString DataPair::getCustomPattern() const
{
    return m_customPattern;
}

void DataPair::setCustomPattern(const QString& customPattern)
{
    if(m_customPattern != customPattern)
    {
        m_customPattern = customPattern;
        emit dataUpdate();
    }
}

int DataPair::getCustomPatternFactor() const
{
    return m_customPatternFactor;
}

void DataPair::setCustomPatternFactor(double customPatternFactor)
{
    if(m_customPatternFactor != customPatternFactor)
    {
        m_customPatternFactor = customPatternFactor;
        emit dataUpdate();
    }
}

bool DataPair::getColorRangeEnable() const
{
    return m_colorRangeEnable;
}

void DataPair::setColorRangeEnable(bool colorRangeEnable)
{
    if(m_colorRangeEnable != colorRangeEnable)
    {
        m_colorRangeEnable = colorRangeEnable;
        emit dataUpdate();
    }
}

QColor DataPair::getColorRangeDefaultColor() const
{
    return m_colorRangeDefaultColor;
}

void DataPair::setColorRangeDefaultColor(const QColor& colorRangeDefaultColor)
{
    if(m_colorRangeDefaultColor != colorRangeDefaultColor)
    {
        m_colorRangeDefaultColor = colorRangeDefaultColor;
        emit dataUpdate();
    }
}

DataPair::ColorRangeMode DataPair::getColorRangeMode() const
{
    return m_colorRangeMode;
}

void DataPair::setColorRangeMode(const ColorRangeMode& colorRangeMode)
{
    if(m_colorRangeMode != colorRangeMode)
    {
        m_colorRangeMode = colorRangeMode;
        emit dataUpdate();
    }
}
