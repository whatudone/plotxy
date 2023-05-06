/* * @filename: PlotItemBase.cpp
* @brief: PlotItemBase PlotItemBase
* @author: zhameng
* @create time: 2022-09-21
*  */

#include "PlotItemBase.h"

#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QPushButton>

PlotItemBase::PlotItemBase(QWidget* parent)
    : QWidget(parent)
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
    m_outerFillColor = Qt::black;
    m_outlineColor = Qt::black;

    m_horzGrids = 4;
    m_vertGrids = 4;
    m_axisWidth = 1;
    m_gridWidth = 1;
    m_axisColor = Qt::white;
    m_gridColor = Qt::white;
    m_gridVisible = true;
    m_tickLabelColor = Qt::white;
    m_tickLabelFontSize = 20;
    m_tickLabelFont.setFamily("Microsoft YaHei");
    m_tickLabelFont.setPointSizeF(m_tickLabelFontSize);
    m_gridStyle = Qt::DotLine;
    m_gridDensity = GridDensity::LESS;
    m_gridFillColor = Qt::black;

    m_showUnits_x = true;
    m_showUnits_y = true;
    m_titleVisible = true;
    m_titleColor = Qt::white;
    m_titleFillColor = Qt::black;
    m_titleFontSize = 16;
    m_titleFont.setFamily("Microsoft YaHei");
    m_titleFont.setPointSizeF(m_titleFontSize);

    m_leftPadding = 10;
    m_rightPadding = 10;
    m_topPadding = 10;
    m_bottomPadding = 10;

    //设置无边框属性
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Widget);

    setAutoFillBackground(true);
    setMinimumSize(200, 150);
    resize(1600, 800);

    //设置下背景颜色区别看
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, getOuterFillColor());
    setPalette(palette);

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

void PlotItemBase::setRect(QRect rect)
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
    setAutoFillBackground(true);
    m_outerFillColor = color;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, m_outerFillColor);
    this->setPalette(palette);
    if(m_customPlot)
    {
        m_customPlot->setBackground(color);
        m_customPlot->replot();
    }
    update();
}

void PlotItemBase::setOutlineColor(const QColor& color)
{
    m_outlineColor = color;
}

void PlotItemBase::setCoordRangeX(double lower, double upper)
{
    m_coordBgn_x = lower;
    m_coordEnd_x = upper;
    if(m_customPlot)
    {
        m_customPlot->xAxis->setRange(lower, upper);
        m_customPlot->replot();
    }
    update();
}

void PlotItemBase::setCoordRangeY(double lower, double upper)
{
    m_coordBgn_y = lower;
    m_coordEnd_y = upper;
    if(m_customPlot)
    {
        m_customPlot->yAxis->setRange(lower, upper);
        m_customPlot->replot();
    }
    update();
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

void PlotItemBase::setHorzGrids(uint count)
{
    m_horzGrids = count;
}

void PlotItemBase::setVertGrids(uint count)
{
    m_vertGrids = count;
}

void PlotItemBase::setAxisColorWidth(const QColor& color, uint width)
{
    m_axisColor = color;
    m_axisWidth = width;
}

void PlotItemBase::setGridColorWidth(const QColor& color, uint width)
{
    m_gridColor = color;
    m_gridWidth = width;
}

void PlotItemBase::setGridVisible(bool enable)
{
    m_gridVisible = enable;
}

void PlotItemBase::setTickLabelColor(const QColor& color)
{
    m_tickLabelColor = color;
}

void PlotItemBase::setTickLabelFont(const QFont& font)
{
    m_tickLabelFont = font;
}

void PlotItemBase::setTickLabelFontSize(int size)
{
    m_tickLabelFontSize = size;
    m_tickLabelFont.setPixelSize(size);
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
}

void PlotItemBase::setGridDensity(GridDensity density)
{
    m_gridDensity = density;
}

void PlotItemBase::setGridFillColor(const QColor& color)
{
    m_gridFillColor = color;
}

void PlotItemBase::setUnitsShowX(bool on)
{
    m_showUnits_x = on;
}

void PlotItemBase::setUnitsShowY(bool on)
{
    m_showUnits_y = on;
}

void PlotItemBase::setUnitsX(const QString& units)
{
    m_units_x = units;
}

void PlotItemBase::setUnitsY(const QString& units)
{
    m_units_y = units;
}

void PlotItemBase::setTitleVisible(bool on)
{
    m_titleVisible = on;
}

void PlotItemBase::setTitle(const QString& title)
{
    m_title = title;
}

void PlotItemBase::setTitleColor(const QColor& color)
{
    m_titleColor = color;
}

void PlotItemBase::setTitleFillColor(const QColor& color)
{
    m_titleFillColor = color;
}

void PlotItemBase::setTitleFont(const QFont& font)
{
    m_titleFont = font;
}

void PlotItemBase::setTitleFontSize(int size)
{
    m_titleFontSize = size;
    m_titleFont.setPointSize(size);
}

void PlotItemBase::setxAxisLabel(const QString& label)
{
    m_xAxisLabel = label;
}

void PlotItemBase::setyAxisLabel(const QString& label)
{
    m_yAxisLabel = label;
}

void PlotItemBase::setAxisLabelColor(const QColor& color)
{
    m_axisLabelColor = color;
}

void PlotItemBase::setAxisLabelFont(const QFont& font)
{
    m_axisLabelFont = font;
}

void PlotItemBase::setAxisLabelFontSize(int size)
{
    m_axisLabelFontSize = size;
    m_axisLabelFont.setPointSize(size);
}

// NOTE::其他地方不要直接创建DataPair对象，需要通过本接口创建，不然会导致丢失信号槽逻辑
DataPair* PlotItemBase::addPlotDataPair(int32_t xEntityID,
                                        const QString& xAttrName,
                                        const QString& xAttrUnitName,
                                        int32_t yEntityID,
                                        const QString& yAttrName,
                                        const QString& yAttrUnitName,
                                        const QVariantList& extraParams)
{
    // TODO:需要完善重复添加逻辑
    DataPair* data =
        new DataPair(xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName);
    m_dataPairs.append(data);
    // 设置额外参数
    if(plotType() == PlotType::Type_PlotLight)
    {
        auto desc = extraParams.at(0).toString();
        data->setDesc(desc);
    }

    // 目前界面上都是直接修改DataPair内部的数据，这里提供一个集中的入口虚函数处理。
    connect(data,
            &DataPair::dataUpdate,
            this,
            &PlotItemBase::onDataPairUpdateData,
            Qt::UniqueConnection);
    emit dataPairsChanged(this);
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
    m_seconds = secs;
    if(getDataPairs().isEmpty())
        return;
    updateDataForDataPairsByTime(m_seconds);
    update();
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

void PlotItemBase::onDataPairsChanged()
{
    updateDataForDataPairsByTime(m_seconds);
}

void PlotItemBase::paintEvent(QPaintEvent* event)
{
    // 绘制本身
    QWidget::paintEvent(event);
    // 绘制通用的属性
    int width = this->width();
    int height = this->height();
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QFontMetricsF fm(m_titleFont);
    double w = fm.size(Qt::TextSingleLine, m_title).width();
    double h = fm.size(Qt::TextSingleLine, m_title).height();
    double as = fm.ascent();
    // 通用标题属性
    if(m_titleVisible)
    {
        painter.setFont(m_titleFont);
        painter.setPen(m_titleColor);
        painter.fillRect(
            (width - w + m_leftPadding - m_rightPadding) / 2, m_topPadding, w, h, m_titleFillColor);
        painter.drawText(
            QPoint((width + m_leftPadding - m_rightPadding - w) / 2, as + m_topPadding), m_title);
    }
    // 绘图分为两种，一种是qcustomplot绘制，一种是自绘
    if(m_customPlot)
    {
        if(plotType() != PlotType::Type_PlotDoppler)
        {
            m_customPlot->setGeometry(m_leftPadding,
                                      h + m_topPadding,
                                      width - m_leftPadding - m_rightPadding,
                                      height - h - m_topPadding - m_bottomPadding);
        }
    }
    else
    {
        customPainting(painter);
    }
    // 根据场景绘制外边框和控制点
    if(m_isNeedDrawBorder)
    {
        updateResizeFocusPos();
        drawBorderAndControls();
    }
}

void PlotItemBase::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>> /* targetMap*/) {}

void PlotItemBase::onDataPairUpdateData()
{
    /*  高级管理界面或者其他界面直接修改DataPair里面的数据时，
     *  此时会触发因为没有封装独立的刷新接口，此处调用总的数据刷新入口
     * TODO:提供一个针对某个DataPair发生变化的刷新接口
    */
    updateDataForDataPairsByTime(m_seconds);
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

void PlotItemBase::customPainting(QPainter& /*painter*/) {}

void PlotItemBase::mousePressEvent(QMouseEvent* event)
{
    if((event->button() == Qt::LeftButton))
    {
        if(m_isDrawMeasureLine)
        {
            if(m_customPlot && m_measureLineItem && m_measureTextItem)
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
    QWidget::mouseMoveEvent(event);
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
        this->setAttribute(Qt::WA_TransparentForMouseEvents, baseTransparent);
    }
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
