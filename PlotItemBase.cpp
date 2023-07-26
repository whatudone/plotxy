/* * @filename: PlotItemBase.cpp
* @brief: PlotItemBase PlotItemBase
* @author: zhameng
* @create time: 2022-09-21
*  */

#include "PlotItemBase.h"
#include "DataManager.h"
#include "PlotXYDemo.h"
#include "Utils.h"

#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QPushButton>

PlotItemBase::PlotItemBase(QWidget* parent)
    : QWidget(parent)
    , m_isHorizonBar(false)
    , m_barLeftPadding(0.0)
    , m_barRightPadding(0.0)
{
    ui.setupUi(this);

    m_position = QPoint(0, 0);
    m_width = 0;
    m_height = 0;
    m_bVisible = true;
    m_coordBgn_x = 0;
    m_coordEnd_x = 100;
    m_coordBgn_y = 0;
    m_coordEnd_y = 100;
    m_xPrecision = 0;
    m_yPrecision = 0;
    m_xRate = 1.0;
    m_yRate = 1.0;

    m_outerFillColor = Qt::transparent;
    m_outlineColor = Qt::transparent;
    m_outlineWidth = 5;

    m_horzGrids = 4;
    m_vertGrids = 4;
    m_axisWidth = 1;
    m_gridWidth = 1;
    m_axisColor = Qt::white;
    m_gridColor = Qt::white;
    m_gridVisible = true;
    m_xTickLabelColor = Qt::white;
    m_xTickLabelFontSize = 20;
    m_xTickLabelFont.setFamily("Microsoft YaHei");
    m_xTickLabelFont.setPixelSize(m_xTickLabelFontSize);
    m_yTickLabelColor = Qt::white;
    m_yTickLabelFontSize = 20;
    m_yTickLabelFont.setFamily("Microsoft YaHei");
    m_yTickLabelFont.setPixelSize(m_yTickLabelFontSize);
    m_gridStyle = Qt::DotLine;
    m_gridDensity = GridDensity::LESS;
    m_gridFillColor = Qt::black;

    m_showUnits_x = true;
    m_showUnits_y = true;
    m_titleVisible = true;
    m_titleColor = Qt::white;
    m_titleFillColor = Qt::black;
    m_titleFontSize = 32;
    m_titleFont.setFamily("Microsoft YaHei");
    m_titleFont.setPixelSize(m_titleFontSize);

    m_xAxisLabelColor = Qt::white;
    m_xAxisLabelFontSize = 32.0;
    m_xAxisLabelFont.setFamily("Microsoft YaHei");
    m_xAxisLabelFont.setPixelSize(m_xAxisLabelFontSize);

    m_yAxisLabelColor = Qt::white;
    m_yAxisLabelFontSize = 32.0;
    m_yAxisLabelFont.setFamily("Microsoft YaHei");
    m_yAxisLabelFont.setPixelSize(m_yAxisLabelFontSize);

    m_leftPadding = 10;
    m_rightPadding = 10;
    m_topPadding = 10;
    m_bottomPadding = 10;
    setMinimumSize(200, 150);
    resize(1000, 600);

    setWindowFlags(Qt::FramelessWindowHint);
    // 基类最外层透明

    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, m_outerFillColor);
    this->setPalette(palette);
    updateResizeFocusPos();
    // 数据对整体发生变化时，更新界面
    connect(this,
            &PlotItemBase::dataPairsChanged,
            this,
            &PlotItemBase::onDataPairsChanged,
            Qt::UniqueConnection);
}

PlotItemBase::~PlotItemBase()
{
    qDeleteAll(m_dataPairs);
}

void PlotItemBase::init() {}

void PlotItemBase::setPosition(int x, int y)
{
    m_position.setX(x);
    m_position.setY(y);
    this->setGeometry(x, y, currWidth(), currHeight());
    update();
}

void PlotItemBase::setWidth(int width)
{
    m_width = width;
    update();
}

void PlotItemBase::setHeight(int height)
{
    m_height = height;
    update();
}

void PlotItemBase::setRect(const QRect& rect)
{
    m_position.setX(rect.x());
    m_position.setY(rect.y());
    m_width = rect.width();
    m_height = rect.height();
    this->setGeometry(rect);
    update();
}

QPoint PlotItemBase::currPosition()
{
    m_position = this->pos();
    return m_position;
}

int PlotItemBase::currWidth()
{
    m_width = this->width();
    return m_width;
}

int PlotItemBase::currHeight()
{
    m_height = this->height();
    return m_height;
}

QRect PlotItemBase::currRect()
{
    return this->rect();
}

void PlotItemBase::setName(const QString& name)
{
    m_plotItemName = name;
}

void PlotItemBase::setTabName(const QString& tabName)
{
    m_tabName = tabName;
}

QString PlotItemBase::getName()
{
    return m_plotItemName;
}

QString PlotItemBase::getTabName()
{
    return m_tabName;
}

void PlotItemBase::setOuterFillColor(const QColor& color)
{
    if(m_outerFillColor == color)
        return;
    m_outerFillColor = color;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, m_outerFillColor);
    this->setPalette(palette);
}

void PlotItemBase::setOutlineColor(const QColor& color)
{
    m_outlineColor = color;
    update();
}

void PlotItemBase::setCoordRangeX(double lower, double upper)
{
    m_coordBgn_x = lower;
    m_coordEnd_x = upper;
    if(m_customPlot && m_customPlot->xAxis)
    {
        m_customPlot->xAxis->setRange(lower, upper);
    }
    emit coordRangeChanged(true, lower, upper);
    replot();
}

void PlotItemBase::setCoordRangeY(double lower, double upper)
{
    m_coordBgn_y = lower;
    m_coordEnd_y = upper;
    if(m_customPlot && m_customPlot->yAxis)
    {
        m_customPlot->yAxis->setRange(lower, upper);
    }
    emit coordRangeChanged(false, lower, upper);
    replot();
}

void PlotItemBase::getCoordRangeX(double& lower, double& upper)
{
    lower = m_coordBgn_x;
    upper = m_coordEnd_x;
}

void PlotItemBase::getCoordRangeY(double& lower, double& upper)
{
    lower = m_coordBgn_y;
    upper = m_coordEnd_y;
}

void PlotItemBase::setHorzGrids(int32_t count)
{
    m_horzGrids = count;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            if(count == 0)
            {
                m_customPlot->yAxis->grid()->setVisible(false);
            }
            else
            {
                m_customPlot->yAxis->grid()->setVisible(true);
                m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
            }
        }
    }
    replot();
}

void PlotItemBase::setVertGrids(int32_t count)
{
    m_vertGrids = count;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            if(count == 0)
            {
                m_customPlot->xAxis->grid()->setVisible(false);
            }
            else
            {
                m_customPlot->xAxis->grid()->setVisible(true);
                m_customPlot->xAxis->ticker()->setTickCount(m_vertGrids);
            }
        }
    }
    replot();
}

void PlotItemBase::setAxisColorWidth(const QColor& color, int32_t width)
{
    m_axisColor = color;
    m_axisWidth = width;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
            m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
        }
    }
    replot();
}

void PlotItemBase::setGridColorWidth(const QColor& color, int32_t width)
{
    m_gridColor = color;
    m_gridWidth = width;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
            m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
        }
    }
    replot();
}

void PlotItemBase::setGridVisible(bool enable)
{
    m_gridVisible = enable;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->xAxis->grid()->setVisible(enable);
            m_customPlot->yAxis->grid()->setVisible(enable);
        }
    }
    replot();
}

void PlotItemBase::setxTickLabelVisible(bool show)
{
    m_xTickLabelVisible = show;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis)
            m_customPlot->xAxis->setTickLabels(show);
    }
    replot();
}

void PlotItemBase::setyTickLabelVisible(bool show)
{
    m_yTickLabelVisible = show;
    if(m_customPlot)
    {
        if(m_customPlot->yAxis)
            m_customPlot->yAxis->setTickLabels(show);
    }
    replot();
}

void PlotItemBase::setxTickLabelColor(const QColor& color)
{
    m_xTickLabelColor = color;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->xAxis->setTickLabelColor(m_xTickLabelColor);
        }
    }
    replot();
}

void PlotItemBase::setxTickLabelFont(const QFont& font)
{
    m_xTickLabelFont = font;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->xAxis->setTickLabelFont(m_xTickLabelFont);
        }
    }
    replot();
}

void PlotItemBase::setxTickLabelFontSize(int size)
{
    m_xTickLabelFontSize = size;
    m_xTickLabelFont.setPixelSize(size);
    setxTickLabelFont(m_xTickLabelFont);
}

void PlotItemBase::setyTickLabelColor(const QColor& color)
{
    m_yTickLabelColor = color;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->yAxis->setTickLabelColor(m_yTickLabelColor);
        }
    }
    replot();
}

void PlotItemBase::setyTickLabelFont(const QFont& font)
{
    m_yTickLabelFont = font;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->yAxis->setTickLabelFont(m_yTickLabelFont);
        }
    }
    replot();
}

void PlotItemBase::setyTickLabelFontSize(int size)
{
    m_yTickLabelFontSize = size;
    m_yTickLabelFont.setPixelSize(size);
    setyTickLabelFont(m_yTickLabelFont);
}

void PlotItemBase::setGridStyle(GridStyle style)
{
    switch(style)
    {
    case GridStyle::SOLIDLINE:
        m_gridStyle = Qt::SolidLine;
        break;
    case GridStyle::DASHLINE:
        m_gridStyle = Qt::DashLine;
        break;
    case GridStyle::DOTLINE:
        m_gridStyle = Qt::DotLine;
        break;
    case GridStyle::DASHDOTLINE:
        m_gridStyle = Qt::DashDotLine;
        break;
    default:
        m_gridStyle = Qt::SolidLine;
        break;
    }
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
            m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
        }
    }
    replot();
}

void PlotItemBase::setGridDensity(GridDensity density)
{
    m_gridDensity = density;
    if(plotType() == PlotType::Type_PlotRTI)
        m_customPlot->yAxis->ticker()->setTickCount(density);
    replot();
}

void PlotItemBase::setGridFillColor(const QColor& color)
{
    m_gridFillColor = color;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
            m_customPlot->axisRect()->setBackground(color);
    }
    replot();
}

void PlotItemBase::setUnitsShowX(bool on)
{
    // 单位是初始在各个派生类中初始化，添加数据对之后，会重新设置单位
    m_showUnits_x = on;
    updateXLabelDescrption();
}

void PlotItemBase::setUnitsShowY(bool on)
{
    m_showUnits_y = on;
    updateYLabelDescrption();
}

void PlotItemBase::setUnitsX(const QString& units)
{
    m_units_x = units;
    updateXLabelDescrption();
}

void PlotItemBase::setUnitsY(const QString& units)
{
    m_units_y = units;
    updateYLabelDescrption();
}

void PlotItemBase::setTitleVisible(bool on)
{
    m_titleVisible = on;
    updateTitle();
}

void PlotItemBase::setTitle(const QString& title)
{
    m_title = title;
    updateTitle();
}

void PlotItemBase::setTitleColor(const QColor& color)
{
    m_titleColor = color;
    updateTitle();
}

void PlotItemBase::setTitleFillColor(const QColor& color)
{
    m_titleFillColor = color;
    updateTitle();
}

void PlotItemBase::setTitleFont(const QFont& font)
{
    m_titleFont = font;
    updateTitle();
}

void PlotItemBase::setTitleFontSize(int size)
{
    m_titleFontSize = size;
    m_titleFont.setPixelSize(size);
    updateTitle();
}

void PlotItemBase::setTitleOffset(int offsetX, int offsetY)
{
    m_titleOffsetX = offsetX;
    m_titleOffsetY = offsetY;
    updateTitle();
}

void PlotItemBase::setxAxisLabelVisible(bool on)
{
    m_xAxisLabelVisible = on;
    updateXLabelDescrption();
}

void PlotItemBase::setyAxisLabelVisible(bool on)
{
    m_yAxisLabelVisible = on;
    updateYLabelDescrption();
}

void PlotItemBase::setxAxisLabel(const QString& label)
{
    m_xAxisLabel = label;
    updateXLabelDescrption();
}

void PlotItemBase::setyAxisLabel(const QString& label)
{
    m_yAxisLabel = label;
    updateYLabelDescrption();
}

void PlotItemBase::setxAxisLabelColor(const QColor& color)
{
    m_xAxisLabelColor = color;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->xAxis->setLabelColor(m_xAxisLabelColor);
        }
    }
    replot();
}

void PlotItemBase::setxAxisLabelFont(const QFont& font)
{
    m_xAxisLabelFont = font;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->xAxis->setLabelFont(m_xAxisLabelFont);
        }
    }
    replot();
}

void PlotItemBase::setxAxisLabelFontSize(int size)
{
    m_xAxisLabelFontSize = size;
    m_xAxisLabelFont.setPixelSize(size);
    if(m_customPlot)
    {
        if(m_customPlot->xAxis)
        {
            m_customPlot->xAxis->setLabelFont(m_xAxisLabelFont);
        }
    }
    replot();
}

void PlotItemBase::setyAxisLabelColor(const QColor& color)
{
    m_yAxisLabelColor = color;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->yAxis->setLabelColor(m_yAxisLabelColor);
        }
    }
    replot();
}

void PlotItemBase::setyAxisLabelFont(const QFont& font)
{
    m_yAxisLabelFont = font;
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            m_customPlot->yAxis->setLabelFont(m_yAxisLabelFont);
        }
    }
    replot();
}

void PlotItemBase::setyAxisLabelFontSize(int size)
{
    m_yAxisLabelFontSize = size;
    m_yAxisLabelFont.setPixelSize(size);
    if(m_customPlot)
    {
        if(m_customPlot->yAxis)
        {
            m_customPlot->yAxis->setLabelFont(m_yAxisLabelFont);
        }
    }
    replot();
}

void PlotItemBase::setXPrecision(int value)
{
    m_xPrecision = value;
    replot();
}

void PlotItemBase::setYPrecision(int value)
{
    m_yPrecision = value;
    replot();
}

void PlotItemBase::setPaddings(double top, double bottom, double left, double right)
{
    m_topPadding = top;
    m_bottomPadding = bottom;
    m_leftPadding = left;
    m_rightPadding = right;
    update();
}

void PlotItemBase::updateTitle()
{
    pTitleLabel->setVisible(m_titleVisible);
    pTitleLabel->setFont(m_titleFont);
    pTitleLabel->setText(m_title);
    QPalette pal(pTitleLabel->palette());
    pal.setColor(QPalette::WindowText, m_titleColor);
    pal.setColor(QPalette::Background, m_titleFillColor);
    pTitleLabel->setPalette(pal);
}

// NOTE::其他地方不要直接创建DataPair对象，需要通过本接口创建，不然会导致丢失信号槽逻辑
DataPair* PlotItemBase::addPlotDataPair(int32_t xEntityID,
                                        const QString& xAttrName,
                                        const QString& xAttrUnitName,
                                        int32_t yEntityID,
                                        const QString& yAttrName,
                                        const QString& yAttrUnitName,
                                        const QHash<QString, QVariant>& extraParams,
                                        bool isFromJson)
{
    // TODO:需要完善重复添加逻辑
    DataPair* data =
        new DataPair(xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName);
    m_dataPairs.append(data);
    if(!isFromJson)
    {
        m_units_x = xAttrUnitName;
        m_units_y = yAttrUnitName;
    }
    // 设置额外参数
    if(plotType() == PlotType::Type_PlotLight)
    {
        auto desc = extraParams.value("Desc").toString();
        data->setDesc(desc);
    }
    if(extraParams.contains("UUID"))
    {
        data->setUuid(extraParams.value("UUID").toString());
    }
    if(extraParams.contains("XEntityName"))
        data->setEntity_x(extraParams.value("XEntityName").toString());
    if(extraParams.contains("YEntityName"))
        data->setEntity_y(extraParams.value("YEntityName").toString());

    // 目前界面上都是直接修改DataPair内部的数据，这里提供一个集中的入口虚函数处理。
    connect(data,
            &DataPair::dataUpdate,
            this,
            &PlotItemBase::onDataPairUpdateData,
            Qt::UniqueConnection);
    if(!isFromJson)
    {
        emit dataPairsChanged(this);
    }
    return data;
}

void PlotItemBase::delPlotPairData(const QString& uuid)
{

    for(int i = 0; i < m_dataPairs.size(); ++i)
    {
        if(m_dataPairs.at(i)->getUuid() == uuid)
        {
            auto data = m_dataPairs.takeAt(i);
            data->deleteLater();

            emit dataPairsChanged(this);
            break;
        }
    }
}

void PlotItemBase::updatePlotPairData(const QString& uuid,
                                      int32_t xEntityID,
                                      const QString& xAttrName,
                                      const QString& xAttrUnitName,
                                      int32_t yEntityID,
                                      const QString& yAttrName,
                                      const QString& yAttrUnitName)
{
    if(m_dataPairs.isEmpty())
        return;

    for(int i = 0; i < m_dataPairs.size(); ++i)
    {
        if(m_dataPairs.at(i)->getUuid() == uuid)
        {
            m_dataPairs.at(i)->setEntityIDX(xEntityID);
            m_dataPairs.at(i)->setAttr_x(xAttrName);
            m_dataPairs.at(i)->setUnit_x(xAttrUnitName);

            m_dataPairs.at(i)->setEntityIDY(yEntityID);
            m_dataPairs.at(i)->setAttr_y(yAttrName);
            m_dataPairs.at(i)->setUnit_y(yAttrUnitName);

            emit dataPairsChanged(this);
            break;
        }
    }
}

DataPair* PlotItemBase::getDataPariByUuid(const QString& uuid)
{
    for(auto dataPair : m_dataPairs)
    {
        if(dataPair->getUuid() == uuid)
        {
            return dataPair;
        }
    }
    return nullptr;
}

void PlotItemBase::setDataPair(QVector<DataPair*>& newVector)
{
    m_dataPairs.swap(newVector);

    emit dataPairsChanged(this);
}

void PlotItemBase::slot_setVisible(bool on)
{
    m_bVisible = on;
    this->setVisible(on);
}

void PlotItemBase::onGetCurrentSeconds(double secs)
{
    updateDataForDataPairsByTime(secs);
}

void PlotItemBase::slot_updateRect(const QRect& rect)
{
    setRect(rect);
}

void PlotItemBase::updateResizeFocusPos()
{
    for(int32_t direction = NORTH_MIDDLE; direction <= NORTH_WEST; ++direction)
    {
        ResizeDirection directionEnum = static_cast<ResizeDirection>(direction);
        m_resizeRectMap[directionEnum] = getRectByDirection(directionEnum);
    }
}

QRect PlotItemBase::getRectByDirection(ResizeDirection direction)
{
    const QRectF parentRect = geometry();
    const qreal parentWidth = parentRect.width();
    const qreal parentHeight = parentRect.height();

    qreal x = 0.0, y = 0.0;

    switch(direction)
    {
    case NORTH_MIDDLE:
        x = parentWidth / 2 - m_resizeFocusSize / 2;
        y = 0;
        break;
    case SOUTH_MIDDLE:
        x = parentWidth / 2 - m_resizeFocusSize / 2;
        y = parentHeight - m_resizeFocusSize;
        break;
    case EAST_MIDDLE:
        x = parentWidth - m_resizeFocusSize;
        y = parentHeight / 2 - m_resizeFocusSize / 2;
        break;
    case WEST_MIDDLE:
        x = 0;
        y = parentHeight / 2 - m_resizeFocusSize / 2;
        break;
    case NORTH_WEST:
        x = 0;
        y = 0;
        break;
    case SOUTH_EAST:
        x = parentWidth - m_resizeFocusSize;
        y = parentHeight - m_resizeFocusSize;
        break;
    case NORTH_EAST:
        x = parentWidth - m_resizeFocusSize;
        y = 0;
        break;
    case SOUTH_WEST:
        x = 0;
        y = parentHeight - m_resizeFocusSize;
        break;
    }
    return QRect(x, y, m_resizeFocusSize, m_resizeFocusSize);
}

void PlotItemBase::setCursorByDirection()
{
    switch(m_curResizeDirection)
    {
    case NORTH_MIDDLE:
        setCursor(Qt::SizeVerCursor);
        break;
    case SOUTH_MIDDLE:
        setCursor(Qt::SizeVerCursor);
        break;
    case EAST_MIDDLE:
        setCursor(Qt::SizeHorCursor);
        break;
    case WEST_MIDDLE:
        setCursor(Qt::SizeHorCursor);
        break;
    case NORTH_WEST:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case SOUTH_EAST:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case NORTH_EAST:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case SOUTH_WEST:
        setCursor(Qt::SizeBDiagCursor);
        break;
    }
}

void PlotItemBase::updateGeoWithMouseMove(int offsetX, int offsetY)
{
    QRect rect = this->geometry();
    qreal x1 = rect.x();
    qreal y1 = rect.y();
    qreal newWidth = rect.width();
    qreal newHeight = rect.height();

    switch(m_curResizeDirection)
    {
    case PlotItemBase::EAST_MIDDLE:
        // 正右边
        newWidth += offsetX;
        break;
    case PlotItemBase::SOUTH_MIDDLE:
        // 正下方
        newHeight += offsetY;
        break;
    case PlotItemBase::WEST_MIDDLE:
        // 正左边
        x1 += offsetX;
        newWidth -= offsetX;
        break;
    case PlotItemBase::NORTH_MIDDLE:
        // 正上方
        y1 += offsetY;
        newHeight -= offsetY;
        break;
    case PlotItemBase::NORTH_EAST:
        // 右上方
        y1 += offsetY;
        newWidth += offsetX;
        newHeight -= offsetY;
        break;
    case PlotItemBase::NORTH_WEST:
        // 左上方
        x1 += offsetX;
        y1 += offsetY;
        newWidth -= offsetX;
        newHeight -= offsetY;
        break;
    case PlotItemBase::SOUTH_EAST:
        // 右下方
        newWidth += offsetX;
        newHeight += offsetY;
        break;
    case PlotItemBase::SOUTH_WEST:
        // 左下方
        x1 += offsetX;
        newWidth -= offsetX;
        newHeight += offsetY;
        break;
    }

    if(newWidth <= minimumWidth() && newHeight <= minimumHeight())
        return;
    setGeometry(x1, y1, newWidth, newHeight);
}

void PlotItemBase::drawBorderAndControls()
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    painter.drawRects(m_resizeRectMap.values().toVector());
}

void PlotItemBase::loadGOGFile(const QString& fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "PlotManager::loadGOGFile read file failure:" << fileName;
        return;
    }
    QList<GOGDataInfo> gogFileList;
    while(!file.atEnd())
    {
        GOGDataInfo data;
        QString lineData;
        lineData = file.readLine();
        if(lineData.startsWith("start"))
        {
            lineData = file.readLine();
            data.type = lineData.simplified();

            lineData = file.readLine();
            while(!lineData.startsWith("end"))
            {
                if(lineData.startsWith("ll") || lineData.startsWith("centerll"))
                {
                    if(lineData.startsWith("ll"))
                    {
                        QStringList lineInfo = lineData.split(" ");
                        data.xList.append(lineInfo.at(2).toDouble());
                        data.yList.append(lineInfo.at(1).toDouble());
                    }
                    else if(lineData.startsWith("centerll"))
                    {
                        QStringList circleInfo = lineData.split(" ");
                        data.xList.append(circleInfo.at(2).toDouble());
                        data.yList.append(circleInfo.at(1).toDouble());

                        lineData = file.readLine();
                        if(lineData.startsWith("radius"))
                            data.radius = lineData.split(" ").at(1).toDouble();
                    }
                }

                if(lineData.startsWith("rangeUnits"))
                {
                    data.rangeUnits = lineData.split(" ").at(1).simplified();
                }
                if(lineData.startsWith("altitudeUnits"))
                {
                    data.altitudeUnits = lineData.split(" ").at(1).simplified();
                }
                if(lineData.startsWith("3d"))
                {
                    data.triDName = lineData.split(" ").at(2).simplified();
                }

                if(lineData.startsWith("linecolor"))
                {
                    QString str = lineData.split(" ").at(2).simplified();
                    if(str.size() != 10)
                    {
                        data.lineColor = Qt::red;
                    }
                    else
                    {
                        data.lineColor = color_transfer::QColorFromHexStr(str);
                    }
                }

                if(lineData.startsWith("fillcolor"))
                {
                    QString str = lineData.split(" ").at(2).simplified();
                    // 0xffffffff
                    if(str.size() == 10)
                    {
                        data.fillColor = color_transfer::QColorFromHexStr(str);
                    }
                }

                if(lineData.startsWith("linewidth"))
                {
                    data.lineWidth = lineData.split(" ").at(1).toInt();
                }

                if(lineData.startsWith("depthBuffer"))
                {
                    data.depthBuffer = (lineData.split(" ").at(1).simplified() == "true");
                }

                if(lineData.startsWith("tessellate"))
                {
                    data.tessellate = (lineData.split(" ").at(1).simplified() == "true");
                }
                lineData = file.readLine();
            }
            gogFileList.push_back(data);
        }
    }
    file.close();
    m_gogDataMap.insert(fileName, gogFileList);
}

void PlotItemBase::updateXLabelDescrption()
{
    if(m_customPlot && m_customPlot->xAxis)
    {
        QString label;
        QString unitLabel = m_showUnits_x ? QString("(" + m_units_x + ")") : QString();
        if(m_xAxisLabelVisible)
            label = QString("%1%2").arg(m_xAxisLabel).arg(unitLabel);
        m_customPlot->xAxis->setLabel(label);
    }
    replot();
}

void PlotItemBase::updateYLabelDescrption()
{
    if(m_customPlot && m_customPlot->yAxis)
    {
        QString label;
        QString unitLabel = m_showUnits_y ? QString("(" + m_units_y + ")") : QString();
        if(m_yAxisLabelVisible)
            label = QString("%1%2").arg(m_yAxisLabel).arg(unitLabel);
        m_customPlot->yAxis->setLabel(label);
    }
    replot();
}

void PlotItemBase::onDataPairsChanged()
{
    // 使用时间轴的当前时间
    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}

void PlotItemBase::onXAxisRangeChanged(const QCPRange& range)
{
    if(m_customPlot && m_customPlot->xAxis)
    {
        m_customPlot->xAxis->blockSignals(true);
        setCoordRangeX(range.lower, range.upper);
        m_customPlot->xAxis->blockSignals(false);
    }
}

void PlotItemBase::onYAxisRangeChanged(const QCPRange& range)
{
    if(m_customPlot && m_customPlot->yAxis)
    {
        m_customPlot->yAxis->blockSignals(true);
        setCoordRangeY(range.lower, range.upper);
        m_customPlot->yAxis->blockSignals(false);
    }
}

void PlotItemBase::paintEvent(QPaintEvent* event)
{

    // 绘制本身
    QPainter painter(this);
    QPen pen;
    pen.setColor(m_outlineColor);
    pen.setWidth(m_outlineWidth);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    painter.drawRect(rect());

    // 根据场景绘制外边框和控制点
    if(m_isNeedDrawBorder)
    {
        updateResizeFocusPos();
        drawBorderAndControls();
    }
    QWidget::paintEvent(event);
}

void PlotItemBase::onDataPairUpdateData()
{
    /*  高级管理界面或者其他界面直接修改DataPair里面的数据时，
     *  此时会触发因为没有封装独立的刷新接口，此处调用总的数据刷新入口
     * TODO:提供一个针对某个DataPair发生变化的刷新接口
    */
    auto dataPair = qobject_cast<DataPair*>(sender());
    if(dataPair)
    {
        updateGraphByDataPair(dataPair, PlotXYDemo::getSeconds());
        replot();
    }
}

void PlotItemBase::onPlotMouseEventEnable(bool on)
{
    if(m_customPlot)
    {
        m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
    }
    else
    {
        // TODO
    }
}

void PlotItemBase::updateDataForDataPairsByTime(double) {}

void PlotItemBase::updateGraphByDataPair(DataPair* dataPair, double curSecs)
{
    Q_UNUSED(dataPair)
    Q_UNUSED(curSecs)
}

void PlotItemBase::customPainting(QPainter& /*painter*/) {}

QMap<QString, GOGCustomSetting> PlotItemBase::getGogCustomSettings() const
{
    return m_gogCustomSettings;
}

void PlotItemBase::setGogCustomSettings(const QMap<QString, GOGCustomSetting>& gogCustomSettings)
{
    m_gogCustomSettings = gogCustomSettings;
    drawGOGData();
}

bool PlotItemBase::getBVisible() const
{
    return m_bVisible;
}

void PlotItemBase::setBVisible(bool bVisible)
{
    m_bVisible = bVisible;
}

void PlotItemBase::mousePressEvent(QMouseEvent* event)
{
    if((event->button() == Qt::LeftButton))
    {
        if(m_isDrawMeasureLine)
        {
            if(m_customPlot && m_measureLineItem && m_measureTextItem)
            {
                if(m_customPlot->xAxis && m_customPlot->yAxis)
                {
                    m_originPoint = m_customPlot->mapFromParent(event->pos());
                    m_measureLineItem->start->setCoords(
                        QPointF(m_customPlot->xAxis->pixelToCoord(m_originPoint.x()),
                                m_customPlot->yAxis->pixelToCoord(m_originPoint.y())));

                    m_measureLineItem->end->setCoords(
                        QPointF(m_customPlot->xAxis->pixelToCoord(m_originPoint.x()),
                                m_customPlot->yAxis->pixelToCoord(m_originPoint.y())));
                    m_measureLineItem->setVisible(true);
                    m_measureTextItem->setText(QString());
                    m_measureTextItem->setVisible(true);
                    m_customPlot->replot();
                }
            }
        }
    }
    QWidget::mousePressEvent(event);
}

void PlotItemBase::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_isDrawMeasureLine)
        {
            // 重置到无效状态
            if(m_customPlot && m_measureLineItem && m_measureTextItem)
            {
                m_measureLineItem->setVisible(false);
                m_measureTextItem->setVisible(false);
                m_customPlot->replot();
            }
        }
        QWidget::mouseReleaseEvent(event);
    }
}

void PlotItemBase::mouseMoveEvent(QMouseEvent* event)
{
    if((event->buttons() & Qt::LeftButton))
    {

        if(m_isDrawMeasureLine)
        {
            if(m_customPlot && m_measureLineItem && m_measureTextItem)
            {
                if(m_customPlot->xAxis && m_customPlot->yAxis)
                {
                    auto point = m_customPlot->mapFromParent(event->pos());
                    double dx = 0.0;
                    double dy = 0.0;
                    double l = 0.0;
                    m_measureLineItem->end->setCoords(
                        QPointF(m_customPlot->xAxis->pixelToCoord(point.x()),
                                m_customPlot->yAxis->pixelToCoord(point.y())));

                    m_measureTextItem->position->setCoords(QPointF(
                        m_customPlot->xAxis->pixelToCoord((point.x() + m_originPoint.x()) / 2),
                        m_customPlot->yAxis->pixelToCoord((point.y() + m_originPoint.y()) / 2)));

                    dx = m_customPlot->xAxis->pixelToCoord(point.x()) -
                         m_customPlot->xAxis->pixelToCoord(m_originPoint.x());
                    dy = m_customPlot->yAxis->pixelToCoord(point.y()) -
                         m_customPlot->yAxis->pixelToCoord(m_originPoint.y());
                    l = sqrt(pow(dx, 2) + pow(dy, 2));
                    m_measureTextItem->setText(
                        QString("dx = %1\ndy = %2\nl = %3").arg(dx).arg(dy).arg(l));
                    m_customPlot->replot();
                }
            }
        }
    }
    QWidget::mouseMoveEvent(event);
}

bool PlotItemBase::eventFilter(QObject* obj, QEvent* event)
{
    if(m_widget && obj == m_widget)
    {
        if(event->type() == QEvent::Paint)
        {
            QPainter painter(m_widget);
            painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            customPainting(painter);
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

double PlotItemBase::getXRate() const
{
    return m_xRate;
}

double PlotItemBase::getYRate() const
{
    return m_yRate;
}

void PlotItemBase::setYRate(double yRate)
{
    if(math::doubleEqual(yRate, 0.0))
    {
        return;
    }
    m_yRate = yRate;
    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}

void PlotItemBase::setXRate(double xRate)
{
    if(math::doubleEqual(xRate, 0.0))
    {
        return;
    }
    m_xRate = xRate;
    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}

int PlotItemBase::getOutlineWidth() const
{
    return m_outlineWidth;
}

void PlotItemBase::setOutlineWidth(int outlineWidth)
{
    m_outlineWidth = outlineWidth;
    replot();
}

QMap<QString, QList<GOGDataInfo>> PlotItemBase::getGogDataMap() const
{
    return m_gogDataMap;
}

int PlotItemBase::getBarLeftPadding() const
{
    return m_barLeftPadding;
}

void PlotItemBase::setBarLeftPadding(int barLeftPadding)
{
    m_barLeftPadding = barLeftPadding;
}

void PlotItemBase::rescaleXAxis()
{
    if(m_customPlot && m_customPlot->xAxis)
    {
        m_customPlot->xAxis->rescale();
        m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
    }
}

void PlotItemBase::rescaleYAxis()
{
    if(m_customPlot && m_customPlot->yAxis)
    {
        m_customPlot->yAxis->rescale();
        m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
    }
}

void PlotItemBase::rescaleAxis()
{
    if(m_customPlot)
    {
        m_customPlot->rescaleAxes();
        m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
    }
}

void PlotItemBase::addEvent(const EventSettings& event)
{
    if(!m_eventList.contains(event))
    {
        m_eventList.append(event);
        updateTimelineGraph();
    }
}

void PlotItemBase::removeEvent(const QString& entityName, const QString& type)
{
    for(auto& event : m_eventList)
    {
        if((entityName == event.m_entityName) && (type == event.m_type))
        {
            m_eventList.removeOne(event);
            updateTimelineGraph();
            return;
        }
    }
}

QList<EventSettings> PlotItemBase::getEventList() const
{
    return m_eventList;
}

void PlotItemBase::setEventList(const QList<EventSettings>& eventList)
{
    m_eventList = eventList;
}

void PlotItemBase::drawGOGData() {}

void PlotItemBase::addGOGFile(const QString& fileName)
{
    m_allGOGFileList.push_back(fileName);
    loadGOGFile(fileName);
    drawGOGData();
}

void PlotItemBase::removeGOGFile(const QString& fileName)
{
    int cnt = m_allGOGFileList.size();
    for(int i = 0; i < cnt; i++)
    {
        if(m_allGOGFileList.at(i) == fileName)
        {
            m_allGOGFileList.removeAt(i);
            break;
        }
    }
    if(m_gogDataMap.contains(fileName))
        m_gogDataMap.remove(fileName);
    drawGOGData();
}

QList<QString> PlotItemBase::getGOGFileList()
{
    return m_allGOGFileList;
}

int PlotItemBase::getBarRightPadding() const
{
    return m_barRightPadding;
}

void PlotItemBase::setBarRightPadding(int barRightPadding)
{
    m_barRightPadding = barRightPadding;
}

int PlotItemBase::getBarBetweenPadding() const
{
    return m_barBetweenPadding;
}

void PlotItemBase::setBarBetweenPadding(int barBetweenPadding)
{
    m_barBetweenPadding = barBetweenPadding;
}

bool PlotItemBase::getIsHorizonBar() const
{
    return m_isHorizonBar;
}

void PlotItemBase::setIsHorizonBar(bool isHorizonBar)
{
    m_isHorizonBar = isHorizonBar;
}

bool PlotItemBase::getYIsAdaptive() const
{
    return m_yIsAdaptive;
}

void PlotItemBase::setYIsAdaptive(bool yIsAdaptive)
{
    m_yIsAdaptive = yIsAdaptive;
}

void PlotItemBase::setInteract(QCP::Interaction inter)
{
    if(m_customPlot)
    {
        m_customPlot->setInteractions(inter);
    }
}

void PlotItemBase::setZoom(uint mode)
{
    if(m_customPlot)
    {
        // 1表示zoom，2表示boxzoom
        if(mode == 1)
            m_customPlot->setInteractions(QCP::iRangeZoom);
        else if(mode == 2)
        {
            m_customPlot->setSelectionRectMode(QCP::srmZoom);
        }
    }
}

void PlotItemBase::clearInter()
{
    if(m_customPlot)
    {
        m_customPlot->setInteractions(QCP::iNone);
        m_customPlot->setSelectionRectMode(QCP::srmNone);
        m_isDrawMeasureLine = false;
    }
}

void PlotItemBase::setNewTickOrigin(const QPoint& point)
{
    if(m_customPlot)
    {
        if(m_customPlot->xAxis && m_customPlot->yAxis)
        {
            auto plotPoint = m_customPlot->mapFromParent(point);
            m_customPlot->xAxis->setRange(m_customPlot->xAxis->pixelToCoord(plotPoint.x()) -
                                              m_customPlot->xAxis->range().size() / 2,
                                          m_customPlot->xAxis->pixelToCoord(plotPoint.x()) +
                                              m_customPlot->xAxis->range().size() / 2);
            m_customPlot->yAxis->setRange(m_customPlot->yAxis->pixelToCoord(plotPoint.y()) -
                                              m_customPlot->yAxis->range().size() / 2,
                                          m_customPlot->yAxis->pixelToCoord(plotPoint.y()) +
                                              m_customPlot->yAxis->range().size() / 2);
            m_customPlot->replot();
        }
    }
}

void PlotItemBase::setIsDrawMeasureLine(bool isDraw)
{
    m_isDrawMeasureLine = isDraw;

    if(m_customPlot && isDraw)
    {
        m_measureLineItem = new QCPItemLine(m_customPlot);
        QPen linePen(Qt::red);
        linePen.setWidth(2);
        m_measureLineItem->setPen(linePen);

        m_measureTextItem = new QCPItemText(m_customPlot);
        m_measureTextItem->setPositionAlignment(Qt::AlignCenter | Qt::AlignTop);
        m_measureTextItem->setColor(Qt::white);
    }
}

void PlotItemBase::setCustomPlotMouseTransparent(bool on)
{
    if(m_customPlot)
    {
        m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
    }
    setAttribute(Qt::WA_TransparentForMouseEvents, on);
}

void PlotItemBase::setCustomPlotMouseTransparent(bool baseTransparent, bool customplotTransparent)
{
    if(m_customPlot)
    {
        m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, customplotTransparent);
    }
    this->setAttribute(Qt::WA_TransparentForMouseEvents, baseTransparent);
}

void PlotItemBase::setupLayout()
{
    pTitleLabel = new QLabel;
    pTitleLabel->setFont(m_titleFont);
    pTitleLabel->setText(m_title);
    pTitleLabel->setContentsMargins(0, 0, 0, 0);
    pTitleLabel->setAlignment(Qt::AlignCenter);

    titleLayout = new QHBoxLayout;
    titleLayout->addSpacerItem(
        new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Preferred));
    titleLayout->addWidget(pTitleLabel);
    titleLayout->addSpacerItem(
        new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Preferred));

    QPalette pe(pTitleLabel->palette());
    pe.setColor(QPalette::WindowText, m_titleColor);
    pe.setColor(QPalette::Background, m_titleFillColor);
    pTitleLabel->setAutoFillBackground(true);
    pTitleLabel->setPalette(pe);

    mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(
        int(m_leftPadding), int(m_topPadding), int(m_rightPadding), int(m_bottomPadding));

    mainLayout->addLayout(titleLayout, 1);

    pTitleLabel->setVisible(m_titleVisible);

    // 绘图分为两种，一种是qcustomplot绘制，一种是自绘
    if(m_customPlot)
    {
        mainLayout->addWidget(m_customPlot, 9);
        // 图表部分透明，其他区域在图表管理中进行手动设置
        m_customPlot->setStyleSheet("background:hsva(255,255,255,0%);");
        m_customPlot->setBackground(Qt::transparent);

        if(m_customPlot->xAxis)
        {
            connect(m_customPlot->xAxis,
                    QOverload<const QCPRange&>::of(&QCPAxis::rangeChanged),
                    this,
                    &PlotItemBase::onXAxisRangeChanged);
        }
        if(m_customPlot->yAxis)
        {
            connect(m_customPlot->yAxis,
                    QOverload<const QCPRange&>::of(&QCPAxis::rangeChanged),
                    this,
                    &PlotItemBase::onYAxisRangeChanged);
        }
    }
    else if(m_widget)
    {
        m_widget->installEventFilter(this);
        mainLayout->addWidget(m_widget, 9);
    }

    delete layout();
    setLayout(mainLayout);
}

void PlotItemBase::replot()
{
    if(m_customPlot)
    {
        m_customPlot->replot();
    }
    else
    {
        update();
    }
}

bool PlotItemBase::getXIsAdaptive() const
{
    return m_xIsAdaptive;
}

void PlotItemBase::setXIsAdaptive(bool xIsAdaptive)
{
    m_xIsAdaptive = xIsAdaptive;
}

bool PlotItemBase::getIsNeedDrawBorder() const
{
    return m_isNeedDrawBorder;
}

void PlotItemBase::setIsNeedDrawBorder(bool isNeedDrawBorder)
{
    m_isNeedDrawBorder = isNeedDrawBorder;
}

bool PlotItemBase::isContainedInResizeRects(const QPoint& point)
{
    for(const auto& key : m_resizeRectMap.keys())
    {
        if(m_resizeRectMap[key].contains(point))
        {
            m_curResizeDirection = static_cast<ResizeDirection>(key);
            return true;
        }
    }
    return false;
}
