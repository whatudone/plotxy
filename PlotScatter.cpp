#include "PlotScatter.h"
#include "DataManager.h"
#include "PlotXYDemo.h"
#include "Utils.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QPainter>
#include <QTime>

int PlotScatter::m_instanceCount = 1;

PlotScatter::PlotScatter(QWidget* parent)
    : PlotItemBase(parent)
{
    QString name = QString("Scatter%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_title = "Scatter Plot";

    m_xAxisLabel = "X Axis";
    m_yAxisLabel = "Y Axis";

    m_showUnits_x = false;
    m_showUnits_y = false;

    initPlot();
    setupLayout();
}

PlotScatter::~PlotScatter() {}

void PlotScatter::initPlot()
{
    m_customPlot = new QCustomPlot();
    m_customPlot->xAxis->setRange(m_coordBgn_x, m_coordEnd_x);
    m_customPlot->yAxis->setRange(m_coordBgn_y, m_coordEnd_y);
    // 坐标轴范围切换之后，需要更新背景分段坐标信息
    connect(m_customPlot->xAxis, QOverload<const QCPRange&>::of(&QCPAxis::rangeChanged), [this]() {
        updateBackgroundColorSeg();
        if(m_isTimeLine)
        {
            updateTimelineGraph();
        }
    });
    connect(m_customPlot->yAxis, QOverload<const QCPRange&>::of(&QCPAxis::rangeChanged), [this]() {
        updateBackgroundColorSeg();
    });
    m_customPlot->axisRect()->setupFullAxesBox(true);

    m_customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->xAxis->ticker()->setTickCount(m_vertGrids);
    m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
    m_customPlot->xAxis->setTickLabelColor(m_xTickLabelColor);
    m_customPlot->yAxis->setTickLabelColor(m_yTickLabelColor);
    m_customPlot->xAxis->setTickLabelFont(m_xTickLabelFont);
    m_customPlot->yAxis->setTickLabelFont(m_yTickLabelFont);
    m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->xAxis->setLabel(m_xAxisLabel);
    m_customPlot->yAxis->setLabel(m_yAxisLabel);

    m_customPlot->setBackground(m_gridFillColor);
    m_customPlot->axisRect()->setBackground(m_gridFillColor);
    m_customPlot->xAxis->setLabelColor(m_xAxisLabelColor);
    m_customPlot->yAxis->setLabelColor(m_yAxisLabelColor);
    m_customPlot->xAxis->setLabelFont(m_xAxisLabelFont);
    m_customPlot->yAxis->setLabelFont(m_yAxisLabelFont);
}

void PlotScatter::delPlotPairData(const QString& uuid)
{
    //scatter
    if(m_mapScatter.contains(uuid))
    {
        auto draw = m_mapScatter.take(uuid);
        if(!draw.graph.isNull())
        {
            m_customPlot->removeGraph(draw.graph);
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
    if(m_dataPairs.isEmpty())
    {
        m_coordBgn_x = std::numeric_limits<double>::min();
        m_coordEnd_x = std::numeric_limits<double>::max();
        m_coordBgn_y = std::numeric_limits<double>::min();
        m_coordEnd_y = std::numeric_limits<double>::max();
    }
}

void PlotScatter::updateDataForDataPairsByTime(double secs)
{
    m_dataHash.clear();
    m_lastDataHash.clear();
    m_isTimeLine = false;
    int itemCnt = getDataPairs().size();

    for(int i = 0; i < itemCnt; ++i)
    {
        QVector<double> x;
        QVector<double> y;
        auto data = getDataPairs().at(i);
        QString uuid = data->getUuid();
        auto xEntityID = data->getEntityIDX();
        auto yEntityID = data->getEntityIDY();
        auto xAttr = data->getAttr_x();
        auto yAttr = data->getAttr_y();
        // 散点图有几种模式，时间模式只有时间属性
        if(xAttr == "Time" && yAttr == "Now")
        {
            //当为Now的时候y轴没有数据，只需要在X轴上显示一个Now的矩形
            m_isTimeLine = true;
        }
        else
        {
            if(xAttr == "Time" && yAttr == "Time")
            {
                x = DataManager::getInstance()->getTimeDataSet();
                y = DataManager::getInstance()->getTimeDataSet();
            }
            else if(xAttr == "Time" && yAttr != "Time")
            {

                x = DataManager::getInstance()->getEntityAttrValueListByMaxTime(
                    yEntityID, xAttr, secs);
                y = DataManager::getInstance()->getEntityAttrValueListByMaxTime(
                    yEntityID, yAttr, secs);
            }
            else if(xAttr != "Time" && yAttr == "Time")
            {
                x = DataManager::getInstance()->getEntityAttrValueListByMaxTime(
                    xEntityID, xAttr, secs);
                y = DataManager::getInstance()->getEntityAttrValueListByMaxTime(
                    xEntityID, yAttr, secs);
            }
            else
            {
                x = DataManager::getInstance()->getEntityAttrValueListByMaxTime(
                    xEntityID, xAttr, secs);
                y = DataManager::getInstance()->getEntityAttrValueListByMaxTime(
                    yEntityID, yAttr, secs);
            }
            m_dataHash.insert(uuid, qMakePair(x, y));
            if(!x.isEmpty() && !y.isEmpty())
            {
                m_lastDataHash.insert(uuid, QPointF(x.last(), y.last()));
            }
        }
    }
    if(m_isTimeLine)
	{
        updateTimelineGraph();
    }
    else
    {
        for(int i = 0; i < itemCnt; ++i)
        {
            updateGraphByDataPair(m_dataPairs.at(i), secs);
        }
    }
    updateMarkers(secs);
    updateConnectionLines();
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::updateGraphByDataPair(DataPair* data, double curSecs)
{
    if(!data)
	{
        return;
    }
    if(m_isTimeLine)
	{
        updateTimelineGraph();
        return;
    }
    auto uuid = data->getUuid();
    auto x = m_dataHash.value(uuid).first;
    auto y = m_dataHash.value(uuid).second;
    if(x.isEmpty() || y.isEmpty())
    {
        return;
    }
    DrawComponents info;
    if(!m_mapScatter.contains(uuid))
    {
        info.graph = m_customPlot->addGraph();
        // 默认采样值是true，在某些情况下采样会导致把原始数据处理错误，导致连线时路径错误
        info.graph->setAdaptiveSampling(false);
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
        m_mapScatter.insert(uuid, info);
    }
    auto graph = m_mapScatter[uuid].graph;
    auto tracerText = m_mapScatter[uuid].tracerText;
    auto pixmap = m_mapScatter[uuid].pixmap;
    if(data->isDraw())
    {
        double secsLimit = data->getSecondsLimit();
        if(!math::doubleEqual(secsLimit, 0.0) && !math::doubleEqual(secsLimit, curSecs))
        {
            graph->setVisible(false);
            tracerText->setVisible(false);
            pixmap->setVisible(false);
            return;
        }
        int32_t pointsLimit = data->getPointsLimit();
        if(pointsLimit > 0)
        {
            x = x.mid(x.size() - pointsLimit);
            y = y.mid(y.size() - pointsLimit);
        }
        graph->setVisible(true);
        // 第三个参数设置为true，禁止内部对数据根据x轴的数值大小进行排序，导致数据插入顺序不对，出现line模式连线不对
        graph->setData(x, y, true);
        QPen pen(data->dataColor(), data->width());
		//line mode
        if(data->isLineMode())
		{
            Qt::PenStyle style = Qt::SolidLine;
            // 线性模式下才支持stipple
            if(data->getIsStippleEnable())
            {
                style = data->getStipplePattern();
            }

            pen.setStyle(style);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle::ssNone);
		}
		else
		{
            graph->setLineStyle(QCPGraph::lsNone);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, data->width()));
		}
        graph->setPen(pen);
        double lastX = x.last();
        double lastY = y.last();
        double lastPosX = m_customPlot->xAxis->coordToPixel(lastX);
        double lastPosY = m_customPlot->yAxis->coordToPixel(lastY);
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

void PlotScatter::exportDataToFile(const QString& filename) const
{
    //    QFile file(filename);
    //    if(file.open(QFile::WriteOnly | QFile::Truncate))
    //    {
    //        QTextStream out(&file); //创建写入流
    //        for(const auto& info : m_mapScatter)
    //        {
    //            if(info.graph && info.graph->visible())
    //            {
    //                out << "[" << info.tracerText->text() << "]"
    //                    << "\r\n";
    //                out << "Title=" << info.tracerText->text() << "\r\n";
    //                out << "Data="
    //                    << "\r\n";
    //                auto dataContainer = info.graph->data().get();
    //                auto size = dataContainer->size();
    //                for(int var = 0; var < size; ++var)
    //                {
    //                    double x = dataContainer->at(var)->key;
    //                    double y = dataContainer->at(var)->value;
    //                    out << x << "       " << y << "\r\n";
    //                }
    //                out << "\r\n";
    //            }
    //        }
    //    }
    //    file.close();
}

void PlotScatter::drawGOGData()
{
    QList<QString> keyList = m_gogDataMap.keys();
    for(auto graph : m_gogGraphList)
    {
        m_customPlot->removeGraph(graph);
    }
    for(auto item : m_gogEllipseList)
    {
        m_customPlot->removeItem(item);
    }
    m_gogGraphList.clear();
    m_gogEllipseList.clear();
    for(auto fileName : keyList)
    {
        GOGCustomSetting setting = m_gogCustomSettings.value(fileName);
        QList<GOGDataInfo> dataList = m_gogDataMap[fileName];

        if(setting.isDraw)
        {
            for(auto data : dataList)
            {
                //                bool isFilled = false;
                //                QColor fillColor;
                //                if(setting.fillState == Qt::Checked)
                //                {
                //                    isFilled = true;
                //                    fillColor = setting.fillColor;
                //                }
                //                else if(setting.fillState == Qt::Unchecked)
                //                {
                //                    isFilled = false;
                //                }
                //                else if(setting.fillState == Qt::PartiallyChecked)
                //                {
                //                    if(data.isFill)
                //                    {
                //                        isFilled = true;
                //                        fillColor = data.fillColor;
                //                    }
                //                    else
                //                    {
                //                        isFilled = false;
                //                    }
                //                }

                QColor fillColor;
                if(setting.fillColor.isValid())
                {
                    fillColor = setting.fillColor;
                }
                else
                {
                    fillColor = data.fillColor;
                }

                int lineWidth = setting.lineWidth == 0 ? data.lineWidth : setting.lineWidth;
                if(data.type == "line")
                {
                    QCPGraph* graph = m_customPlot->addGraph();
                    //                    if(isFilled)
                    //                        graph->setBrush(fillColor);
                    //                    else
                    graph->setBrush(Qt::NoBrush);
                    graph->setAdaptiveSampling(false);
                    graph->setVisible(true);
                    graph->setPen(QPen(data.lineColor, lineWidth));
                    graph->setLineStyle(QCPGraph::lsLine);
                    graph->setScatterStyle(QCPScatterStyle::ssNone);
                    graph->setData(data.xList, data.yList, true);
                    m_gogGraphList.append(graph);
                }
                else if(data.type == "circle")
                {
                    QCPItemEllipse* ellipse = new QCPItemEllipse(m_customPlot);
                    double radius = 0.0;
                    // GOG的数据内容都是基于经纬度的数据点，所以只需要考虑rangeUnits的单位，然后做出对应的单位转换即可
                    if(data.rangeUnits == "meters")
                    {
                        radius = data.radius / 110000;
                    }
                    else
                    {
                        radius = data.radius;
                    }
                    ellipse->topLeft->setCoords(data.xList.at(0) - radius,
                                                data.yList.at(0) + radius);
                    ellipse->bottomRight->setCoords(data.xList.at(0) + radius,
                                                    data.yList.at(0) - radius);
                    ellipse->setPen(QPen(QColor(fillColor), lineWidth));
                    //                    if(isFilled)
                    //                        ellipse->setBrush(QColor(fillColor));
                    //                    else
                    ellipse->setBrush(data.fillColor);
                    m_gogEllipseList.append(ellipse);
                }
            }
        }
    }
    m_customPlot->replot();
}

void PlotScatter::addBackgroundColorInfo(const QString& limitName,
                                         double limitValue,
                                         const QColor& lineColor,
                                         uint32_t lineWidth,
                                         const QColor& fillAboveColor,
                                         const QColor& fillBelowColor)
{
    if(!m_bkgLimitSegMap.contains(limitValue))
    {
        BackgroundLimitSeg seg;
        seg.m_limitName = limitName;
        seg.m_limitValue = limitValue;
        seg.m_lineColor = lineColor;
        seg.m_lineWidth = lineWidth;
        seg.m_fillAboveColor = fillAboveColor;
        seg.m_fillBelowColor = fillBelowColor;
        m_bkgLimitSegMap.insert(limitValue, seg);
    }
    updateBackgroundColorSeg();
}

void PlotScatter::removeBackgroundColorInfo(double value)
{
    if(m_bkgLimitSegMap.contains(value))
    {
        m_bkgLimitSegMap.remove(value);
    }
    updateBackgroundColorSeg();
}

DataPair* PlotScatter::addPlotDataPair(int32_t xEntityID,
                                       const QString& xAttrName,
                                       const QString& xAttrUnitName,
                                       int32_t yEntityID,
                                       const QString& yAttrName,
                                       const QString& yAttrUnitName,
                                       const QHash<QString, QVariant>& extraParams,
                                       bool isFromJson)
{
    if(!isFromJson)
    {

        // 更新range
        QPair<double, double> xlimit;
        if(xAttrName == "Time")
        {
            xlimit = DataManager::getInstance()->getMaxAndMinEntityAttrValue(yEntityID, xAttrName);
        }
        else
        {
            xlimit = DataManager::getInstance()->getMaxAndMinEntityAttrValue(xEntityID, xAttrName);
        }

        // x轴
        if(!m_isInitCoorRange)
        {
            // 表示m_min数值无意义，先赋值
            m_coordBgn_x = xlimit.first;
        }
        else
        {
            m_coordBgn_x = m_coordBgn_x < xlimit.first ? m_coordBgn_x : xlimit.first;
        }

        if(!m_isInitCoorRange)
        {
            // 表示m_max数值无意义，先赋值
            m_coordEnd_x = xlimit.second;
        }
        else
        {
            m_coordEnd_x = m_coordEnd_x > xlimit.second ? m_coordEnd_x : xlimit.second;
        }

        // y轴
        QPair<double, double> ylimit =
            DataManager::getInstance()->getMaxAndMinEntityAttrValue(yEntityID, yAttrName);
        if(!m_isInitCoorRange)
        {
            // 表示m_min数值无意义，先赋值
            m_coordBgn_y = ylimit.first;
        }
        else
        {
            m_coordBgn_y = m_coordBgn_y < ylimit.first ? m_coordBgn_y : ylimit.first;
        }

        if(!m_isInitCoorRange)
        {
            // 表示m_max数值无意义，先赋值
            m_coordEnd_y = ylimit.second;
        }
        else
        {
            m_coordEnd_y = m_coordEnd_y > ylimit.second ? m_coordEnd_y : ylimit.second;
        }

        m_customPlot->xAxis->setRange(m_coordBgn_x, m_coordEnd_x);
        m_customPlot->yAxis->setRange(m_coordBgn_y, m_coordEnd_y);
        m_units_x = xAttrUnitName;
        m_units_y = yAttrUnitName;
        setxAxisLabel(xAttrName);
        setyAxisLabel(yAttrName);
        setTitle(QString("%1 VS. %2").arg(xAttrName).arg(yAttrName));
    }
    m_isInitCoorRange = true;
    return PlotItemBase::addPlotDataPair(
        xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName, extraParams);
}

void PlotScatter::clearEventText()
{
    for(auto text : m_eventList)
    {
        m_customPlot->removeItem(text);
    }
    m_eventList.clear();
}

void PlotScatter::clearHistoryLines()
{
    for(const auto& draw : m_mapScatter)
    {
        m_customPlot->removeGraph(draw.graph);
        m_customPlot->removeItem(draw.pixmap);
        m_customPlot->removeItem(draw.tracerText);
    }
    m_mapScatter.clear();
}

void PlotScatter::updateTimelineGraph()
{
    if(math::doubleEqual(m_lastTime, std::numeric_limits<double>::max()))
    {
        m_lastTime = PlotXYDemo::getSeconds();
    }
    //Timeline模式 Now和event标签都是不移动，只是会移动坐标轴范围
    if(!m_timelineGraph)
    {
        m_timelineGraph = m_customPlot->addGraph();
        m_timelineGraph->setVisible(true);
        QVector<double> timeVec = DataManager::getInstance()->getTimeDataSet();
        if(!timeVec.isEmpty())
        {
            setCoordRangeX(timeVec.first(), timeVec.last());
        }
        else
        {
            setCoordRangeX(0, 10000);
        }

        m_customPlot->xAxis->setLabel("Time(s)");
        setCoordRangeY(0.0, 1.0);
        m_customPlot->yAxis->setLabel("All Platforms");
        m_customPlot->yAxis->setTickLabels(false);
    }
    m_customPlot->yAxis->grid()->setVisible(false);
    double now = PlotXYDemo::getSeconds();
    if(!m_timelineNowRect)
    {
        m_timelineNowRect = new QCPItemRect(m_customPlot);
        m_timelineNowRect->setBrush(Qt::gray);
        QPen pen;
        pen.setColor(Qt::gray);
        m_timelineNowRect->setPen(pen);
        m_timelineNowRect->topLeft->setType(QCPItemPosition::ptAbsolute);
        m_timelineNowRect->bottomRight->setType(QCPItemPosition::ptAbsolute);
    }

    // Now矩形高度占比0.9
    double centerX = m_customPlot->xAxis->coordToPixel(now);
    double rectTop = m_customPlot->yAxis->coordToPixel(0.95);
    double bottom = m_customPlot->yAxis->coordToPixel(0.0);
    m_timelineNowRect->topLeft->setCoords(centerX - 10, rectTop);
    m_timelineNowRect->bottomRight->setCoords(centerX + 10, bottom);

    if(!m_timelineNowText)
    {
        m_timelineNowText = new QCPItemText(m_customPlot);
        m_timelineNowText->setText("Now");
        m_timelineNowText->setColor(Qt::gray);
        m_timelineNowText->setTextAlignment(Qt::AlignCenter);
        m_timelineNowText->position->setType(QCPItemPosition::ptAbsolute);
    }
    double textTop = m_customPlot->yAxis->coordToPixel(0.98);
    m_timelineNowText->position->setCoords(centerX, textTop);

    clearEventText();

    auto eventList = getEventList();
    // 上下预留0.1，中间区域占比0.8
    double heightDelta = 0.8 / eventList.size();
    int32_t index = 0;
    for(auto& event : eventList)
    {
        auto dataList = DataManagerInstance->getGenericDataListByID(event.m_entityID, event.m_type);
        //如果x轴是time，那么需要绘制事件标签，整个用一个Text显示
        for(const auto& data : dataList)
        {

            QCPItemText* textItem = new QCPItemText(m_customPlot);
            QString text;
            if(event.m_eventStyle == "Small X")
            {
                text = "× ";
            }
            else
            {
                text = "| ";
            }
            if(event.m_isIncludeTag)
            {
                // 离线数据中eventname包含了平台信息，在线数据中需要自己拼接
                if(DataManagerInstance->getIsRealTime())
                {
                    text.append(data.m_platName + data.m_eventName);
                }
                else
                {
                    text.append(data.m_eventName);
                }
            }
            text.append(QString("(%1s)").arg(data.m_relativeTime));
            textItem->setText(text);
            QFont font;
            font.setFamily(event.m_eventFontFamily);
            font.setPixelSize(event.m_eventFontSize);
            textItem->setFont(font);
            textItem->setColor(event.m_eventColor);
            // x轴需要设置到对应的时间坐标上，y轴需要按照像素坐标从低到高排列,目前暂时设置到0.5垂直居中
            textItem->position->setTypeX(QCPItemPosition::ptPlotCoords);
            textItem->position->setTypeY(QCPItemPosition::ptAxisRectRatio);
            textItem->position->setCoords(data.m_relativeTime, 0.1 + index * heightDelta);
            m_eventList.append(textItem);
        }
        ++index;
    }
    // 在线模式试试刷新最新时间轴范围，起点和终点同步更新
    double delta = now - m_lastTime;
    double timeBegin = m_customPlot->xAxis->range().lower + delta;
    double timeEnd = m_customPlot->xAxis->range().upper + delta;
    m_lastTime = now;
    setCoordRangeX(timeBegin, timeEnd);
    m_customPlot->replot();
}

void PlotScatter::updateBackgroundColorSeg()
{
    // 清理历史背景分段信息
    for(auto rect : m_backSegRectList)
    {
        m_customPlot->removeItem(rect);
    }
    m_backSegRectList.clear();
    if(m_bkgLimitSegMap.isEmpty())
    {
        m_customPlot->replot();
        return;
    }
    // 已经对分段进行排序，从小到大,先绘制最下层的背景分段，然后循环绘制上层背景分段
    QList<double> valueVec = m_bkgLimitSegMap.keys();
    double value = valueVec.at(0);
    QCPItemRect* bottomRect = new QCPItemRect(m_customPlot);
    bottomRect->topLeft->setTypeX(QCPItemPosition::ptAxisRectRatio);
    bottomRect->topLeft->setTypeY(QCPItemPosition::ptPlotCoords);
    bottomRect->topLeft->setCoords(0.0, value);
    bottomRect->bottomRight->setTypeX(QCPItemPosition::ptPlotCoords);
    bottomRect->bottomRight->setTypeY(QCPItemPosition::ptPlotCoords);
    bottomRect->bottomRight->setCoords(m_customPlot->xAxis->range().upper,
                                       m_customPlot->yAxis->range().lower);
    bottomRect->setBrush(m_bkgLimitSegMap.value(value).m_fillBelowColor);
    // 将rect放置到最底层进行绘制，避免遮挡其他元素
    bottomRect->setLayer("background");
    m_backSegRectList.append(bottomRect);

    int32_t size = valueVec.size();
    for(int var = 0; var < size; ++var)
    {
        QCPItemRect* topRect = new QCPItemRect(m_customPlot);
        topRect->topLeft->setTypeX(QCPItemPosition::ptAxisRectRatio);
        topRect->topLeft->setTypeY(QCPItemPosition::ptPlotCoords);
        double topValue = 0.0;
        if(valueVec.size() > (var + 1))
        {
            topValue = std::min(valueVec.at(var + 1), m_customPlot->yAxis->range().upper);
        }
        else
        {
            topValue = m_customPlot->yAxis->range().upper;
        }
        double curValue = valueVec.at(var);
        topRect->topLeft->setCoords(0, topValue);
        topRect->bottomRight->setTypeX(QCPItemPosition::ptAxisRectRatio);
        topRect->bottomRight->setTypeY(QCPItemPosition::ptPlotCoords);
        topRect->bottomRight->setCoords(1, curValue);
        auto seg = m_bkgLimitSegMap.value(curValue);

        topRect->setBrush(seg.m_fillAboveColor);
        topRect->setLayer("background");
        m_backSegRectList.append(topRect);

        QCPItemLine* line = new QCPItemLine(m_customPlot);
        line->setPen(QPen(seg.m_lineColor, seg.m_lineWidth));
        line->start->setType(QCPItemPosition::ptPlotCoords);
        line->start->setCoords(m_customPlot->xAxis->range().lower, curValue);
        line->end->setType(QCPItemPosition::ptPlotCoords);
        line->end->setCoords(m_customPlot->xAxis->range().upper, curValue);
        line->setLayer("background");
        m_backSegRectList.append(line);
    }

    m_customPlot->replot();
}

void PlotScatter::updateMarkers(double currentSeconds)
{
    for(auto textItem : m_plotMarkerItems)
    {
        m_customPlot->removeItem(textItem);
    }
    m_plotMarkerItems.clear();
    if(m_plotMarkers.isEmpty())
    {
        m_customPlot->replot();
        return;
    }
    for(const auto& marker : m_plotMarkers)
    {
        if(marker.time > currentSeconds)
        {
            continue;
        }
        QCPItemText* textItem = new QCPItemText(m_customPlot);
        textItem->setText("× " + marker.text);
        QFont font;
        font.setFamily(marker.fontFamily);
        font.setPixelSize(marker.fontSize);
        textItem->setFont(font);
        textItem->setColor(marker.color);
        textItem->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
        // x轴需要设置到对应的时间坐标上，y轴需要按照像素坐标从低到高排列,目前暂时设置到0.5垂直居中
        textItem->position->setType(QCPItemPosition::ptPlotCoords);
        textItem->position->setCoords(marker.x, marker.y);
        m_plotMarkerItems.insert(marker.uuid, textItem);
    }
    m_customPlot->replot();
}

void PlotScatter::updateConnectionLines()
{
    // 清理历史连线
    for(auto lines : m_connectionLines)
    {
        m_customPlot->removeItem(lines);
    }
    m_connectionLines.clear();
    for(const auto& setting : m_conHash)
    {

        if(m_lastDataHash.contains(setting.startDataPairUuid) &&
           m_lastDataHash.contains(setting.endDataPairUuid))
        {
            QPointF startPoint = m_lastDataHash.value(setting.startDataPairUuid);
            QPointF endPoint = m_lastDataHash.value(setting.endDataPairUuid);
            QCPItemLine* line = new QCPItemLine(m_customPlot);
            QPen pen(setting.color, setting.width);
            line->setPen(pen);
            line->start->setType(QCPItemPosition::ptPlotCoords);
            line->end->setType(QCPItemPosition::ptPlotCoords);
            line->start->setCoords(startPoint);
            line->end->setCoords(endPoint);

            m_connectionLines.append(line);
        }
    }
    m_customPlot->replot();
}

QHash<QString, ConnectionSetting> PlotScatter::getConHash() const
{
    return m_conHash;
}

void PlotScatter::setConHash(const QHash<QString, ConnectionSetting>& conHash)
{
    m_conHash = conHash;
}

QHash<QString, PlotMarker> PlotScatter::getPlotMarkers() const
{
    return m_plotMarkers;
}

PlotMarker PlotScatter::getMarkerByUuid(const QString& uuid) const
{
    return m_plotMarkers.value(uuid);
}

void PlotScatter::addConnection(const ConnectionSetting& con)
{
    if(!m_conHash.contains(con.uuid))
    {
        m_conHash.insert(con.uuid, con);
        updateConnectionLines();
    }
}

void PlotScatter::removeConnection(const QString& uuid)
{
    if(m_conHash.contains(uuid))
    {
        m_conHash.remove(uuid);
        updateConnectionLines();
    }
}

void PlotScatter::updateConnection(
    const QString& uuid, const QColor& color, int32_t width, const QString& stipple, int32_t speed)
{
    if(m_conHash.contains(uuid))
    {
        m_conHash[uuid].color = color;
        m_conHash[uuid].width = width;
        m_conHash[uuid].stipple = stipple;
        m_conHash[uuid].speed = speed;
        updateConnectionLines();
    }
}

QMap<double, PlotScatter::BackgroundLimitSeg> PlotScatter::getBkgLimitSegMap() const
{
    return m_bkgLimitSegMap;
}

void PlotScatter::addMarker(const PlotMarker& marker)
{
    if(!m_plotMarkers.contains(marker.uuid))
    {
        m_plotMarkers.insert(marker.uuid, marker);
        updateMarkers(PlotXYDemo::getSeconds());
    }
}

void PlotScatter::removeMarker(const QString& uuid)
{
    if(m_plotMarkers.contains(uuid))
    {
        m_plotMarkers.remove(uuid);
        updateMarkers(PlotXYDemo::getSeconds());
    }
}

void PlotScatter::modifyMarker(const QString& uuid, const PlotMarker& marker)
{
    if(m_plotMarkers.contains(marker.uuid))
    {
        m_plotMarkers.insert(uuid, marker);
        updateMarkers(PlotXYDemo::getSeconds());
    }
}
