#include "PlotScatter.h"

#include "PlotXYDemo.h"
#include "Utils.h"
#include "data_manager_data.h"

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

    m_isShowStaticGrid = false;
    m_staticTextColor = Qt::green;
    m_staticFont.setFamily("Microsoft YaHei");
    m_staticFont.setPixelSize(12);

    m_staticGridStyle = SOLIDLINE;
    m_staticGridDensity = GridDensity::NORMAL;
    m_tmpCoordBgnX = m_coordBgn_x;
    m_tmpCoordBgnY = m_coordBgn_y;
    m_tmpCoordEndX = m_coordEnd_x;
    m_tmpCoordEndY = m_coordEnd_y;

    initPlot();
    setupLayout();
}

PlotScatter::~PlotScatter()
{
    if(m_pixmapItem)
    {
        m_customPlot->removeItem(m_pixmapItem);
    }
}

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
            m_customPlot->replot();
        }
    });
    connect(m_customPlot->yAxis, QOverload<const QCPRange&>::of(&QCPAxis::rangeChanged), [this]() {
        updateBackgroundColorSeg();
    });
    connect(m_customPlot, &QCustomPlot::mouseMove, this, &PlotScatter::onPlotMouseMove);
    m_customPlot->axisRect()->setupFullAxesBox(true);

    m_customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->xAxis->ticker()->setTickCount(m_vertGrids);
    m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
    m_oriXAxisTicker = m_customPlot->xAxis->ticker();
    m_oriYAxisTicker = m_customPlot->yAxis->ticker();
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
    m_xScrollHash.remove(uuid);
    m_yScrollHash.remove(uuid);
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
        auto xType = data->getXDataType();
        auto yType = data->getYDataType();

        // 散点图有几种模式，时间模式只有时间属性
        if(xType == DataPair::Time && yAttr == "Now")
        {
            //当为Now的时候y轴没有数据，只需要在X轴上显示一个Now的矩形
            m_isTimeLine = true;
            m_lastDataHash.insert(uuid, QPointF(PlotXYDemo::getSeconds(), 0));
        }
        else
        {
            if(xType == DataPair::Time && yType == DataPair::Time)
            {
                x = DataManagerDataInstance->getTimeDataSet();
                y = DataManagerDataInstance->getTimeDataSet();
            }
            else if(xType == DataPair::Time && yType == DataPair::Parameter)
            {
                x = DataManagerDataInstance->getEntityAttrValueListByMaxTimeAndIniFile(
                    yEntityID, xAttr, secs, m_xRate);
                y = DataManagerDataInstance->getEntityAttrValueListByMaxTimeAndIniFile(
                    yEntityID, yAttr, secs, m_yRate);
            }
            else if(xType == DataPair::Time && yType == DataPair::RangeCalculation)
            {
                x = DataManagerDataInstance->getEntityAttrValueListByMaxTimeAndIniFile(
                    yEntityID, xAttr, secs, m_xRate);

                auto yTargetEntityID = data->getTargetEntityIDY();
                auto yCalType = data->getYCalType();
                y = DataManagerDataInstance->rangeCalculationValueList(
                    yEntityID, yTargetEntityID, yCalType, secs, m_xRate);
            }
            else if(xType == DataPair::Parameter && yType == DataPair::Time)
            {
                x = DataManagerDataInstance->getEntityAttrValueListByMaxTimeAndIniFile(
                    xEntityID, xAttr, secs, m_xRate);
                y = DataManagerDataInstance->getEntityAttrValueListByMaxTimeAndIniFile(
                    xEntityID, yAttr, secs, m_yRate);
            }
            else if(xType == DataPair::Parameter && yType == DataPair::Parameter)
            {
                x = DataManagerDataInstance->getEntityAttrValueListByMaxTimeAndIniFile(
                    xEntityID, xAttr, secs, m_xRate);
                y = DataManagerDataInstance->getEntityAttrValueListByMaxTimeAndIniFile(
                    yEntityID, yAttr, secs, m_yRate);
            }
            else if(xType == DataPair::Parameter && yType == DataPair::RangeCalculation)
            {
                x = DataManagerDataInstance->getEntityAttrValueListByMaxTimeAndIniFile(
                    xEntityID, xAttr, secs, m_xRate);

                auto yTargetEntityID = data->getTargetEntityIDY();
                auto yCalType = data->getYCalType();
                y = DataManagerDataInstance->rangeCalculationValueList(
                    yEntityID, yTargetEntityID, yCalType, secs, m_xRate);
            }
            else if(xType == DataPair::RangeCalculation && yType == DataPair::Time)
            {
                auto xTargetEntityID = data->getTargetEntityIDX();
                auto xCalType = data->getXCalType();
                x = DataManagerDataInstance->rangeCalculationValueList(
                    xEntityID, xTargetEntityID, xCalType, secs, m_xRate);

                y = DataManagerDataInstance->getEntityAttrValueListByMaxTimeAndIniFile(
                    xEntityID, xAttr, secs, m_xRate);
            }
            else if(xType == DataPair::RangeCalculation && yType == DataPair::Parameter)
            {
                auto xTargetEntityID = data->getTargetEntityIDX();
                auto xCalType = data->getXCalType();
                x = DataManagerDataInstance->rangeCalculationValueList(
                    xEntityID, xTargetEntityID, xCalType, secs, m_xRate);

                y = DataManagerDataInstance->getEntityAttrValueListByMaxTimeAndIniFile(
                    yEntityID, xAttr, secs, m_xRate);
            }
            else if(xType == DataPair::RangeCalculation && yType == DataPair::RangeCalculation)
            {
                auto xTargetEntityID = data->getTargetEntityIDX();
                auto xCalType = data->getXCalType();
                x = DataManagerDataInstance->rangeCalculationValueList(
                    xEntityID, xTargetEntityID, xCalType, secs, m_xRate);

                auto yTargetEntityID = data->getTargetEntityIDY();
                auto yCalType = data->getYCalType();
                y = DataManagerDataInstance->rangeCalculationValueList(
                    yEntityID, yTargetEntityID, yCalType, secs, m_xRate);
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
    updateScrollAxis();
    m_customPlot->replot(QCustomPlot::rpQueuedReplot);
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
        m_customPlot->replot();
        return;
    }
    auto uuid = data->getUuid();
    auto x = m_dataHash.value(uuid).first;
    auto y = m_dataHash.value(uuid).second;
    if(x.isEmpty() || y.isEmpty())
    {
        //无效数据自动隐藏Label和Icon
        if(m_mapScatter.contains(uuid))
        {
            m_mapScatter[uuid].graph->setVisible(false);
            m_mapScatter[uuid].tracerText->setVisible(false);
            m_mapScatter[uuid].pixmap->setVisible(false);
        }
        return;
    }
    if(!m_mapScatter.contains(uuid))
	{
        DrawComponents info;
        info.graph = m_customPlot->addGraph();
        /*
         * 默认采样值是true，在某些情况下采样会导致把原始数据处理错误，导致连线时路径错误
         * 但是在线模式会一直无限追加数据，需要开启采样，不然数据量太大会严重卡顿
         * 离线模式不启用采样
         */
        info.graph->setAdaptiveSampling(true);
        info.graph->setBrush(Qt::NoBrush);
        info.tracerText = new QCPItemText(m_customPlot);
        info.tracerText->position->setType(QCPItemPosition::ptPlotCoords);
        // 默认position的坐标是center,为了方便计算，将position的坐标改为左上角
        info.tracerText->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
        info.tracerText->setTextAlignment(Qt::AlignCenter);

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
        double secsLimit = data->getSecondsLimit();
        if(!math::doubleEqual(secsLimit, 0.0) && (curSecs > secsLimit))
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

        // seconds  ,ordinal  ,month day
        if(m_xTimeTickFormat == "ordinal")
        {
            int refYear = DataManagerDataInstance->getRefYear();
            QDateTime refTime = QDateTime::fromString(QString("%1-01-01 00:00:00").arg(refYear),
                                                      "yyyy-MM-dd hh:mm:ss");
            qint64 offsetSeconds = refTime.toSecsSinceEpoch();
            for(int i = 0; i < x.size(); i++)
            {
                x.replace(i, x.at(i) + offsetSeconds);
            }
        }

        if(m_yTimeTickFormat == "ordinal")
        {
            int refYear = DataManagerDataInstance->getRefYear();
            QDateTime refTime = QDateTime::fromString(QString("%1-01-01 00:00:00").arg(refYear),
                                                      "yyyy-MM-dd hh:mm:ss");
            qint64 offsetSeconds = refTime.toSecsSinceEpoch();
            for(int i = 0; i < y.size(); i++)
            {
                y.replace(i, y.at(i) + offsetSeconds);
            }
        }
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
                double topXCoord = m_customPlot->xAxis->pixelToCoord(topX);
                double topYCoord = m_customPlot->yAxis->pixelToCoord(topY);
                pixmap->topLeft->setCoords(topXCoord, topYCoord);
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
            QPointF labelPoint = data->processLabelPosition(
                QPointF(lastPosX, lastPosY), text, tracerText->textAlignment());
            // 根据像素大小反过来计算坐标
            double labelXCoord = m_customPlot->xAxis->pixelToCoord(labelPoint.x());
            double labelYCoord = m_customPlot->yAxis->pixelToCoord(labelPoint.y());
            tracerText->position->setCoords(labelXCoord, labelYCoord);
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

void PlotScatter::resizeEvent(QResizeEvent* event)
{
    setStaticGrid();
    QWidget::resizeEvent(event);
}

void PlotScatter::onPlotMouseMove(QMouseEvent* event)
{
    if(m_isTimeLine)
    {
        double xCoord = m_customPlot->xAxis->pixelToCoord(event->x());
        double yCoord = m_customPlot->yAxis->pixelToCoord(event->y());
        m_customPlot->setToolTip(QString("X:%1\nY:%2").arg(xCoord).arg(yCoord));
    }
}

void PlotScatter::setStaticGrid(bool isResetRange)
{
    if(!m_pixmapItem)
        m_pixmapItem = new QCPItemPixmap(m_customPlot);
    if(m_isShowStaticGrid)
    {
        int gridNum = 0;
        switch(m_staticGridDensity)
        {
        case GridDensity::LESS:
            gridNum = 5;
            break;
        case GridDensity::NORMAL:
            gridNum = 7;
            break;
        case GridDensity::MORE:
            gridNum = 9;
            break;
        }

        if(isResetRange)
        {
            m_tmpCoordBgnX = m_coordBgn_x;
            m_tmpCoordBgnY = m_coordBgn_y;
            m_tmpCoordEndX = m_coordEnd_x;
            m_tmpCoordEndY = m_coordEnd_y;
        }

        int width = m_customPlot->axisRect()->width();
        int height = m_customPlot->axisRect()->height();
        QPixmap pixmap(width, height);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(QPen(Qt::white, 1, Qt::PenStyle(m_staticGridStyle + 1)));
        for(int gridIndex = 1; gridIndex < gridNum - 1; gridIndex++)
        {
            painter.drawLine(
                width / (gridNum - 1) * gridIndex, 0, width / (gridNum - 1) * gridIndex, height);
            painter.drawLine(
                0, height / (gridNum - 1) * gridIndex, width, height / (gridNum - 1) * gridIndex);
        }
        painter.setPen(QPen(m_staticTextColor));
        painter.setFont(m_staticFont);
        for(int gridIndex = 1; gridIndex < gridNum - 1; gridIndex++)
        {
            QString xText = QString::number(m_tmpCoordBgnX + (m_tmpCoordEndX - m_tmpCoordBgnX) /
                                                                 (gridNum - 1) * gridIndex);
            QString yText = QString::number(m_tmpCoordEndY - (m_tmpCoordEndY - m_tmpCoordBgnY) /
                                                                 (gridNum - 1) * gridIndex);
            painter.drawText(width / (gridNum - 1) * gridIndex, int(height * 0.05), xText);
            painter.drawText(width / (gridNum - 1) * gridIndex, int(height * 0.95), xText);
            painter.drawText(int(width * 0.05), height / (gridNum - 1) * gridIndex, yText);
            painter.drawText(int(width * 0.95), height / (gridNum - 1) * gridIndex, yText);
        }
        m_pixmapItem->topLeft->setType(QCPItemPosition::ptAxisRectRatio);
        m_pixmapItem->topLeft->setCoords(0, 0);
        m_pixmapItem->bottomRight->setType(QCPItemPosition::ptAxisRectRatio);
        m_pixmapItem->bottomRight->setCoords(1, 1);
        m_pixmapItem->setPixmap(pixmap);
        m_pixmapItem->setLayer("grid");
        m_pixmapItem->setVisible(true);
    }
    else
        m_pixmapItem->setVisible(false);

    m_customPlot->replot();
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
                                         int32_t lineWidth,
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
            xlimit =
                DataManagerDataInstance->getMaxAndMinEntityAttrValue(yEntityID, xAttrName, m_xRate);
        }
        else
        {
            xlimit =
                DataManagerDataInstance->getMaxAndMinEntityAttrValue(xEntityID, xAttrName, m_xRate);
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
            DataManagerDataInstance->getMaxAndMinEntityAttrValue(yEntityID, yAttrName, m_yRate);
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
    auto dataPair = PlotItemBase::addPlotDataPair(xEntityID,
                                                  xAttrName,
                                                  xAttrUnitName,
                                                  yEntityID,
                                                  yAttrName,
                                                  yAttrUnitName,
                                                  extraParams,
                                                  isFromJson);
    //scroll
    if(!isFromJson)
    {
        m_xScrollHash.insert(dataPair->getUuid(), false);
        m_yScrollHash.insert(dataPair->getUuid(), false);
    }
    return dataPair;
}

void PlotScatter::clearEventText()
{
    for(const auto& draw : m_eventDrawList)
    {
        m_customPlot->removeItem(draw.line);
        m_customPlot->removeItem(draw.text);
    }
    m_eventDrawList.clear();
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
    if(!m_timelineGraph)
    {
        m_timelineGraph = m_customPlot->addGraph();
        m_timelineGraph->setVisible(true);
        QVector<double> timeVec = DataManagerDataInstance->getTimeDataSet();
        if(!timeVec.isEmpty())
        {
            setCoordRangeX(timeVec.first(), timeVec.last());
        }
        else
        {
            setCoordRangeX(0, 10000);
        }

        m_customPlot->xAxis->setLabel("Time(s)");
        m_customPlot->yAxis->setLabel("All Platforms");
    }
    m_customPlot->yAxis->grid()->setVisible(false);
    setCoordRangeY(0.0, 1.0);
    m_customPlot->yAxis->setTickLabels(true);
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
    // 动态给左边添加显示平台名称的ticker
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);

    // 上下预留0.1，中间区域占比0.8
    double heightDelta = 0.8 / eventList.size();
    int32_t index = 0;
    QHash<int32_t, double> entityHeightHash;
    for(const auto& event : eventList)
    {
        // 当前平台处于图表中的高度比率
        double height = 0.0;
        // 相同平台的事件在同一个平台高度
        if(entityHeightHash.contains(event.m_entityID))
        {
            height = entityHeightHash.value(event.m_entityID);
        }
        else
        {
            height = 0.1 + index * heightDelta;
            entityHeightHash.insert(event.m_entityID, height);
            // 根据text的高度比率计算ticker的坐标值,因为y轴范围0-1，所以省去乘以1的操作
            double tickerCoord = (1 - height) * 1;
            textTicker->addTick(tickerCoord, event.m_entityName);
        }
        auto dataList =
            DataManagerDataInstance->getGenericDataListByID(event.m_entityID, event.m_type, 10);
        //如果x轴是time，那么需要绘制事件标签，整个用一个Text显示
        for(const auto& data : dataList)
        {
            QCPItemText* textItem = new QCPItemText(m_customPlot);

            textItem->setPositionAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            // x轴需要设置到对应的时间坐标上，y轴需要按照像素坐标从低到高排列
            textItem->position->setTypeX(QCPItemPosition::ptPlotCoords);
            textItem->position->setTypeY(QCPItemPosition::ptAxisRectRatio);

            QFont font;
            font.setFamily(event.m_eventFontFamily);
            font.setPixelSize(event.m_eventFontSize);
            textItem->setFont(font);
            textItem->setColor(event.m_eventColor);

            QCPItemLine* lineItem = new QCPItemLine(m_customPlot);
            QString text;
            if(event.m_eventStyle == "Small X")
            {
                text = "× ";
                lineItem->setVisible(false);
                textItem->position->setCoords(data.m_relativeTime, height);
            }
            else
            {
                lineItem->setVisible(true);
                QPen pen(event.m_eventColor, 2);
                lineItem->setPen(pen);
                lineItem->start->setTypeX(QCPItemPosition::ptPlotCoords);
                lineItem->start->setTypeY(QCPItemPosition::ptAxisRectRatio);
                lineItem->start->setCoords(data.m_relativeTime, 0.0);

                lineItem->end->setTypeX(QCPItemPosition::ptPlotCoords);
                lineItem->end->setTypeY(QCPItemPosition::ptAxisRectRatio);
                lineItem->end->setCoords(data.m_relativeTime, 1.0);

                double textXPixel = m_customPlot->xAxis->coordToPixel(data.m_relativeTime) + 10;
                double textXCoord = m_customPlot->xAxis->pixelToCoord(textXPixel);
                textItem->position->setCoords(textXCoord, height);
            }
            if(event.m_isIncludeTag)
            {
                text.append(data.m_platName + data.m_eventName);
            }
            text.append(QString("(%1s)").arg(data.m_relativeTime));
            textItem->setText(text);

            EventComponents eventDraw;
            eventDraw.line = lineItem;
            eventDraw.text = textItem;

            m_eventDrawList.append(eventDraw);
        }
        ++index;
    }
    m_customPlot->yAxis->setTicker(textTicker);
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
    for(const auto& draw : m_plotMarkerItems)
    {
        m_customPlot->removeItem(draw.line);
        m_customPlot->removeItem(draw.text);
    }
    m_plotMarkerItems.clear();
    if(m_plotMarkers.isEmpty())
    {
        return;
    }
    for(const auto& marker : m_plotMarkers)
    {
        if(marker.time > currentSeconds)
        {
            continue;
        }
        QCPItemText* textItem = new QCPItemText(m_customPlot);
        QCPItemLine* lineItem = new QCPItemLine(m_customPlot);
        if(marker.iconType == "无")
        {
            textItem->setText(marker.text);
            lineItem->setVisible(false);
        }
        else if(marker.iconType == "O 圆圈")
        {
            textItem->setText("O " + marker.text);
            lineItem->setVisible(false);
        }
        else if(marker.iconType == "X 叉")
        {
            textItem->setText("× " + marker.text);
            lineItem->setVisible(false);
        }
        else if(marker.iconType == "+ 加号")
        {
            textItem->setText("+ " + marker.text);
            lineItem->setVisible(false);
        }
        else if(marker.iconType == "[] 方框")
        {
            textItem->setText("[] " + marker.text);
            lineItem->setVisible(false);
        }
        else if(marker.iconType == "<> 菱形")
        {
            textItem->setText("<> " + marker.text);
            lineItem->setVisible(false);
        }
        else if(marker.iconType == "- 水平线")
        {
            textItem->setText("- " + marker.text);
            lineItem->setVisible(true);
        }
        else
        {
            // 垂直线
            textItem->setText(marker.text);
            lineItem->setVisible(true);
        }
        QFont font;
        font.setFamily(marker.fontFamily);
        font.setPixelSize(marker.fontSize);
        textItem->setFont(font);
        textItem->setColor(marker.color);
        if(lineItem->visible())
        {
            QPen pen(marker.color, 2);
            lineItem->setPen(pen);
            if(marker.iconType == "- 水平线")
            {
                lineItem->start->setTypeX(QCPItemPosition::ptAxisRectRatio);
                lineItem->start->setTypeY(QCPItemPosition::ptPlotCoords);
                lineItem->start->setCoords(0.0, marker.y);

                lineItem->end->setTypeX(QCPItemPosition::ptAxisRectRatio);
                lineItem->end->setTypeY(QCPItemPosition::ptPlotCoords);
                lineItem->end->setCoords(1.0, marker.y);
            }
            else
            {
                lineItem->start->setTypeX(QCPItemPosition::ptPlotCoords);
                lineItem->start->setTypeY(QCPItemPosition::ptAxisRectRatio);
                lineItem->start->setCoords(marker.x, 0.0);

                lineItem->end->setTypeX(QCPItemPosition::ptPlotCoords);
                lineItem->end->setTypeY(QCPItemPosition::ptAxisRectRatio);
                lineItem->end->setCoords(marker.x, 1.0);
            }
            textItem->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
            textItem->position->setType(QCPItemPosition::ptPlotCoords);
            double textXPixel = m_customPlot->xAxis->coordToPixel(marker.x) + 10;
            double textXCoord = m_customPlot->xAxis->pixelToCoord(textXPixel);
            textItem->position->setCoords(textXCoord, marker.y);
        }
        else
        {
            textItem->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
            textItem->position->setType(QCPItemPosition::ptPlotCoords);
            textItem->position->setCoords(marker.x, marker.y);
        }

        EventComponents markDraw;
        markDraw.line = lineItem;
        markDraw.text = textItem;

        m_plotMarkerItems.insert(marker.uuid, markDraw);
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
}

void PlotScatter::updateScrollAxis()
{
    bool xNeedScroll = false;
    bool yNeedScroll = false;
    QPointF currentPoint = getCurrentAverageXYValue(xNeedScroll, yNeedScroll);

    if(m_xScrollOn && xNeedScroll)
    {
        setCoordRangeX(currentPoint.x() - m_xFollow, currentPoint.x() + m_xLead);
    }
    if(m_yScrollOn && yNeedScroll)
    {
        setCoordRangeY(currentPoint.y() - m_yFollow, currentPoint.y() + m_yLead);
    }
}

QPointF PlotScatter::getCurrentAverageXYValue(bool& xNeedScroll, bool& yNeedScroll)
{
    // 存在
    QStringList uuidList = m_lastDataHash.keys();
    double sumX = 0.0;
    double sumY = 0.0;
    int32_t xEnableSize = 0;
    int32_t yEnableSize = 0;
    for(const QString& uuid : uuidList)
    {
        if(m_xScrollHash.value(uuid, false))
        {
            QPointF currentPoint = m_lastDataHash.value(uuid);
            sumX += currentPoint.x();
            ++xEnableSize;
        }
        if(m_yScrollHash.value(uuid, false))
        {
            QPointF currentPoint = m_lastDataHash.value(uuid);
            sumY += currentPoint.y();
            ++yEnableSize;
        }
    }
    QPointF point;
    if(xEnableSize > 0)
    {
        double averageX = sumX / xEnableSize;
        point.setX(averageX);
        xNeedScroll = true;
    }
    else
    {
        xNeedScroll = false;
    }
    if(yEnableSize > 0)
    {
        double averageY = sumY / yEnableSize;
        point.setY(averageY);
        yNeedScroll = true;
    }
    else
    {
        yNeedScroll = false;
    }
    return point;
}

GridDensity PlotScatter::getStaticGridDensity() const
{
    return m_staticGridDensity;
}

void PlotScatter::setStaticGridDensity(const GridDensity& staticGridDensity)
{
    m_staticGridDensity = staticGridDensity;
    setStaticGrid();
}

GridStyle PlotScatter::getStaticGridStyle() const
{
    return m_staticGridStyle;
}

void PlotScatter::setStaticGridStyle(const GridStyle& staticGridStyle)
{
    m_staticGridStyle = staticGridStyle;
    setStaticGrid();
}

QFont PlotScatter::getStaticFont() const
{
    return m_staticFont;
}

void PlotScatter::setStaticFont(const QFont& staticFont)
{
    m_staticFont = staticFont;
    setStaticGrid();
}

QColor PlotScatter::getStaticTextColor() const
{
    return m_staticTextColor;
}

void PlotScatter::setStaticTextColor(const QColor& staticTextColor)
{
    m_staticTextColor = staticTextColor;
    setStaticGrid();
}

bool PlotScatter::getIsShowStaticGrid() const
{
    return m_isShowStaticGrid;
}

void PlotScatter::setIsShowStaticGrid(bool isShowStaticGrid)
{
    m_isShowStaticGrid = isShowStaticGrid;
    setStaticGrid(true);
}

QString PlotScatter::getYTimeTickFormat() const
{
    return m_yTimeTickFormat;
}

QString PlotScatter::getXTimeTickFormat() const
{
    return m_xTimeTickFormat;
}

void PlotScatter::setBkgLimitSegMap(const QMap<double, BackgroundLimitSeg>& bkgLimitSegMap)
{
    m_bkgLimitSegMap = bkgLimitSegMap;
}

void PlotScatter::setTimeTickerFormat(const QString& format, bool isXAxis)
{

    if(isXAxis)
    {
        m_xTimeTickFormat = format;
        auto xAxis = m_customPlot->xAxis;
        if(format == "seconds")
        {
            xAxis->setTicker(m_oriXAxisTicker);
        }
        else if(format == "ordinal")
        {
            QSharedPointer<QCPAxisTickerDateTime> xDateTimeTicker(new QCPAxisTickerDateTime);
            xDateTimeTicker->setDateTimeFormat("yyyy/MM/dd hh:mm:ss.zzz");
            xDateTimeTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
            xAxis->setTicker(xDateTimeTicker);
        }
        else if(format == "month day")
        {
            //            QSharedPointer<QCPAxisTickerDateTime> monthDayTicker(new QCPAxisTickerDateTime);

            //            int refYear = DataManagerDataInstance->getRefYear();
            //            // show data time
            //            QString dataTime = OrdinalTimeFormatter::toString(m_seconds, refYear);

            //            monthDayTicker->setDateTimeFormat("yyyy/MM/dd hh:mm:ss.zzz");
            //            monthDayTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
            //            xAxis->setTicker(monthDayTicker);
        }
    }
    else
    {
        m_yTimeTickFormat = format;
        auto yAxis = m_customPlot->yAxis;
        if(format == "seconds")
        {
            yAxis->setTicker(m_oriYAxisTicker);
        }
        else if(format == "ordinal")
        {
            QSharedPointer<QCPAxisTickerDateTime> yDateTimeTicker(new QCPAxisTickerDateTime);
            yDateTimeTicker->setDateTimeFormat("yyyy/MM/dd hh:mm:ss.zzz");
            yDateTimeTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
            yAxis->setTicker(yDateTimeTicker);
        }
        else if(format == "month day")
        {}
    }

    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}

QHash<QString, bool> PlotScatter::getYScrollHash() const
{
    return m_yScrollHash;
}

void PlotScatter::setYScrollHash(const QHash<QString, bool>& yScrollHash)
{
    m_yScrollHash = yScrollHash;
}

void PlotScatter::setXScrollEnableByUUID(const QString& uuid, bool enable)
{
    if(m_xScrollHash.contains(uuid))
    {
        m_xScrollHash.insert(uuid, enable);
        updateScrollAxis();
    }
}

bool PlotScatter::getXScrollEnableByUUID(const QString& uuid)
{
    return m_xScrollHash.value(uuid, false);
}

void PlotScatter::setYScrollEnableByUUID(const QString& uuid, bool enable)
{
    if(m_yScrollHash.contains(uuid))
    {
        m_yScrollHash.insert(uuid, enable);
        updateScrollAxis();
    }
}

bool PlotScatter::getYScrollEnableByUUID(const QString& uuid)
{
    return m_yScrollHash.value(uuid, false);
}

double PlotScatter::getYFollow() const
{
    return m_yFollow;
}

void PlotScatter::setYFollow(double yFollow)
{
    m_yFollow = yFollow;
    updateScrollAxis();
}

double PlotScatter::getYLead() const
{
    return m_yLead;
}

void PlotScatter::setYLead(double yLead)
{
    m_yLead = yLead;
    updateScrollAxis();
}

bool PlotScatter::getYScrollOn() const
{
    return m_yScrollOn;
}

void PlotScatter::setYScrollOn(bool yScrollOn)
{
    m_yScrollOn = yScrollOn;
    updateScrollAxis();
}

QHash<QString, bool> PlotScatter::getXScrollHash() const
{
    return m_xScrollHash;
}

void PlotScatter::setXScrollHash(const QHash<QString, bool>& xScrollHash)
{
    m_xScrollHash = xScrollHash;
}

double PlotScatter::getXFollow() const
{
    return m_xFollow;
}

void PlotScatter::setXFollow(double xFollow)
{
    m_xFollow = xFollow;
    updateScrollAxis();
}

double PlotScatter::getXLead() const
{
    return m_xLead;
}

void PlotScatter::setXLead(double xLead)
{
    m_xLead = xLead;
    updateScrollAxis();
}

bool PlotScatter::getXScrollOn() const
{
    return m_xScrollOn;
}

void PlotScatter::setXScrollOn(bool xScrollOn)
{
    m_xScrollOn = xScrollOn;
    updateScrollAxis();
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
        m_customPlot->replot();
    }
}

void PlotScatter::removeConnection(const QString& uuid)
{
    if(m_conHash.contains(uuid))
    {
        m_conHash.remove(uuid);
        updateConnectionLines();
        m_customPlot->replot();
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
        m_customPlot->replot();
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
        m_customPlot->replot();
    }
}

void PlotScatter::removeMarker(const QString& uuid)
{
    if(m_plotMarkers.contains(uuid))
    {
        m_plotMarkers.remove(uuid);
        updateMarkers(PlotXYDemo::getSeconds());
        m_customPlot->replot();
    }
}

void PlotScatter::modifyMarker(const QString& uuid, const PlotMarker& marker)
{
    if(m_plotMarkers.contains(marker.uuid))
    {
        m_plotMarkers.insert(uuid, marker);
        updateMarkers(PlotXYDemo::getSeconds());
        m_customPlot->replot();
    }
}
