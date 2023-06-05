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
    m_coordBgn_x = std::numeric_limits<double>::min();
    m_coordEnd_x = std::numeric_limits<double>::max();
    m_coordBgn_y = std::numeric_limits<double>::min();
    m_coordEnd_y = std::numeric_limits<double>::max();

    initPlot();
    setupLayout();
}

PlotScatter::~PlotScatter() {}

void PlotScatter::initPlot()
{
    m_customPlot = new QCustomPlot();
    m_customPlot->axisRect()->setupFullAxesBox(true);

    m_customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->xAxis->ticker()->setTickCount(m_vertGrids);
    m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
    m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
    m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);
    m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
    m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);
    m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->xAxis->setLabel(m_xAxisLabel);
    m_customPlot->yAxis->setLabel(m_yAxisLabel);

    m_customPlot->setBackground(m_outerFillColor);
    m_customPlot->axisRect()->setBackground(m_gridFillColor);
    m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
    m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
    m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
    m_customPlot->yAxis->setLabelFont(m_axisLabelFont);
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
        if(!draw.tracer.isNull())
        {
            m_customPlot->removeItem(draw.tracer);
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
        if(xAttr == "Time" && yAttr == "Time")
        {
            x = DataManager::getInstance()->getTimeDataSet();
            y = DataManager::getInstance()->getTimeDataSet();
        }
        else if(xAttr == "Time" && yAttr != "Time")
        {
            if(yAttr != "Now")
            {
                x = DataManager::getInstance()->getEntityAttrValueListByMaxTime(
                    yEntityID, xAttr, secs);
                y = DataManager::getInstance()->getEntityAttrValueListByMaxTime(
                    yEntityID, yAttr, secs);
            }
            else
            {
                //当为Now的时候y轴没有数据，只需要在X轴上显示一个Now的矩形
                m_isTimeLine = true;
            }
        }
        else if(xAttr != "Time" && yAttr == "Time")
        {
            x = DataManager::getInstance()->getEntityAttrValueListByMaxTime(xEntityID, xAttr, secs);
            y = DataManager::getInstance()->getEntityAttrValueListByMaxTime(xEntityID, yAttr, secs);
        }
        else
        {
            x = DataManager::getInstance()->getEntityAttrValueListByMaxTime(xEntityID, xAttr, secs);
            y = DataManager::getInstance()->getEntityAttrValueListByMaxTime(yEntityID, yAttr, secs);
        }
        m_dataHash.insert(uuid, qMakePair(x, y));
    }
    if(m_isTimeLine)
    {
        updateTimelineGraph();
    }
    else
    {
        for(int i = 0; i < itemCnt; ++i)
        {
            updateGraphByDataPair(m_dataPairs.at(i));
        }
    }
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::updateGraphByDataPair(DataPair* data)
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

        info.tracerText = new QCPItemText(m_customPlot);
        info.tracerText->position->setType(QCPItemPosition::ptPlotCoords);

        info.pixmap = new QCPItemPixmap(m_customPlot); // 创建 QCPItemPixmap 对象
        info.pixmap->setClipToAxisRect(false); // 允许图标超出坐标轴范围
        info.pixmap->setClipAxisRect(m_customPlot->axisRect()); // 设置图标显示范围
        info.pixmap->topLeft->setType(QCPItemPosition::ptPlotCoords);
        info.pixmap->setPen(Qt::NoPen); // 不显示边框
        m_mapScatter.insert(uuid, info);
    }
    auto graph = m_mapScatter[uuid].graph;
    auto tracerText = m_mapScatter[uuid].tracerText;
    auto pixmap = m_mapScatter[uuid].pixmap;
    if(data->isDraw())
    {

        graph->setVisible(true);
        graph->setData(x, y);

        graph->setPen(QPen(data->dataColor(), data->width()));
		//line mode
        if(data->isLineMode())
		{
            graph->setLineStyle(QCPGraph::lsLine);
            Qt::PenStyle style = Qt::SolidLine;
            // 线性模式下才支持stipple
            if(data->getIsStippleEnable())
            {
                style = data->getStipplePattern();
            }
            QPen pen = graph->pen();
            pen.setStyle(style);
            graph->setPen(pen);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
		}
		else
		{
            graph->setLineStyle(QCPGraph::lsNone);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, data->width()));
		}
		//icon
        if(data->isIconDraw())
        {
            auto pix = data->processIcon();
            if(!pix.isNull())
            {
                pixmap->setVisible(true);
                pixmap->setPixmap(pix);
                pixmap->topLeft->setCoords(x.last(), y.last());
            }
        }
        else
        {
            pixmap->setVisible(false);
        }

        //Label Text
        if(data->isLabelTextShow())
        {
            // 游标功能先注释掉
            tracerText->setVisible(false);
            QString text = data->processLabelText(x.last(), y.last());
            tracerText->setText(text);
            tracerText->position->setCoords(x.last(), y.last());
            auto alignFlag = data->processLabelTextPosition();
            tracerText->setTextAlignment(alignFlag);
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
        tracerText->setVisible(false);
    }
}

void PlotScatter::exportDataToFile(const QString& filename) const
{
    QFile file(filename);
    if(file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&file); //创建写入流
        for(const auto& info : m_mapScatter)
        {
            if(info.graph && info.graph->visible())
            {
                out << "[" << info.tracerText->text() << "]"
                    << "\r\n";
                out << "Title=" << info.tracerText->text() << "\r\n";
                out << "Data="
                    << "\r\n";
                auto dataContainer = info.graph->data().get();
                auto size = dataContainer->size();
                for(int var = 0; var < size; ++var)
                {
                    double x = dataContainer->at(var)->key;
                    double y = dataContainer->at(var)->value;
                    out << x << "       " << y << "\r\n";
                }
                out << "\r\n";
            }
        }
    }
    file.close();
}

void PlotScatter::setAxisVisible(bool on, AxisType type)
{
    switch(type)
    {
    case AxisType::xAxis:
        m_customPlot->xAxis->setVisible(on);
        break;
    case AxisType::yAxis:
        m_customPlot->yAxis->setVisible(on);
        break;
    case AxisType::xAxis2:
        m_customPlot->xAxis2->setVisible(on);
        break;
    case AxisType::yAxis2:
        m_customPlot->yAxis2->setVisible(on);
        break;
    default:
        break;
    }
    m_customPlot->replot();
}

void PlotScatter::setAxisTickLabelShow(bool on, AxisType type)
{
    switch(type)
    {
    case AxisType::xAxis:
        m_customPlot->xAxis->setTickLabels(on);
        break;
    case AxisType::yAxis:
        m_customPlot->yAxis->setTickLabels(on);
        break;
    case AxisType::xAxis2:
        m_customPlot->xAxis2->setTickLabels(on);
        break;
    case AxisType::yAxis2:
        m_customPlot->yAxis2->setTickLabels(on);
        break;
    default:
        break;
    }
    m_customPlot->replot();
}

DataPair* PlotScatter::addPlotDataPair(int32_t xEntityID,
                                       const QString& xAttrName,
                                       const QString& xAttrUnitName,
                                       int32_t yEntityID,
                                       const QString& yAttrName,
                                       const QString& yAttrUnitName,
                                       const QHash<QString, QVariant>& extraParams)
{
    // 更新range
    QPair<double, double> xlimit =
        DataManager::getInstance()->getMaxAndMinEntityAttrValue(xEntityID, xAttrName);

    // x轴
    if(math::doubleEqual(m_coordBgn_x, std::numeric_limits<double>::min()))
    {
        // 表示m_min数值无意义，先赋值
        m_coordBgn_x = xlimit.first;
    }
    else
    {
        m_coordBgn_x = m_coordBgn_x < xlimit.first ? m_coordBgn_x : xlimit.first;
    }

    if(math::doubleEqual(m_coordEnd_x, std::numeric_limits<double>::max()))
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
    if(math::doubleEqual(m_coordBgn_y, std::numeric_limits<double>::min()))
    {
        // 表示m_min数值无意义，先赋值
        m_coordBgn_y = ylimit.first;
    }
    else
    {
        m_coordBgn_y = m_coordBgn_y < ylimit.first ? m_coordBgn_y : ylimit.first;
    }

    if(math::doubleEqual(m_coordEnd_y, std::numeric_limits<double>::max()))
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
    return PlotItemBase::addPlotDataPair(
        xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName, extraParams);
}

QPair<double, double> PlotScatter::processLabelTextPosition(const QString& text, DataPair* data)
{
    QFontMetricsF fm(data->getLabelFont());
    double wd = (fm.size(Qt::TextSingleLine, text).width()) / 3.0;
    double ht = fm.size(Qt::TextSingleLine, text).height() / 1.0;
    QPair<double, double> pair;
    switch(data->getLabelPosition())
    {
    case DataPair::left_top: //left-top
        pair = qMakePair(-wd, -ht);
        break;
    case DataPair::top: //top
        pair = qMakePair(0, -ht);
        break;
    case DataPair::right_top: //right-top
        pair = qMakePair(wd, -ht);
        break;
    case DataPair::left: //left
        pair = qMakePair(-wd, 0);
        break;
    case DataPair::center: //center
        pair = qMakePair(0, 0);
        break;
    case DataPair::right: //right
        pair = qMakePair(wd, 0);
        break;
    case DataPair::left_bottom: //left-bottom
        pair = qMakePair(-wd, ht);
        break;
    case DataPair::bottom: //bottom
        pair = qMakePair(0, ht);
        break;
    case DataPair::right_bottom: //right-bottom
        pair = qMakePair(wd, ht);
        break;
    default: //right
        pair = qMakePair(wd, 0);
        break;
    }
    return pair;
}

void PlotScatter::clearEventText()
{
    for(auto text : m_eventList)
    {
        m_customPlot->removeItem(text);
    }
    m_eventList.clear();
}

void PlotScatter::updateTimelineGraph()
{
    if(!m_timelineGraph)
    {
        m_timelineGraph = m_customPlot->addGraph();
        m_timelineGraph->setVisible(true);
        QVector<double> timeVec = DataManager::getInstance()->getTimeDataSet();
        if(!timeVec.isEmpty())
        {
            m_customPlot->xAxis->setRange(timeVec.first(), timeVec.last());
            m_customPlot->xAxis->setLabel("Time(s)");
            m_customPlot->yAxis->setRange(0, 1);
            m_customPlot->yAxis->setLabel("All Platforms");
        }
        m_customPlot->yAxis->setTickLabels(false);
    }
    m_customPlot->yAxis->grid()->setVisible(false);
    if(!m_timelineNowRect)
    {
        m_timelineNowRect = new QCPItemRect(m_customPlot);
        m_timelineNowRect->setBrush(Qt::gray);
        QPen pen;
        pen.setColor(Qt::gray);
        m_timelineNowRect->setPen(pen);
    }
    double now = PlotXYDemo::getSeconds();
    // Now矩形高度占比0.9
    double centerX = m_customPlot->xAxis->coordToPixel(now);
    double rectTop = m_customPlot->yAxis->coordToPixel(0.9);
    double bottom = m_customPlot->yAxis->coordToPixel(0.0);
    m_timelineNowRect->topLeft->setType(QCPItemPosition::ptAbsolute);
    m_timelineNowRect->topLeft->setCoords(centerX - 10, rectTop);
    m_timelineNowRect->bottomRight->setType(QCPItemPosition::ptAbsolute);
    m_timelineNowRect->bottomRight->setCoords(centerX + 10, bottom);

    if(!m_timelineNowText)
    {
        m_timelineNowText = new QCPItemText(m_customPlot);
        m_timelineNowText->setText("Now");
        m_timelineNowText->setColor(Qt::gray);
        m_timelineNowText->setTextAlignment(Qt::AlignCenter);
    }
    double textTop = m_customPlot->yAxis->coordToPixel(0.98);
    m_timelineNowText->position->setType(QCPItemPosition::ptAbsolute);

    m_timelineNowText->position->setCoords(centerX + 10, textTop);
    clearEventText();

    auto eventList = getEventList();
    // 上下预留0.1，中间区域占比0.8
    double heightDelta = 0.8 / eventList.size();
    int32_t index = 0;
    for(auto& event : eventList)
    {

        auto dataList = DataManagerInstance->getGenericDataListByID(event.m_entityID);
        //如果x轴是time，那么需要绘制事件标签，整个用一个Text显示
        for(const auto& data : dataList)
        {

            QCPItemText* textItem = new QCPItemText(m_customPlot);
            QString text;
            if(event.m_eventStyle == "Small X")
            {
                text = "X ";
            }
            else
            {
                text = "| ";
            }
            if(event.m_isIncludeTag)
            {
                text.append(data.m_name);
            }
            text.append(QString("(%1s)").arg(data.m_relativeTime));
            textItem->setText(text);
            QFont font;
            font.setFamily(event.m_eventFontFamily);
            font.setPixelSize(event.m_eventFontSize);
            textItem->setColor(QColor(event.m_eventColor));
            // x轴需要设置到对应的时间坐标上，y轴需要按照像素坐标从低到高排列,目前暂时设置到0.5垂直居中
            textItem->position->setTypeX(QCPItemPosition::ptPlotCoords);
            textItem->position->setTypeY(QCPItemPosition::ptAxisRectRatio);
            textItem->position->setCoords(data.m_relativeTime, 0.1 + index * heightDelta);
            m_eventList.append(textItem);
        }
        ++index;
    }

    m_customPlot->replot();
}
