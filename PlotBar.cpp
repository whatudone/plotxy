#include "PlotBar.h"
#include "DataManager.h"
#include "Utils.h"
#include <QDebug>
#include <QPainter>
int PlotBar::m_instanceCount = 1;
PlotBar::PlotBar(QWidget* parent)
    : PlotItemBase(parent)

{
    QString name = QString("Bar%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_title = "Bar";

    m_xAxisLabel = "X Axis";
    m_yAxisLabel = "Y Axis";

    initPlot();
    setupLayout();
}

PlotBar::~PlotBar() {}

void PlotBar::updateDataForDataPairsByTime(double secs)
{
    m_curValue.clear();
    int itemCnt = getDataPairs().size();

    // 获取数据
    for(int i = 0; i < itemCnt; i++)
    {
        auto data = getDataPairs().at(i);
        auto uuid = data->getUuid();
        auto xEntityID = data->getEntityIDX();
        auto xAttr = data->getAttr_x();
        auto xDataType = data->getXDataType();
        double value = 0.0;
        if(xDataType == DataPair::Parameter)
        {
            value =
                DataManagerInstance->getEntityAttrValueByMaxTime(xEntityID, xAttr, secs, m_xRate);
        }
        else
        {
            auto xTargetEntityID = data->getTargetEntityIDX();
            auto xCalType = data->getXCalType();
            value = DataManagerInstance->rangeCalculation(
                xEntityID, xTargetEntityID, xCalType, secs, m_xRate);
        }

        if(math::doubleEqual(value, std::numeric_limits<double>::max())) // 表示value无效
        {
            value = 0;
        }

        m_curValue.insert(uuid, value);
    }

    updateKeyAxisTickLabel();

    for(int i = 0; i < itemCnt; ++i)
    {
        updateGraphByDataPair(m_dataPairs.at(i), secs);
    }

    m_customPlot->replot();
}

void PlotBar::initPlot()
{
    // 开始默认是水平方向，所以keyAxis为yAxis
    m_customPlot = new QCustomPlot();
    valueAxis()->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    keyAxis()->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    valueAxis()->ticker()->setTickCount(m_vertGrids);
    keyAxis()->ticker()->setTickCount(m_horzGrids);
    valueAxis()->setTickLabelColor(m_yTickLabelColor);
    keyAxis()->setTickLabelColor(m_xTickLabelColor);
    valueAxis()->setTickLabelFont(m_yTickLabelFont);
    keyAxis()->setTickLabelFont(m_xTickLabelFont);
    valueAxis()->setBasePen(QPen(m_axisColor, m_axisWidth));
    keyAxis()->setBasePen(QPen(m_axisColor, m_axisWidth));
    valueAxis()->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    keyAxis()->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));

    m_customPlot->axisRect()->setBackground(m_gridFillColor);

    keyAxis()->setTickLabelRotation(45); // 轴刻度文字旋转45度
    keyAxis()->setSubTicks(false); // 不显示子刻度
    keyAxis()->setTickLength(0, 4); // 轴内外刻度的长度分别是0,4,也就是轴内的刻度线不显示
    keyAxis()->setUpperEnding(QCPLineEnding::esSpikeArrow);

    valueAxis()->setPadding(30);
    valueAxis()->setUpperEnding(QCPLineEnding::esSpikeArrow);

    m_customPlot->xAxis->setLabel(m_xAxisLabel);
    m_customPlot->yAxis->setLabel(m_yAxisLabel);
    m_customPlot->xAxis->setLabelColor(m_xAxisLabelColor);
    m_customPlot->yAxis->setLabelColor(m_yAxisLabelColor);
    m_customPlot->xAxis->setLabelFont(m_xAxisLabelFont);
    m_customPlot->yAxis->setLabelFont(m_yAxisLabelFont);
}

void PlotBar::updateGraphByDataPair(DataPair* data, double curSecs)
{
    if(!data)
    {
        return;
    }

    auto uuid = data->getUuid();
    // 清理原始数据对对应Bar
    if(m_allBar.contains(uuid))
    {
        for(int i = 0; i < m_allBar[uuid].size(); i++)
        {
            m_customPlot->removePlottable(m_allBar[uuid].at(i));
        }
        m_allBar.remove(uuid);
    }
    if(m_barColorInfoMap.contains(uuid))
    {
        m_barColorInfoMap[uuid].clear();
    }
    // 有效的ColorRange需要将原始的单个Bar拆分成多个Bar,无效的则直接使用添加数据对时产生的默认单个Bar
    if(data->getColorRangeEnable())
    {
        auto mode = data->getColorRangeMode();
        if(mode == DataPair::MutilColor && isValidColorRange(data))
        {
            QList<std::tuple<QString, double, QColor>> colorList = data->getColorRanges();
            m_allColorInfoList.insert(uuid, colorList);

            for(int i = 0; i < m_allColorInfoList[uuid].size(); i++)
            {
                double thresholdValue = std::get<1>(m_allColorInfoList[uuid].at(i));
                QColor color = std::get<2>(m_allColorInfoList[uuid].at(i));
                m_barColorInfoMap[uuid].insert(thresholdValue, color);
            }

            QList<QCPBars*> tarBar;
            // colorRange下限前面的部分，用默认颜色绘制
            QCPBars* subBar = new QCPBars(keyAxis(), valueAxis());
            subBar->setPen(QPen(data->getColorRangeDefaultColor().lighter(130)));
            subBar->setBrush(data->getColorRangeDefaultColor());
            tarBar.push_back(subBar);

            for(auto it = m_barColorInfoMap[uuid].constBegin();
                it != m_barColorInfoMap[uuid].constEnd();
                it++)
            {
                QCPBars* subBar = new QCPBars(keyAxis(), valueAxis());
                subBar->setPen(QPen(it.value().lighter(130)));
                subBar->setBrush(it.value());
                tarBar.push_back(subBar);
            }
            m_allBar.insert(uuid, tarBar);

            if(tarBar.size() > 1)
            {
                for(int i = 1; i < tarBar.size(); i++)
                {
                    tarBar.at(i)->moveAbove(tarBar.at(i - 1));
                }
            }
        }
        // 渐变先按照单色处理
        if(mode == DataPair::SingleColor || mode == DataPair::Gradient)
        {
            QList<QCPBars*> tarBar;
            QCPBars* subBar = new QCPBars(keyAxis(), valueAxis());
            subBar->setPen(QPen(data->getColorRangeDefaultColor().lighter(130)));
            subBar->setBrush(data->getColorRangeDefaultColor());
            tarBar.push_back(subBar);
            m_allBar.insert(uuid, tarBar);
            QMap<double, QColor> baseBarMap;
            baseBarMap.insert(0.0, data->getColorRangeDefaultColor());
            m_barColorInfoMap.insert(uuid, baseBarMap);
        }
    }
    else
    {
        // 没有启动ColorRange就使用数据颜色绘制
        QList<QCPBars*> tarBar;
        QCPBars* subBar = new QCPBars(keyAxis(), valueAxis());
        subBar->setPen(QPen(data->dataColor().lighter(130)));
        subBar->setBrush(data->dataColor());
        tarBar.push_back(subBar);
        m_allBar.insert(uuid, tarBar);
        QMap<double, QColor> baseBarMap;
        baseBarMap.insert(0.0, data->dataColor());
        m_barColorInfoMap.insert(uuid, baseBarMap);
    }
    /*
     * 没有进行ColorRange处理使用默认生成的Bar
     * 设置了正确的colorRange，每个target上绘制多个Bar
    */

    if(!m_barColorInfoMap.contains(uuid) || !m_allBar.contains(uuid))
        return;
    QList<QCPBars*> barList = m_allBar[uuid];
    if(barList.isEmpty())
        return;
    auto value = m_curValue[uuid];
    QMap<double, QColor> colorMap = m_barColorInfoMap[uuid];

    QCPItemText* valueLabel = nullptr;
    if(m_barValueLabelHash.contains(uuid))
    {
        valueLabel = m_barValueLabelHash.value(uuid);
    }
    else
    {
        valueLabel = new QCPItemText(m_customPlot);
        valueLabel->position->setType(QCPItemPosition::ptAbsolute);
        valueLabel->setPositionAlignment(Qt::AlignCenter);
        m_barValueLabelHash.insert(uuid, valueLabel);
    }
    if(data->isDraw())
    {
        for(auto bar : barList)
        {
            bar->setVisible(true);
        }
        // 获取数据对原始的序号，考虑到Bar的特殊性需要从1开始
        int index = getDataPairs().indexOf(data) + 1;

        if(barList.size() == 1)
        {
            barList.at(0)->setData(QVector<double>() << index, QVector<double>() << value);
        }
        else
        {
            QList<double> valueList;
            valueList.push_back(m_itemData[uuid]);
            for(auto it = colorMap.constBegin(); it != colorMap.constEnd(); it++)
            {
                if(value - it.key() > 0)
                {
                    // 表示数值在标记的范围内
                    valueList.push_back(it.key());
                    continue;
                }
            }
            valueList.push_back(value);

            if(value < valueList.at(0))
            {
                barList.at(0)->setData(QVector<double>() << index, QVector<double>() << 0.0);
            }
            else
            {
                barList.at(0)->setData(QVector<double>() << index,
                                       QVector<double>() << valueList.at(1));
            }

            if(valueList.size() > 1)
            {

                for(int cnt = 1; cnt < valueList.size() - 1; cnt++)
                {
                    barList.at(cnt)->setData(QVector<double>() << index,
                                             QVector<double>()
                                                 << (valueList.at(cnt + 1) - valueList.at(cnt)));
                }
            }
        }

        if(data->isLabelTextShow())
        {
            valueLabel->setVisible(true);
            QString text = data->processLabelText(value);
            valueLabel->setText(text);
            double lastPosX = keyAxis()->coordToPixel(index);
            double lastPosY = valueAxis()->coordToPixel(value);
            QPointF lastPoint =
                getIsHorizonBar() ? QPointF(lastPosY, lastPosX) : QPointF(lastPosX, lastPosY);
            QPointF labelPoint = data->processBarLabelPosition(lastPoint, text, getIsHorizonBar());
            valueLabel->position->setCoords(labelPoint);
            valueLabel->setFont(data->getLabelFont());
            valueLabel->setColor(data->getLabelColor());
            valueLabel->setBrush(data->getLabelBackground());
            // 保证这个label不会被Bar覆盖，让他处于绘制的较上层
            valueLabel->setLayer("axes");
        }
        else
        {
            valueLabel->setVisible(false);
        }
    }
    else
    {
        for(auto bar : barList)
        {
            bar->setVisible(false);
        }
        valueLabel->setVisible(false);
    }
}

void PlotBar::setIsHorizonBar(bool isHorizonBar)
{
    bool oldHorizon = getIsHorizonBar();
    if(oldHorizon != isHorizonBar)
    {
        PlotItemBase::setIsHorizonBar(isHorizonBar);

        keyAxis()->setRange(m_coordBgn_x, m_coordEnd_x);
        valueAxis()->setRange(m_coordBgn_y, m_coordEnd_y);
        exchangeKeyAndValueAxisTickLabel();
        // 切换已经存在的Bar的key轴 value轴
        for(auto barList : m_allBar)
        {
            for(int i = 0; i < barList.size(); i++)
            {
                if(getIsHorizonBar())
                {
                    barList.at(i)->setKeyAxis(m_customPlot->yAxis);
                    barList.at(i)->setValueAxis(m_customPlot->xAxis);
                }
                else
                {
                    barList.at(i)->setKeyAxis(m_customPlot->xAxis);
                    barList.at(i)->setValueAxis(m_customPlot->yAxis);
                }
            }
        }

        m_customPlot->replot();
    }
}

QCPAxis* PlotBar::keyAxis()

{
    return getIsHorizonBar() ? m_customPlot->yAxis : m_customPlot->xAxis;
}

QCPAxis* PlotBar::valueAxis()
{
    return getIsHorizonBar() ? m_customPlot->xAxis : m_customPlot->yAxis;
}

void PlotBar::updateKeyAxisTickLabel()
{
    QVector<double> barTicks;
    QVector<QString> labels;
    int itemCnt = getDataPairs().size();
    for(int i = 0; i < itemCnt; i++)
    {
        auto data = getDataPairs().at(i);
        auto uuid = data->getUuid();
        auto label = m_tickLabelMap.value(uuid);
        // 序号需要从1开始
        barTicks << i + 1;
        labels << label;
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(barTicks, labels);
    keyAxis()->setTicker(textTicker);

    // 坐标轴范围由绘图管理器统一控制
    // 关于间隔的设置，和坐标轴范围设置功能重叠，以后再将这两个功能改善一下
    //    int barLeftPadding = getBarLeftPadding();
    //    int barRightPadding = getBarRightPadding();
    //    int barPadding = barLeftPadding + barRightPadding;
    //    double offset = 0.0;
    //    if(getIsHorizonBar())
    //    {
    //        m_customPlot->yAxis->setTicker(textTicker); // 是水平轴，则x轴为数值，y轴为文字
    //        if(barPadding != 0)
    //        {
    //            int axisHeight = m_customPlot->axisRect()->height();
    //            offset = ((index - 1.3) * barPadding) / (axisHeight - barPadding);
    //            double leftOffset = double(barLeftPadding) / barPadding * offset;
    //            double rightOffset = double(barRightPadding) / barPadding * offset;
    //            m_customPlot->yAxis->setRange(0.625 - leftOffset, index - 0.625 + rightOffset);
    //        }
    //        else
    //            m_customPlot->yAxis->setRange(0.625, index - 0.625);
    //        m_customPlot->xAxis->setRange(m_min, m_max);
    //        m_customPlot->xAxis->setTicks(true);
    //    }
    //    else
    //    {
    //        m_customPlot->xAxis->setTicker(textTicker); // 是竖直轴，则y轴为数值，x轴为文字
    //        if(barPadding != 0)
    //        {
    //            int axisWidth = m_customPlot->axisRect()->width();
    //            offset = ((index - 1.3) * barPadding) / (axisWidth - barPadding);
    //            double leftOffset = double(barLeftPadding) / barPadding * offset;
    //            double rightOffset = double(barRightPadding) / barPadding * offset;
    //            m_customPlot->xAxis->setRange(0.625 - leftOffset, index - 0.625 + rightOffset);
    //        }
    //        else
    //            m_customPlot->xAxis->setRange(0.625, index - 0.625);
    //        m_customPlot->yAxis->setRange(m_min, m_max);
    //        m_customPlot->yAxis->setTicks(true);
    //    }
    m_customPlot->replot();
}

void PlotBar::exchangeKeyAndValueAxisTickLabel()
{
    auto oldKeyTicker = keyAxis()->ticker();
    auto oldValueTicker = valueAxis()->ticker();
    keyAxis()->setTicker(oldValueTicker);
    valueAxis()->setTicker(oldKeyTicker);
}

bool PlotBar::isValidColorRange(DataPair* data)
{
    QList<std::tuple<QString, double, QColor>> colorList = data->getColorRanges();
    if(colorList.isEmpty())
    {
        // 表示未设置显示范围，则全用默认颜色绘制
        return false;
    }

    // 如果用户设置的colorRange范围在所有数据的最小值和最大值之外，则认为设置无效，跳过本数据对，采用默认颜色
    for(int i = 0; i < colorList.size(); i++)
    {
        if(std::get<1>(colorList.at(i)) < m_coordBgn_y ||
           std::get<1>(colorList.at(i)) > m_coordEnd_y)
        {
            return false;
        }
    }
    return true;
}

DataPair* PlotBar::addPlotDataPair(int32_t xEntityID,
                                   const QString& xAttrName,
                                   const QString& xAttrUnitName,
                                   int32_t yEntityID,
                                   const QString& yAttrName,
                                   const QString& yAttrUnitName,
                                   const QHash<QString, QVariant>& extraParams,
                                   bool isFromJson)
{
    Q_UNUSED(extraParams)
    DataPair* data =
        new DataPair(xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName);
    m_dataPairs.append(data);
    if(!isFromJson)
    {
        m_units_x = xAttrUnitName;
        m_units_y = yAttrUnitName;
    }

    if(extraParams.contains("UUID"))
    {
        data->setUuid(extraParams.value("UUID").toString());
    }
    if(extraParams.contains("XDataType"))
    {
        data->setXDataType(static_cast<DataPair::DataType>(extraParams.value("XDataType").toInt()));
    }
    if(extraParams.contains("YDataType"))
    {
        data->setYDataType(static_cast<DataPair::DataType>(extraParams.value("YDataType").toInt()));
    }
    if(extraParams.contains("XCalType"))
    {
        data->setXCalType(
            static_cast<DataPair::RangeCalculationType>(extraParams.value("XCalType").toInt()));
    }
    if(extraParams.contains("YCalType"))
    {
        data->setYCalType(
            static_cast<DataPair::RangeCalculationType>(extraParams.value("YCalType").toInt()));
    }
    if(extraParams.contains("XEntityName"))
    {
        data->setEntity_x(extraParams.value("XEntityName").toString());
    }
    if(extraParams.contains("YEntityName"))
    {
        data->setEntity_y(extraParams.value("YEntityName").toString());
    }
    if(extraParams.contains("XTargetID"))
    {
        data->setTargetEntityIDX(extraParams.value("XTargetID").toInt());
    }
    if(extraParams.contains("YTargetID"))
    {
        data->setTargetEntityIDY(extraParams.value("YTargetID").toInt());
    }
    if(extraParams.contains("XTargetEntityName"))
    {
        data->setTargetEntityX(extraParams.value("XTargetEntityName").toString());
    }
    if(extraParams.contains("YTargetEntityName"))
    {
        data->setTargetEntityY(extraParams.value("YTargetEntityName").toString());
    }

    connect(data,
            &DataPair::dataUpdate,
            this,
            &PlotItemBase::onDataPairUpdateData,
            Qt::UniqueConnection);
    QString uuid = data->getUuid();

    QColor dataColor = data->dataColor();
    QCPBars* pBar = new QCPBars(keyAxis(), valueAxis());
    pBar->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    pBar->setPen(QPen(dataColor.lighter(130)));
    pBar->setBrush(dataColor);

    QMap<double, QColor> baseBarMap;
    baseBarMap.insert(0.0, dataColor);
    m_barColorInfoMap.insert(uuid, baseBarMap);
    QList<QCPBars*> baseBar;
    baseBar.push_back(pBar);
    m_allBar.insert(uuid, baseBar);
    if(data->getXDataType() == DataPair::RangeCalculation)
    {
        m_tickLabelMap.insert(uuid, data->getEntity_x() + " VS " + data->getTargetEntityX());
    }
    else
    {
        m_tickLabelMap.insert(uuid, data->getEntity_x());
    }

    QPair<double, double> limit =
        DataManager::getInstance()->getMaxAndMinEntityAttrValue(xEntityID, xAttrName, m_xRate);

    m_itemData.insert(uuid, limit.first);

    updateKeyAxisTickLabel();
    if(!isFromJson)
    {
        emit dataPairsChanged(this);
    }

    return data;
}

void PlotBar::delPlotPairData(const QString& uuid)
{
    if(m_dataPairs.isEmpty())
        return;
    if(m_barColorInfoMap.contains(uuid))
    {
        m_barColorInfoMap.remove(uuid);
    }
    if(m_itemData.contains(uuid))
    {
        m_itemData.remove(uuid);
    }
    if(m_tickLabelMap.contains(uuid))
    {
        m_tickLabelMap.remove(uuid);
    }
    if(m_allBar.contains(uuid))
    {
        for(int i = 0; i < m_allBar[uuid].size(); i++)
        {
            m_customPlot->removePlottable(m_allBar[uuid].at(i));
        }
        m_allBar.remove(uuid);
    }
    if(m_barValueLabelHash.contains(uuid))
    {
        m_customPlot->removeItem(m_barValueLabelHash.value(uuid));
        m_barValueLabelHash.remove(uuid);
    }
    updateKeyAxisTickLabel();
    PlotItemBase::delPlotPairData(uuid);
}
