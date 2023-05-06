#include "PlotBar.h"
#include "DataManager.h"
#include <QDebug>
#include <QPainter>
int PlotBar::m_instanceCount = 1;
PlotBar::PlotBar(QWidget* parent)
    : PlotItemBase(parent)
{
    QString name = QString("Bar%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_defaultColor = Qt::green;
    m_gridColor = Qt::darkGray;
	m_axisColor = Qt::white;
	m_titleColor = Qt::white;
    m_outerFillColor = Qt::black;
    m_gridFillColor = Qt::black;
	m_title = "Bar";
	m_titleFont.setFamily("Microsoft YaHei");
    m_titleFont.setPointSizeF(16.0);

    m_coordBgn_x = 0;
    m_coordEnd_x = 8;
    m_coordBgn_y = 0;
    m_coordEnd_y = 12.1;

    m_horzGrids = 5;
    m_vertGrids = 5;
    m_axisWidth = 1;
    m_gridWidth = 1;
    m_axisColor = Qt::white;
    m_gridColor = QColor(200, 200, 200);
    m_gridVisible = true;
    m_tickLabelColor = Qt::white;
    m_tickLabelFont.setFamily("Microsoft YaHei");
    m_tickLabelFont.setPointSizeF(8.0);
    m_gridStyle = Qt::DotLine;
    m_gridDensity = GridDensity::LESS;

    initPlot();
}

PlotBar::~PlotBar() {}

void PlotBar::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
		return;
    m_dataList.clear();
    int isize = getDataPairs().size();
    for(int i = 0; i < isize; i++)
    {
        auto dataPair = getDataPairs().at(i);
        auto xEntityID = dataPair->getEntityIDX();
        auto xEntityName = dataPair->getEntity_x();
        auto xAttr = dataPair->getAttr_x();
        QList<double> xSecList =
            DataManager::getInstance()->getEntityAttrValueListByMaxTime(xEntityID, xAttr, secs);

        if(xSecList.isEmpty())
            continue;

        //*获取当前Attr值
        double currValue = xSecList.last();
        QString currKey = xEntityName + '_' + xAttr;
        m_dataList.append({currValue, currKey});
    }
    updateGraph();
}

void PlotBar::initPlot()
{
    m_customPlot = new QCustomPlot();
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_customPlot);
    setLayout(pLayout);
    m_customPlot->axisRect()->setupFullAxesBox(true);
    m_customPlot->axisRect()->setMinimumMargins(QMargins(30, 15, 30, 15));

    m_customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->xAxis->ticker()->setTickCount(int(m_vertGrids));
    m_customPlot->yAxis->ticker()->setTickCount(int(m_horzGrids));
    m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
    m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);
    m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
    m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);
    m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));

    m_customPlot->setBackground(m_outerFillColor);
    m_customPlot->axisRect()->setBackground(m_gridFillColor);

    m_pBar = new QCPBars(m_customPlot->yAxis, m_customPlot->xAxis);

    m_pBar->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    m_pBar->setPen(QPen(m_defaultColor.lighter(130))); // 设置柱状图的边框颜色
    m_pBar->setBrush(m_defaultColor); // 设置柱状图的画刷颜色

    m_customPlot->yAxis->setTickLabelRotation(60); // 轴刻度文字旋转60度
    m_customPlot->yAxis->setSubTicks(false); // 不显示子刻度
    m_customPlot->yAxis->setTickLength(0, 4); // 轴内外刻度的长度分别是0,4,也就是轴内的刻度线不显示
    m_customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    m_customPlot->xAxis->setPadding(30);
    m_customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    m_customPlot->replot();
}

void PlotBar::updateGraph()
{
    int isize = m_dataList.size();
    if(isize <= 0)
    {
        return;
    }
    double min = 0.0;
    double max = 0.0;
    m_barTicks.clear();
    m_barLabels.clear();
    m_barData.clear();
    for(int i = 0; i < isize; i++)
    {
        m_barTicks << i + 1;
        QPair<double, QString> pair = m_dataList.at(i);
        double value = pair.first;
        QString attr = pair.second;
        m_barLabels << attr;
        m_barData << value;
        max = max > value ? max : value;
        min = min < value ? min : value;
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(m_barTicks, m_barLabels);
    m_customPlot->yAxis->setTicker(textTicker); // 设置为文字轴

    m_customPlot->yAxis->setRange(0, isize + 1); // 设置范围
    m_customPlot->xAxis->setRange(min > 0 ? min * 0.9 : min * 1.1, max > 0 ? max * 1.1 : max * 0.9);

    m_pBar->setData(m_barTicks, m_barData);
    m_customPlot->replot();
}
