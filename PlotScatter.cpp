#include "PlotScatter.h"
#include "DataManager.h"

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

    m_outerFillColor = Qt::black;
    m_gridFillColor = Qt::black;
    m_title = "Scatter Plot";

    m_axisLabelColor = Qt::white;
    m_axisLabelFont.setFamily("Microsoft YaHei");
    m_axisLabelFont.setPointSizeF(10.0);
    m_xAxisLabel = "X Axis";
    m_yAxisLabel = "Y Axis";

    //  m_leftPadding = 10;
    //  m_rightPadding = 10;
    //  m_topPadding = 10;
    //  m_bottomPadding = 10;

    m_coordBgn_x = 0;
    m_coordEnd_x = 2000;
    m_coordBgn_y = 0;
    m_coordEnd_y = 2000;

    m_horzGrids = 5;
    m_vertGrids = 5;
    m_axisWidth = 1;
    m_gridWidth = 1;
    m_axisColor = Qt::white;
    m_gridColor = QColor(200, 200, 200);
    m_gridVisible = true;
    m_tickLabelColor = Qt::white;
    m_tickLabelFontSize = 8;
    m_tickLabelFont.setFamily("Microsoft YaHei");
    m_tickLabelFont.setPointSizeF(m_tickLabelFontSize);
    m_gridStyle = Qt::DotLine;
    m_gridDensity = GridDensity::LESS;

    m_showUnits_x = false;
    m_showUnits_y = false;

    initPlot();
}

PlotScatter::~PlotScatter() {}

void PlotScatter::initPlot()
{
    m_customPlot = new QCustomPlot();
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_customPlot);
    setLayout(pLayout);
    m_customPlot->axisRect()->setupFullAxesBox(true);
    m_customPlot->axisRect()->setMinimumMargins(QMargins(30, 15, 30, 15));

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
    m_customPlot->xAxis->setRange(m_coordBgn_x, m_coordEnd_x);
    m_customPlot->yAxis->setRange(m_coordBgn_y, m_coordEnd_y);

    //      m_customPlot->xAxis->setNumberPrecision(3);
    //      m_customPlot->yAxis->setNumberPrecision(3);

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
}

void PlotScatter::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
        return;

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
    // 先默认缩放坐标轴看效果
    m_customPlot->rescaleAxes(true);
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::updateGraphByDataPair(DataPair* data)
{
    if(!data)
    {
        return;
    }
    ScatterInfo info;
    auto uuid = data->getUuid();
    if(!m_mapScatter.contains(uuid))
	{
        info.graph = m_customPlot->addGraph();
        info.tracer = new QCPItemTracer(m_customPlot);
        info.tracerText = new QCPItemText(m_customPlot);
        info.tracer->setGraph(info.graph);
        info.tracer->setInterpolating(false);
        info.tracer->setStyle(QCPItemTracer::tsNone);
        info.tracerText->position->setType(QCPItemPosition::ptPlotCoords);
        info.tracerText->position->setParentAnchor(info.tracer->position);
        m_mapScatter.insert(uuid, info);
    }
    auto graph = m_mapScatter[uuid].graph;
    auto tracerText = m_mapScatter[uuid].tracerText;
    if(data->isDraw())
    {
        auto x = m_dataHash.value(uuid).first;
        auto y = m_dataHash.value(uuid).second;
        if(x.isEmpty() || y.isEmpty())
            return;

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
            //test
            QPixmap pix(data->iconName());
            pix = pix.scaled(data->iconSize(), Qt::IgnoreAspectRatio);
            QTransform trans;
            auto rotationIndex = data->iconRotation();
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
            default:
                trans.rotate(45);
                break;
            }
            pix = pix.transformed(trans);
            //水平镜像
            if(data->iconFlipHorz() == true)
            {
                QImage oldImage = pix.toImage();
                QImage newImage = oldImage.mirrored(true, false);
                pix = QPixmap::fromImage(newImage);
            }
            //垂直镜像
            if(data->iconFlipVert() == true)

			{
				QImage oldImage = pix.toImage();
				QImage newImage = oldImage.mirrored(false, true);
				pix = QPixmap::fromImage(newImage);
			}
			QCPScatterStyle style(pix);
            graph->setScatterStyle(style);
		}
        graph->setData(x, y);

        //Label Text
        if(data->isLabelTextShow())
        {

            // 游标功能先注释掉

            //			m_mapScatter[dataPair].tracer->setVisible(true);
            tracerText->setVisible(true);
			//设置锚点
            //			m_mapScatter[dataPair].tracer->setGraphKey(x.last());
            QString labelText;
            if(DataPair::format_default == data->getTextFormat())
            { //default
                QString prefix_x, prefix_y;
                QString object_x, object_y, attr_x, attr_y;
                QString data_x, data_y, unit_x, unit_y, Left_bracket, right_bracket;

                //考虑仅显示实体名时的操作
                if(data->isObjectShow() && !data->isPrefixShow() && !data->isAttrShow() &&
                   !data->isDataShow() && !data->isUnitShow())

				{
                    object_x = data->getEntity_x();
                    object_y = data->getEntity_y();
					//实体名相同时，仅显示一个实体名
                    if(0 == object_x.compare(object_y) && !object_x.isEmpty() &&
                       !object_y.isEmpty())
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
                    if(data->isPrefixShow())
                    {
                        prefix_x = "X:";
                        prefix_y = "Y:";
                    }

                    if(data->isObjectShow())

					{
                        object_x = data->getEntity_x();
                        object_y = data->getEntity_y();
					}

                    if(data->isAttrShow())
					{
                        attr_x = data->getAttr_x();
                        attr_y = data->getAttr_y();
					}

                    if(data->isDataShow())
                    {
                        data_x = QString::number(x.last(), 10, data->getLabelPrecision_x());
                        data_y = QString::number(y.last(), 10, data->getLabelPrecision_y());
                        Left_bracket = "(";
                        right_bracket = ")";
                    }

                    if(data->isUnitShow())
                    {
                        unit_x = data->getUnit_x();
                        unit_y = data->getUnit_y();
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
            else if(DataPair::format_custom == data->getTextFormat())
            { //custom
                labelText = data->getCustomText();
            }
            else if(DataPair::format_script == data->getTextFormat()) //script
            {}

            tracerText->setText(labelText);

            QFontMetricsF fm(data->getLabelFont());
            double wd = (fm.size(Qt::TextSingleLine, data->getLabelText()).width()) / 3.0;
            double ht = fm.size(Qt::TextSingleLine, data->getLabelText()).height() / 1.0;
            switch(data->getLabelPosition())
            {
            case DataPair::left_top: //left-top
                tracerText->position->setCoords(-wd, -ht);
				break;
            case DataPair::top: //top
                tracerText->position->setCoords(0, -ht);
				break;
            case DataPair::right_top: //right-top
                tracerText->position->setCoords(wd, -ht);
				break;
            case DataPair::left: //left
                tracerText->position->setCoords(-wd, 0);
				break;
            case DataPair::center: //center
                tracerText->position->setCoords(0, 0);
				break;
            case DataPair::right: //right
                tracerText->position->setCoords(wd, 0);
				break;
            case DataPair::left_bottom: //left-bottom
                tracerText->position->setCoords(-wd, ht);
				break;
            case DataPair::bottom: //bottom
                tracerText->position->setCoords(0, ht);
				break;
            case DataPair::right_bottom: //right-bottom
                tracerText->position->setCoords(wd, ht);
				break;
            default: //right
                tracerText->position->setCoords(wd, 0);
				break;
			}
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
