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
	m_titleColor = Qt::white;
	m_titleFillColor = Qt::black;
	m_titleFontSize = 16;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(m_titleFontSize);
	m_titleVisible = true;

	m_axisLabelColor = Qt::white;
	m_axisLabelFont.setFamily("Microsoft YaHei");
	m_axisLabelFont.setPointSizeF(10.0);
	m_xAxisLabel = "X Axis";
	m_yAxisLabel = "Y Axis";

	m_leftPadding = 10;
	m_rightPadding = 10;
	m_topPadding = 10;
	m_bottomPadding = 10;

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
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);
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

	//  	m_customPlot->xAxis->setNumberPrecision(3);
	//  	m_customPlot->yAxis->setNumberPrecision(3);

	m_customPlot->setBackground(m_outerFillColor);
	m_customPlot->axisRect()->setBackground(m_gridFillColor);
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->yAxis->setLabelFont(m_axisLabelFont);

	m_customPlot->replot();
}

void PlotScatter::addPlotPairData(const QPair<QString, QString>& pair)
{
    PlotItemBase::addPlotPairData(pair);

	//scatter
	ScatterInfo info;
	info.graph = m_customPlot->addGraph();
	info.tracer = new QCPItemTracer(m_customPlot);
	info.tracerText = new QCPItemText(m_customPlot);
	info.tracer->setGraph(info.graph);
	info.tracer->setInterpolating(true);
	info.tracer->setStyle(QCPItemTracer::tsNone);
	info.tracerText->position->setType(QCPItemPosition::ptPlotCoords);
	info.tracerText->position->setParentAnchor(info.tracer->position);
	m_mapScatter.insertMulti(pair, info);
}

void PlotScatter::delPlotPairData(const QPair<QString, QString>& pair)
{
    if(m_dataPairs.isEmpty())
		return;

    for(int i = 0; i < m_dataPairs.size(); ++i)
	{
        if(m_dataPairs.at(i)->getDataPair() == pair)
		{
            m_dataPairs.remove(i);

            emit sgn_dataPairChanged(this);
			break;
		}
	}

	//scatter
    if(m_mapScatter.contains(pair))
	{
		m_mapScatter.remove(pair);
	}
}

void PlotScatter::updatePlotPairData(const QPair<QString, QString>& oldPair,
                                     const QPair<QString, QString>& newPair)
{
    if(m_dataPairs.isEmpty())
		return;

    for(int i = 0; i < m_dataPairs.size(); ++i)
	{
        if(m_dataPairs.at(i)->getDataPair() == oldPair)
		{
            m_dataPairs.at(i)->setDataPair(newPair);

            emit sgn_dataPairChanged(this);
			break;
		}
	}

	//scatter
    if(m_mapScatter.contains(oldPair))
	{
		m_mapScatter.remove(oldPair);
		ScatterInfo info;
		info.graph = m_customPlot->addGraph();
		info.tracer = new QCPItemTracer(m_customPlot);
		info.tracerText = new QCPItemText(m_customPlot);
		info.tracer->setGraph(info.graph);
		info.tracer->setInterpolating(true);
		info.tracer->setStyle(QCPItemTracer::tsNone);
		info.tracerText->position->setType(QCPItemPosition::ptPlotCoords);
		info.tracerText->position->setParentAnchor(info.tracer->position);
		m_mapScatter.insertMulti(newPair, info);
	}
}

void PlotScatter::getDataInfo(double secs)
{
    if(getDataPairs().isEmpty())
		return;

    int itemCnt = m_dataPairs.size();

    for(int i = 0; i < itemCnt; ++i)
	{
        updateData(secs, i, m_dataPairs.at(i));
	}
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
	//	update();
}

void PlotScatter::updateData(double secs, int index, DataPair* data)
{
	QPair<QString, QString> dataPair = data->getDataPair();
    if(data->isDraw())
	{
		QVector<double> x, y;
		QString xEntityType = dataPair.first;
		QString yEntityType = dataPair.second;
		QStringList xlist = xEntityType.split("+");
		QStringList ylist = yEntityType.split("+");
        if(xlist.size() == 1 && ylist.size() == 1)
		{
			x = DataManager::getInstance()->getTimeData_vector();
			y = DataManager::getInstance()->getTimeData_vector();
		}
        else if(xlist.size() == 1 && ylist.size() == 2)
		{
            x = DataManager::getInstance()
                    ->getEntityAttr_MaxPartValue_List(ylist.at(0), xlist.at(0), secs)
                    .toVector();
            y = DataManager::getInstance()
                    ->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs)
                    .toVector();
		}
        else if(xlist.size() == 2 && ylist.size() == 1)
		{
            x = DataManager::getInstance()
                    ->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs)
                    .toVector();
            y = DataManager::getInstance()
                    ->getEntityAttr_MaxPartValue_List(xlist.at(0), ylist.at(0), secs)
                    .toVector();
		}
		else
		{
            x = DataManager::getInstance()
                    ->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs)
                    .toVector();
            y = DataManager::getInstance()
                    ->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs)
                    .toVector();
		}

        if(x.isEmpty() || y.isEmpty())
			return;

		m_mapScatter[dataPair].graph->setVisible(true);
		m_mapScatter[dataPair].graph->setPen(QPen(data->dataColor(), data->lineWidth()));
		//line mode
        if(data->isLineMode())
		{
			m_mapScatter[dataPair].graph->setLineStyle(QCPGraph::lsLine);
			m_mapScatter[dataPair].graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
		}
		else
		{
			m_mapScatter[dataPair].graph->setLineStyle(QCPGraph::lsNone);
            m_mapScatter[dataPair].graph->setScatterStyle(
                QCPScatterStyle(QCPScatterStyle::ssDisc, data->lineWidth()));
		}
		//icon
        if(data->isIconDraw())
		{
			//test
			QPixmap pix(data->iconName());
			//QPixmap pix(":/statusbar/centerPlot.bmp");
			pix = pix.scaled(data->iconSize(), Qt::IgnoreAspectRatio);
			QTransform trans;
			int rotationIndex = data->iconRotation();
            switch(rotationIndex)
			{
			case 0:
				trans.rotate(0);
				break;
			case 1:
				trans.rotate(90);
				break;
			case 2:
				trans.rotate(180);
				break;
			case 3:
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
			m_mapScatter[dataPair].graph->setScatterStyle(style);
		}
		m_mapScatter[dataPair].graph->setData(x, y);

		//Label Text
        if(data->isLabelTextShow())
		{
			//设置锚点
			m_mapScatter[dataPair].tracer->setGraphKey(x.last());
			// 			m_mapScatter[dataPair].tracer->setInterpolating(true);
			// 			m_mapScatter[dataPair].tracer->setStyle(QCPItemTracer::tsNone);

            // add label
            //m_mapScatter[dataPair].tracerText->position->setType(QCPItemPosition::ptPlotCoords);
            //m_mapScatter[dataPair].tracerText->position->setParentAnchor(m_mapScatter[dataPair].tracer->position);

			m_mapScatter[dataPair].tracer->setVisible(true);
			m_mapScatter[dataPair].tracerText->setVisible(true);
			//设置锚点
			m_mapScatter[dataPair].tracer->setGraphKey(x.last());

            if(0 == data->getTextFormat()) //default
			{
				QString labelText = nullptr, prefix_x = nullptr, prefix_y = nullptr;
				QString object_x = nullptr, object_y = nullptr, attr_x = nullptr, attr_y = nullptr;
                QString data_x = nullptr, data_y = nullptr, unit_x = nullptr, unit_y = nullptr,
                        Left_bracket = nullptr, right_bracket = nullptr;

				//考虑仅显示实体名时的操作
                if(data->isObjectShow() && !data->isPrefixShow() && !data->isAttrShow() &&
                   !data->isDataShow() && !data->isUnitShow())
				{
					object_x = data->getObjectName_x();
					object_y = data->getObjectName_y();
					//实体名相同时，仅显示一个实体名
                    if(0 == object_x.compare(object_y) && object_x != nullptr &&
                       object_y != nullptr)
					{
						labelText = object_x;
					}
                    else if(object_x == nullptr && object_y == nullptr)
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
						object_x = data->getObjectName_x();
						object_y = data->getObjectName_y();
					}

                    if(data->isAttrShow())
					{
						attr_x = data->getAttrName_x();
						attr_y = data->getAttrName_y();
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
				data->setLabelText(labelText);
			}
            else if(1 == data->getTextFormat()) //custom
			{
				data->setLabelText(data->getCustomText());
			}
            else if(2 == data->getTextFormat()) //script
            {}
			m_mapScatter[dataPair].tracerText->setText(data->getLabelText());

			QFontMetricsF fm(data->getLabelFont());
			double wd = (fm.size(Qt::TextSingleLine, data->getLabelText()).width()) / 3.0;
			double ht = fm.size(Qt::TextSingleLine, data->getLabelText()).height() / 1.0;
            switch(data->getLabelPosition())
			{
            case 0: //left-top
				m_mapScatter[dataPair].tracerText->position->setCoords(-wd, -ht);
				break;
            case 1: //top
				m_mapScatter[dataPair].tracerText->position->setCoords(0, -ht);
				break;
            case 2: //right-top
				m_mapScatter[dataPair].tracerText->position->setCoords(wd, -ht);
				break;
            case 3: //left
				m_mapScatter[dataPair].tracerText->position->setCoords(-wd, 0);
				break;
            case 4: //center
				m_mapScatter[dataPair].tracerText->position->setCoords(0, 0);
				break;
            case 5: //right
				m_mapScatter[dataPair].tracerText->position->setCoords(wd, 0);
				break;
            case 6: //left-bottom
				m_mapScatter[dataPair].tracerText->position->setCoords(-wd, ht);
				break;
            case 7: //bottom
				m_mapScatter[dataPair].tracerText->position->setCoords(0, ht);
				break;
            case 8: //right-bottom
				m_mapScatter[dataPair].tracerText->position->setCoords(wd, ht);
				break;
            default: //right
				m_mapScatter[dataPair].tracerText->position->setCoords(wd, 0);
				break;
			}
			m_mapScatter[dataPair].tracerText->setPositionAlignment(Qt::AlignCenter);
			m_mapScatter[dataPair].tracerText->setTextAlignment(Qt::AlignLeft);
			m_mapScatter[dataPair].tracerText->setFont(data->getLabelFont());
			m_mapScatter[dataPair].tracerText->setColor(data->getLabelColor());
            if(data->getLabelBackTransparent())
				m_mapScatter[dataPair].tracerText->setBrush(Qt::transparent);
			else
				m_mapScatter[dataPair].tracerText->setBrush(data->getLabelBackground());
		}
		else
		{
			m_mapScatter[dataPair].tracer->setVisible(false);
			m_mapScatter[dataPair].tracerText->setVisible(false);
		}
	}
	else
	{
		m_mapScatter[dataPair].graph->setVisible(false);
		m_mapScatter[dataPair].tracer->setVisible(false);
		m_mapScatter[dataPair].tracerText->setVisible(false);
	}
}

void PlotScatter::onGetCurrentSeconds(double secs)
{
	m_curSeconds = secs;
	//    update();
	getDataInfo(m_curSeconds);
}

void PlotScatter::paintEvent(QPaintEvent* event)
{
	int width = this->width();
	int height = this->height();

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h = fm.size(Qt::TextSingleLine, m_title).height();
	double as = fm.ascent();
	//绘制标题
    if(m_titleVisible)
	{
		painter.setFont(m_titleFont);
		painter.setPen(m_titleColor);
        painter.fillRect(
            (width - w + m_leftPadding - m_rightPadding) / 2, m_topPadding, w, h, m_titleFillColor);
        painter.drawText(
            QPoint((width - w + m_leftPadding - m_rightPadding) / 2, as + m_topPadding), m_title);
	}

    m_customPlot->setGeometry(m_leftPadding,
                              h + m_topPadding,
                              width - m_leftPadding - m_rightPadding,
                              height - h - m_topPadding - m_bottomPadding);

    PlotItemBase::paintEvent(event);
}

void PlotScatter::setPaddings(double top, double bottom, double left, double right)
{
	m_topPadding = top;
	m_bottomPadding = bottom;
	m_leftPadding = left;
	m_rightPadding = right;
	update();
}

void PlotScatter::setTitle(QString& str)
{
	m_title = str;
	update();
}

void PlotScatter::setTitleColor(QColor& color)
{
	m_titleColor = color;
	update();
}

void PlotScatter::setTitleFillColor(QColor& color)
{
	m_titleFillColor = color;
	update();
}

void PlotScatter::setTitleFont(QFont& font)
{
	m_titleFont = font;
	update();
}

void PlotScatter::setTitleFontSize(int size)
{
	m_titleFontSize = size;
	m_titleFont.setPointSize(size);
	update();
}

void PlotScatter::setTitleVisible(bool show)
{
	m_titleVisible = show;
	update();
}

void PlotScatter::setxAxisLabel(QString& str)
{
	m_xAxisLabel = str;
	m_customPlot->xAxis->setLabel(m_xAxisLabel);
}

void PlotScatter::setyAxisLabel(QString& str)
{
	m_yAxisLabel = str;
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->replot();
}

void PlotScatter::setAxisLabelColor(QColor& color)
{
	m_axisLabelColor = color;
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->replot();
}

void PlotScatter::setAxisLabelFont(QFont& font)
{
	m_axisLabelFont = font;
	m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->yAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->replot();
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

void PlotScatter::setHorzGrids(uint count)
{
    if(m_horzGrids == count || count < 0)
	{
		return;
	}
	m_horzGrids = count;
    if(count == 0)
	{
		m_customPlot->yAxis->grid()->setVisible(false);
	}
	else
	{
		m_customPlot->yAxis->grid()->setVisible(m_gridVisible);
		m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
	}
	m_customPlot->replot();
}

void PlotScatter::setVertGrids(uint count)
{
    if(m_vertGrids == count || count < 0)
	{
		return;
	}
	m_vertGrids = count;
    if(count == 0)
	{
		m_customPlot->xAxis->grid()->setVisible(false);
	}
	else
	{
		m_customPlot->xAxis->grid()->setVisible(true);
		m_customPlot->xAxis->ticker()->setTickCount(m_vertGrids);
	}
	m_customPlot->replot();
}

void PlotScatter::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
	m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->replot();
}

void PlotScatter::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->replot();
}

void PlotScatter::setGridFillColor(QColor color)
{
	m_gridFillColor = color;
	m_customPlot->axisRect()->setBackground(color);
	m_customPlot->replot();
}

void PlotScatter::setGridVisible(bool enable)
{
	m_gridVisible = enable;
	m_customPlot->xAxis->grid()->setVisible(enable);
	m_customPlot->yAxis->grid()->setVisible(enable);
	m_customPlot->replot();
}

void PlotScatter::setTickLabelColor(QColor& color)
{
	m_tickLabelColor = color;
	m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
	m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);
	m_customPlot->replot();
}

void PlotScatter::setTickLabelFont(QFont& font)
{
	m_tickLabelFont = font;
	m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
	m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);
	m_customPlot->replot();
}

void PlotScatter::setTickLabelFontSize(int size)
{
	m_tickLabelFontSize = size;
	m_tickLabelFont.setPointSize(size);
	setTickLabelFont(m_tickLabelFont);
}

void PlotScatter::setGridStyle(GridStyle style)
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
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->replot();
}

void PlotScatter::setGridDensity(GridDensity density) {}

void PlotScatter::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
	m_customPlot->xAxis->setAxisFormatShow(on);
	m_customPlot->replot();
}

void PlotScatter::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
	m_customPlot->yAxis->setAxisFormatShow(on);
	m_customPlot->replot();
}

void PlotScatter::setUnitsX(const QString& units)
{
	m_units_x = units;
	m_customPlot->xAxis->setAxisFormat(units);
	m_customPlot->replot();
}

void PlotScatter::setUnitsY(const QString& units)
{
	m_units_y = units;
	m_customPlot->yAxis->setAxisFormat(units);
	m_customPlot->replot();
}
