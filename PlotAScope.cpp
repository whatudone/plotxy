#include "PlotAScope.h"
#include "DataManager.h"

int PlotAScope::m_instanceCount = 1;
PlotAScope::PlotAScope(QWidget* parent)
    : PlotItemBase(parent)
{
    QString name = QString("A-Scope%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_outerFillColor = Qt::black;
    m_gridFillColor = Qt::black;
    m_title = "A-Scope";

    m_axisLabelFont.setFamily("Microsoft YaHei");
    m_axisLabelFont.setPointSizeF(10.0);
    m_xAxisLabel = "Range(m)";
    m_yAxisLabel = "Voltage(V)";

    m_coordBgn_x = 0;
    m_coordEnd_x = 100;
    m_coordBgn_y = 0;
    m_coordEnd_y = 100;

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

PlotAScope::~PlotAScope() {}

void PlotAScope::initPlot()
{
    m_customPlot = new QCustomPlot(this);
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_customPlot);
    setLayout(pLayout);
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
    m_customPlot->xAxis->setRange(m_coordBgn_x, m_coordEnd_x);
    m_customPlot->yAxis->setRange(m_coordBgn_y, m_coordEnd_y);

    m_customPlot->setBackground(m_outerFillColor);
    m_customPlot->axisRect()->setBackground(m_gridFillColor);
    m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
    m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
    m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
    m_customPlot->yAxis->setLabelFont(m_axisLabelFont);

    m_customPlot->replot();
}

void PlotAScope::setPaddings(double top, double bottom, double left, double right)
{
    m_topPadding = top;
    m_bottomPadding = bottom;
    m_leftPadding = left;
    m_rightPadding = right;
    update();
}

void PlotAScope::setUnitsShowX(bool on)
{
    m_showUnits_x = on;
    m_customPlot->xAxis->setAxisFormatShow(on);
    m_customPlot->replot();
}

void PlotAScope::setUnitsShowY(bool on)
{
    m_showUnits_y = on;
    m_customPlot->yAxis->setAxisFormatShow(on);
    m_customPlot->replot();
}

void PlotAScope::setUnitsX(const QString& units)
{
    m_units_x = units;
    m_customPlot->xAxis->setAxisFormat(units);
    m_customPlot->replot();
}

void PlotAScope::setUnitsY(const QString& units)
{
    m_units_y = units;
    m_customPlot->yAxis->setAxisFormat(units);
    m_customPlot->replot();
}

void PlotAScope::setTitle(const QString& str)
{
    m_title = str;
    update();
}

void PlotAScope::setTitleColor(const QColor& color)
{
    m_titleColor = color;
    update();
}

void PlotAScope::setTitleFont(const QFont& font)
{
    m_titleFont = font;
    update();
}

void PlotAScope::setTitleVisible(bool show)
{
    m_titleVisible = show;
    update();
}

void PlotAScope::setTitleFontSize(int size)
{
    m_titleFontSize = size;
    m_titleFont.setPointSize(size);
    update();
}

void PlotAScope::setTitleFillColor(QColor& color)
{
    m_titleFillColor = color;
    update();
}

void PlotAScope::delPlotPairData(const QString& uuid)
{
    if(m_dataPairs.isEmpty())
        return;
    if(m_graphMap.contains(uuid))
    {
        auto graph = m_graphMap.take(uuid);
        m_customPlot->removeGraph(graph);
    }

    PlotItemBase::delPlotPairData(uuid);
}

void PlotAScope::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
        return;

    int itemCnt = m_dataPairs.size();

    for(int i = 0; i < itemCnt; ++i)
    {
        updateGraph(secs, m_dataPairs.at(i));
    }
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotAScope::updateGraph(double secs, DataPair* data)
{
    if(!data)
    {
        return;
    }
    QString uuid = data->getUuid();
    QCPGraph* graph = nullptr;
    if(m_graphMap.contains(uuid))
    {
        graph = m_graphMap.value(uuid);
    }
    else
    {
        graph = m_customPlot->addGraph();
        m_graphMap.insert(uuid, graph);
    }
    if(!graph)
    {
        return;
    }
    if(data->isDraw())
    {
        QVector<double> x, y;
        // TODO:数据结构还没定好，暂时没有数据提供

        if(x.isEmpty() || y.isEmpty())
            return;
        graph->setVisible(true);
        graph->setPen(QPen(data->dataColor(), data->lineWidth()));

        //icon
        if(data->isIconDraw())
        {
            //test
            QPixmap pix(data->iconName());
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
            //            QCPScatterStyle style(pix);
            //            graph->setScatterStyle(style);
        }
        graph->setData(x, y);
        m_customPlot->xAxis->setRange(x.first(), x.last());
        m_customPlot->yAxis->setRange(y.first(), y.last());
        //Label Text
        if(data->isLabelTextShow())
        {

            if(0 == data->getTextFormat())
            { //default
                QString labelText, prefix_x, prefix_y;
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
                data->setLabelText(labelText);
            }
            else if(1 == data->getTextFormat())
            { //custom
                data->setLabelText(data->getCustomText());
            }
            else if(2 == data->getTextFormat()) //script
            {}
            //            m_graphMap[dataPair].tracerText->setText(data->getLabelText());

            //            QFontMetricsF fm(data->getLabelFont());
            //            double wd = (fm.size(Qt::TextSingleLine, data->getLabelText()).width()) / 3.0;
            //            double ht = fm.size(Qt::TextSingleLine, data->getLabelText()).height() / 1.0;
            //            switch(data->getLabelPosition())
            //            {
            //            case 0: //left-top
            //                m_graphMap[dataPair].tracerText->position->setCoords(-wd, -ht);
            //                break;
            //            case 1: //top
            //                m_graphMap[dataPair].tracerText->position->setCoords(0, -ht);
            //                break;
            //            case 2: //right-top
            //                m_graphMap[dataPair].tracerText->position->setCoords(wd, -ht);
            //                break;
            //            case 3: //left
            //                m_graphMap[dataPair].tracerText->position->setCoords(-wd, 0);
            //                break;
            //            case 4: //center
            //                m_graphMap[dataPair].tracerText->position->setCoords(0, 0);
            //                break;
            //            case 5: //right
            //                m_graphMap[dataPair].tracerText->position->setCoords(wd, 0);
            //                break;
            //            case 6: //left-bottom
            //                m_graphMap[dataPair].tracerText->position->setCoords(-wd, ht);
            //                break;
            //            case 7: //bottom
            //                m_graphMap[dataPair].tracerText->position->setCoords(0, ht);
            //                break;
            //            case 8: //right-bottom
            //                m_graphMap[dataPair].tracerText->position->setCoords(wd, ht);
            //                break;
            //            default: //right
            //                m_graphMap[dataPair].tracerText->position->setCoords(wd, 0);
            //                break;
            //            }
            //            m_graphMap[dataPair].tracerText->setPositionAlignment(Qt::AlignCenter);
            //            m_graphMap[dataPair].tracerText->setTextAlignment(Qt::AlignLeft);
            //            m_graphMap[dataPair].tracerText->setFont(data->getLabelFont());
            //            m_graphMap[dataPair].tracerText->setColor(data->getLabelColor());
            //            if(data->getLabelBackTransparent())
            //                m_graphMap[dataPair].tracerText->setBrush(Qt::transparent);
            //            else
            //                m_graphMap[dataPair].tracerText->setBrush(data->getLabelBackground());
        }
        else
        {
            //            m_graphMap[dataPair].tracer->setVisible(false);
            //            m_graphMap[dataPair].tracerText->setVisible(false);
        }
    }
    else
    {
        graph->setVisible(false);
        //        m_graphMap[dataPair].tracer->setVisible(false);
        //        m_graphMap[dataPair].tracerText->setVisible(false);
    }
}

void PlotAScope::setxAxisLabel(const QString& str)
{
    m_xAxisLabel = str;
    m_customPlot->xAxis->setLabel(m_xAxisLabel);
    m_customPlot->replot();
}

void PlotAScope::setyAxisLabel(const QString& str)
{
    m_yAxisLabel = str;
    m_customPlot->yAxis->setLabel(m_yAxisLabel);
    m_customPlot->replot();
}

void PlotAScope::setAxisLabelColor(QColor& color)
{
    m_axisColor = color;
    m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
    m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
    m_customPlot->replot();
}

void PlotAScope::setAxisLabelFont(QFont& font)
{
    m_axisLabelFont = font;
    m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
    m_customPlot->yAxis->setLabelFont(m_axisLabelFont);
    m_customPlot->replot();
}

void PlotAScope::setAxisVisible(bool on, AxisType type)
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

void PlotAScope::setAxisTickLabelShow(bool on, AxisType type)
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

void PlotAScope::setHorzGrids(uint count)
{
    if(m_horzGrids == count)
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
        m_customPlot->yAxis->grid()->setVisible(true);
        m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
    }
    m_customPlot->replot();
}

void PlotAScope::setVertGrids(uint count)
{
    if(m_vertGrids == count)
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

void PlotAScope::setAxisColorWidth(QColor color, uint width)
{
    m_axisColor = color;
    m_axisWidth = width;
    m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->replot();
}

void PlotAScope::setGridColorWidth(QColor color, uint width)
{
    m_gridColor = color;
    m_gridWidth = width;
    m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->replot();
}

void PlotAScope::setGridFillColor(QColor color)
{
    m_gridFillColor = color;
    m_customPlot->axisRect()->setBackground(color);
    m_customPlot->replot();
}

void PlotAScope::setGridVisible(bool enable)
{
    m_gridVisible = enable;
    m_customPlot->xAxis->grid()->setVisible(enable);
    m_customPlot->yAxis->grid()->setVisible(enable);
    m_customPlot->replot();
}

void PlotAScope::setTickLabelColor(QColor& color)
{
    m_tickLabelColor = color;
    m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
    m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);
    m_customPlot->replot();
}

void PlotAScope::setTickLabelFont(QFont& font)
{
    m_tickLabelFont = font;
    m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
    m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);
    m_customPlot->replot();
}

void PlotAScope::setTickLabelFontSize(int size)
{
    m_tickLabelFontSize = size;
    m_tickLabelFont.setPointSize(size);
    setTickLabelFont(m_tickLabelFont);
}

void PlotAScope::setGridStyle(GridStyle style)
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

void PlotAScope::setGridDensity(GridDensity density) {}

void PlotAScope::setMinimumMargins(const QMargins& margins)
{
    m_customPlot->axisRect()->setMinimumMargins(margins);
}
