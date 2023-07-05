#include "PlotPolar.h"
#include "DataManager.h"
#include "Utils.h"

int PlotPolar::m_instanceCount = 1;
PlotPolar::PlotPolar(QWidget* parent)
    : PlotItemBase(parent)
{
    QString name = QString("Polar%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_title = "Polar";

    m_angularRange_lower = 0.0;
    m_angularRange_upper = 360.0;
    m_radialRange_lower = 0.0;
    m_radialRange_upper = 2000.0;

    m_horzGrids = (m_angularRange_upper - m_angularRange_lower) / 30.0;

    m_gridStyle = Qt::SolidLine;

    initPlot();
    setupLayout();
}

PlotPolar::~PlotPolar() {}

void PlotPolar::initPlot()
{
    m_customPlot = new QCustomPlot(this);
    m_customPlot->plotLayout()->clear();

    m_angularAxis = new QCPPolarAxisAngular(m_customPlot);
    m_angularAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->plotLayout()->addElement(0, 0, m_angularAxis);
    m_customPlot->setBackground(m_gridFillColor);

    m_angularAxis->setRangeDrag(false);
    m_angularAxis->setTickLabelMode(QCPPolarAxisAngular::lmUpright);

    m_angularAxis->setTickLabelColor(m_xTickLabelColor);
    m_xTickLabelFont.setPixelSize(m_xTickLabelFontSize);
    m_angularAxis->setTickLabelFont(m_xTickLabelFont);
    m_angularAxis->setTickPen(QPen(m_gridColor, 1));

    m_angularAxis->radialAxis()->setTickLabelColor(m_yTickLabelColor);
    m_yTickLabelFont.setPixelSize(m_yTickLabelFontSize);
    m_angularAxis->radialAxis()->setTickLabelFont(m_yTickLabelFont);

    m_angularAxis->radialAxis()->setTickLabelMode(QCPPolarAxisRadial::lmUpright);
    m_angularAxis->radialAxis()->setTickLabelRotation(0);
    m_angularAxis->radialAxis()->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_angularAxis->radialAxis()->setAngle(0);

    //angularAxis->radialAxis()->setNumberFormat("e");
    m_angularAxis->grid()->setAngularPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_angularAxis->grid()->setRadialPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_angularAxis->grid()->setSubGridType(QCPPolarGrid::gtNone);

    m_angularAxis->setRange(m_angularRange_lower, m_angularRange_upper);
    m_angularAxis->radialAxis()->setRange(m_radialRange_lower, m_radialRange_upper);
    m_angularAxis->radialAxis()->setRangeReversed(false);

    m_vertGrids = m_angularAxis->radialAxis()->ticker()->tickCount();
    m_customPlot->replot();
}

void PlotPolar::setAxisColorWidth(const QColor& color, int32_t width)
{
    if(m_axisColor == color && m_axisWidth == width)
        return;

    m_axisColor = color;
    m_axisWidth = width;
    m_angularAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_angularAxis->radialAxis()->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->replot();
}

void PlotPolar::setGridColorWidth(const QColor& color, int32_t width)
{
    if(m_gridColor == color && m_gridWidth == width)
        return;

    m_gridColor = color;
    m_gridWidth = width;
    m_angularAxis->grid()->setAngularPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
    m_angularAxis->grid()->setRadialPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
    m_angularAxis->setTickPen(QPen(m_gridColor, 1));
    m_customPlot->replot();
}

void PlotPolar::setHorzGrids(int32_t count)
{
    if(m_horzGrids == count || count <= 0)
    {
        return;
    }
    m_horzGrids = count;
    double range = (m_angularRange_upper - m_angularRange_lower) / count;
    m_angularAxis->ticker().dynamicCast<QCPAxisTickerFixed>().data()->setTickStep(range);
    m_customPlot->replot();
}

void PlotPolar::setVertGrids(int32_t count)
{
    if(m_vertGrids == count)
    {
        return;
    }
    m_vertGrids = count;
    if(count == 0)
    {
        m_angularAxis->grid()->setVisible(false);
    }
    else
    {
        m_angularAxis->grid()->setVisible(true);
        m_angularAxis->radialAxis()->ticker()->setTickCount(count);
    }
    m_customPlot->replot();
}

void PlotPolar::setGridVisible(bool enable)
{
    m_gridVisible = enable;
    m_angularAxis->grid()->setVisible(enable);
    m_customPlot->replot();
}

void PlotPolar::setxTickLabelVisible(bool show)
{
    m_xTickLabelVisible = show;
    m_angularAxis->setTickLabels(show);
    m_customPlot->replot();
}

void PlotPolar::setxTickLabelColor(const QColor& color)
{
    m_xTickLabelColor = color;
    m_angularAxis->setTickLabelColor(m_xTickLabelColor);
    m_customPlot->replot();
}

void PlotPolar::setxTickLabelFont(const QFont& font)
{
    m_xTickLabelFont = font;
    m_angularAxis->setTickLabelFont(font);
    m_customPlot->replot();
}

void PlotPolar::setxTickLabelFontSize(int size)
{
    m_xTickLabelFontSize = size;
    m_xTickLabelFont.setPixelSize(size);
    m_angularAxis->setTickLabelFont(m_xTickLabelFont);
    m_customPlot->replot();
}

void PlotPolar::setyTickLabelVisible(bool show)
{
    m_yTickLabelVisible = show;
    m_angularAxis->radialAxis()->setTickLabels(show);
    m_customPlot->replot();
}

void PlotPolar::setyTickLabelColor(const QColor& color)
{
    m_yTickLabelColor = color;
    m_angularAxis->radialAxis()->setTickLabelColor(m_yTickLabelColor);
    m_customPlot->replot();
}

void PlotPolar::setyTickLabelFont(const QFont& font)
{
    m_yTickLabelFont = font;
    m_angularAxis->radialAxis()->setTickLabelFont(font);
    m_customPlot->replot();
}

void PlotPolar::setyTickLabelFontSize(int size)
{
    m_yTickLabelFontSize = size;
    m_yTickLabelFont.setPixelSize(size);
    m_angularAxis->radialAxis()->setTickLabelFont(m_yTickLabelFont);
    m_customPlot->replot();
}

void PlotPolar::setGridStyle(GridStyle style)
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
    m_angularAxis->grid()->setAngularPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_angularAxis->grid()->setRadialPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->replot();
}

void PlotPolar::setUnitsShowX(bool on)
{
    m_showUnits_x = on;
    m_angularAxis->setFormatShow(on);
    m_customPlot->replot();
}

void PlotPolar::setUnitsShowY(bool on)
{
    m_showUnits_y = on;
    m_angularAxis->radialAxis()->setFormatShow(on);
    m_customPlot->replot();
}

void PlotPolar::setUnitsX(const QString& units)
{
    m_units_x = units;
    m_angularAxis->setFormat(units);
    m_customPlot->replot();
}

void PlotPolar::setUnitsY(const QString& units)
{
    m_units_y = units;
    m_angularAxis->radialAxis()->setFormat(units);
    m_customPlot->replot();
}

void PlotPolar::setCoordRangeX(double lower, double upper)
{
    m_angularRange_lower = lower;
    m_angularRange_upper = upper;
    m_angularAxis->setRange(lower, upper);
    m_customPlot->replot();
}

void PlotPolar::setCoordRangeY(double lower, double upper)
{
    m_radialRange_lower = lower;
    m_radialRange_upper = upper;
    m_angularAxis->radialAxis()->setRange(lower, upper);
    m_customPlot->replot();
}

void PlotPolar::getCoordRangeX(double& lower, double& upper)
{
    lower = m_angularRange_lower;
    upper = m_angularRange_upper;
}

void PlotPolar::getCoordRangeY(double& lower, double& upper)
{
    lower = m_radialRange_lower;
    upper = m_radialRange_upper;
}

void PlotPolar::rescaleXAxis()
{
    m_angularAxis->rescale();
    m_customPlot->replot();
}

void PlotPolar::rescaleYAxis()
{
    m_angularAxis->radialAxis()->rescale();
    m_customPlot->replot();
}

void PlotPolar::rescaleAxis()
{
    m_customPlot->rescaleAxes();
    m_customPlot->replot();
}

void PlotPolar::drawGOGData()
{
    QList<QString> keyList = m_gogDataMap.keys();
    for(auto graph : m_gogGraphList)
    {
        m_angularAxis->removeGraph(graph);
    }
    m_gogGraphList.clear();
    for(auto fileName : keyList)
    {
        GOGCustomSetting setting = m_gogCustomSettings.value(fileName);
        QList<GOGDataInfo> dataList = m_gogDataMap[fileName];
        if(setting.isDraw)
        {

            for(auto data : dataList)
            {
                bool isFilled = false;
                QColor fillColor;
                if(setting.fillState == Qt::Checked)
                {
                    isFilled = true;
                    fillColor = setting.fillColor;
                }
                else if(setting.fillState == Qt::Unchecked)
                {
                    isFilled = false;
                }
                else if(setting.fillState == Qt::PartiallyChecked)
                {
                    if(data.isFill)
                    {
                        isFilled = true;
                        fillColor = data.fillColor;
                    }
                    else
                    {
                        isFilled = false;
                    }
                }
                int lineWidth = setting.lineWidth == 0 ? data.lineWidth : setting.lineWidth;
                QCPPolarGraph* graph =
                    new QCPPolarGraph(m_angularAxis, m_angularAxis->radialAxis());
                if(data.type == "line")
                {
                    //                    if(isFilled)
                    //                        graph->setBrush(fillColor);
                    //                    else
                    graph->setBrush(Qt::NoBrush);
                    graph->setVisible(true);
                    QPen pen = graph->pen();
                    pen.setStyle(Qt::SolidLine);
                    pen.setColor(QColor(data.lineColor));
                    pen.setWidth(lineWidth);
                    graph->setPen(pen);

                    graph->setScatterStyle(QCPScatterStyle::ssNone);
                    graph->setLineStyle(QCPPolarGraph::lsLine);
                    graph->setData(data.xList, data.yList, true);
                }
                else if(data.type == "circle")
                {
                    // 极坐标图没有绘制圆的相关接口，只能通过画多个点的方式拟合圆
                    QPen pen = graph->pen();
                    pen.setStyle(Qt::SolidLine);
                    pen.setColor(QColor(data.lineColor));
                    pen.setWidth(lineWidth);
                    graph->setPen(pen);

                    // 填充颜色
                    //                    if(isFilled)
                    //                        graph->setBrush(fillColor);
                    //                    else
                    graph->setBrush(Qt::NoBrush);

                    // 添加数据点
                    int numPoints = 200; // 圆形轮廓的点数
                    double radius = data.radius; // 圆形的半径
                    double centerX = data.xList.at(0);
                    double centerY = data.yList.at(0);
                    QVector<double> vecX;
                    QVector<double> vecY;

                    for(int i = 0; i <= numPoints; ++i)
                    {
                        double theta = 2.0 * M_PI * i / numPoints; // 角度范围从 0 到 2π
                        double x = centerX + radius * qCos(theta);
                        double y = centerY + radius * qSin(theta);
                        vecX.append(x);
                        vecY.append(y);
                    }
                    graph->setData(vecX, vecY, true);
                }
                m_gogGraphList.append(graph);
            }
        }
    }
}

void PlotPolar::updateDataForDataPairsByTime(double secs)
{
    int isize = getDataPairs().size();

    for(int i = 0; i < isize; ++i)
    {
        auto dataPair = getDataPairs().at(i);
        auto xEntityID = dataPair->getEntityIDX();
        auto yEntityID = dataPair->getEntityIDY();
        auto xAttr = dataPair->getAttr_x();
        auto yAttr = dataPair->getAttr_y();

        QVector<double> x =
            DataManager::getInstance()->getEntityAttrValueListByMaxTime(xEntityID, xAttr, secs);
        QVector<double> y =
            DataManager::getInstance()->getEntityAttrValueListByMaxTime(yEntityID, yAttr, secs);

        m_dataHash.insert(dataPair->getUuid(), qMakePair(x, y));
    }
    for(int i = 0; i < isize; ++i)
    {
        updateGraphByDataPair(m_dataPairs.at(i), secs);
    }
    m_customPlot->replot();
}

void PlotPolar::updateGraphByDataPair(DataPair* data, double curSecs)
{
    auto uuid = data->getUuid();
    if(!m_graphHash.contains(uuid))
    {
        DrawComponents info;
        info.graph = new QCPPolarGraph(m_angularAxis, m_angularAxis->radialAxis());
        info.graph->setBrush(Qt::NoBrush);
        info.tracerText = new QCPItemText(m_customPlot);
        info.tracerText->position->setType(QCPItemPosition::ptAbsolute);
        // 默认position的坐标是center,为了方便计算，将position的坐标改为左上角
        info.tracerText->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);

        info.pixmap = new QCPItemPixmap(m_customPlot); // 创建 QCPItemPixmap 对象
        info.pixmap->setClipToAxisRect(false); // 允许图标超出坐标轴范围
        info.pixmap->setClipAxisRect(m_customPlot->axisRect()); // 设置图标显示范围
        info.pixmap->topLeft->setType(QCPItemPosition::ptAbsolute);
        info.pixmap->setPen(Qt::NoPen); // 不显示边框

        m_graphHash.insert(uuid, info);
    }
    auto graph = m_graphHash.value(uuid).graph;
    auto tracerText = m_graphHash.value(uuid).tracerText;
    auto pixmap = m_graphHash.value(uuid).pixmap;
    if(data->isDraw())
    {
        auto x = m_dataHash.value(uuid).first;
        auto y = m_dataHash.value(uuid).second;
        if(x.isEmpty() || y.isEmpty())
        {
            graph->setVisible(false);
            return;
        }
        double secsLimit = data->getSecondsLimit();
        if(!math::doubleEqual(secsLimit, 0.0) && !math::doubleEqual(secsLimit, curSecs))
        {
            graph->setVisible(false);
            return;
        }
        int32_t pointsLimit = data->getPointsLimit();
        if(pointsLimit > 0)
        {
            x = x.mid(x.size() - pointsLimit);
            y = y.mid(y.size() - pointsLimit);
        }

        graph->setVisible(true);
        graph->setData(x, y, true);
        QPen pen(data->dataColor(), data->width());
        if(data->isLineMode())
        {
            Qt::PenStyle style = Qt::SolidLine;
            // 线性模式下才支持stipple
            if(data->getIsStippleEnable())
            {
                style = data->getStipplePattern();
            }
            pen.setStyle(style);

            graph->setScatterStyle(QCPScatterStyle::ssNone);
            graph->setLineStyle(QCPPolarGraph::lsLine);
        }
        else
        {
            graph->setScatterStyle(
                QCPScatterStyle(QCPScatterStyle::ssDisc, data->dataColor(), data->width()));
            graph->setLineStyle(QCPPolarGraph::lsNone);
        }
        graph->setPen(pen);

        double lastX = x.last();
        double lastY = y.last();
        QPointF lastPoint = m_angularAxis->coordToPixel(lastX, lastY);
        double lastPosX = lastPoint.x();
        double lastPosY = lastPoint.y();
        //icon
        if(data->isIconDraw())
        {
            auto pix = data->processIcon();
            if(!pix.isNull())
            {
                pixmap->setVisible(true);
                pixmap->setPixmap(pix);
                // 以最后一个数据点的坐标作为图片的中心点,这里不能直接使用data中原始的图片宽高作为基准，因为旋转之后，图片宽高会发生变化
                double topX = lastPosX - pix.width() / 2;
                double topY = lastPosY - pix.height() / 2;
                pixmap->topLeft->setCoords(topX, topY);
            }
        }
        else
        {
            pixmap->setVisible(false);
        }

        //Label Text
        if(data->isLabelTextShow())
        {
            tracerText->setVisible(true);
            QString text = data->processLabelText(lastX, lastY);
            tracerText->setText(text);
            QPointF labelPoint = data->processLabelPosition(QPointF(lastPosX, lastPosY), text);
            tracerText->position->setCoords(labelPoint.x(), labelPoint.y());
            tracerText->setFont(data->getLabelFont());
            tracerText->setColor(data->getLabelColor());
            tracerText->setBrush(data->getLabelBackground());
        }
        else
        {
            tracerText->setVisible(false);
        }
    }
    else
    {
        graph->setVisible(false);
        pixmap->setVisible(false);
        tracerText->setVisible(false);
    }
}

void PlotPolar::delPlotPairData(const QString& uuid)
{
    if(m_graphHash.contains(uuid))
    {
        auto draw = m_graphHash.take(uuid);
        if(!draw.graph.isNull())
        {
            m_angularAxis->removeGraph(draw.graph);
        }
        if(!draw.pixmap.isNull())
        {
            m_customPlot->removeItem(draw.pixmap);
        }
        if(!draw.tracerText.isNull())
        {
            m_customPlot->removeItem(draw.tracerText);
        }
        m_customPlot->replot();
    }
    PlotItemBase::delPlotPairData(uuid);
}
