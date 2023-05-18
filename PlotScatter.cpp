#include "PlotScatter.h"
#include "DataManager.h"
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
    if(m_dataPairs.isEmpty())
        return;
    //scatter
    if(m_mapScatter.contains(uuid))
    {
        m_mapScatter.remove(uuid);
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
            x = DataManager::getInstance()->getEntityAttrValueListByMaxTime(yEntityID, xAttr, secs);
            y = DataManager::getInstance()->getEntityAttrValueListByMaxTime(yEntityID, yAttr, secs);
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
    for(int i = 0; i < itemCnt; ++i)
    {
        updateGraphByDataPair(m_dataPairs.at(i));
	}

	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::updateGraphByDataPair(DataPair* data)
{
    if(!data)
    {
        return;
    }
    DrawComponents info;
    auto uuid = data->getUuid();
    if(!m_mapScatter.contains(uuid))
	{
        info.graph = m_customPlot->addGraph();
        //        info.tracer = new QCPItemTracer(m_customPlot);
        //        info.tracer->setGraph(info.graph);
        //        info.tracer->setInterpolating(false);
        //        info.tracer->setStyle(QCPItemTracer::tsNone);

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
        auto x = m_dataHash.value(uuid).first;
        auto y = m_dataHash.value(uuid).second;
        if(x.isEmpty() || y.isEmpty())
            return;
        graph->setData(x, y);

        graph->setVisible(true);
        graph->setPen(QPen(data->dataColor(), data->lineWidth()));
		//line mode
        if(data->isLineMode())
		{
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
		}
		else
		{
            graph->setLineStyle(QCPGraph::lsNone);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, data->lineWidth()));
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

            //			m_mapScatter[dataPair].tracer->setVisible(true);
            tracerText->setVisible(true);
			//设置锚点
            //			m_mapScatter[dataPair].tracer->setGraphKey(x.last());
            QString text = data->processLabelText(x.last(), y.last());
            tracerText->setText(text);
            auto pair = data->processLabelTextPosition(text);
            tracerText->position->setCoords(pair.first, pair.second);
            tracerText->setPositionAlignment(Qt::AlignCenter);
            tracerText->setTextAlignment(Qt::AlignLeft);
            tracerText->setFont(data->getLabelFont());
            tracerText->setColor(data->getLabelColor());
            if(data->getLabelBackTransparent())
                tracerText->setBrush(Qt::transparent);
			else
                tracerText->setBrush(data->getLabelBackground());
		}
		else
		{
            //			m_mapScatter[dataPair].tracer->setVisible(false);
            tracerText->setVisible(false);
		}
	}
	else
	{
        graph->setVisible(false);
        //		m_mapScatter[dataPair].tracer->setVisible(false);
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

void PlotScatter::rescale_xAxis(bool on)
{
    m_customPlot->xAxis->rescale(on);
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::rescale_yAxis(bool on)
{
    m_customPlot->yAxis->rescale(on);
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::rescaleAxis(bool on)
{
    m_customPlot->rescaleAxes(on);
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

DataPair* PlotScatter::addPlotDataPair(int32_t xEntityID,
                                       const QString& xAttrName,
                                       const QString& xAttrUnitName,
                                       int32_t yEntityID,
                                       const QString& yAttrName,
                                       const QString& yAttrUnitName,
                                       const QVariantList& extraParams)
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
