#include "PlotLight.h"
#include "DataManager.h"
#include "PlotItemBase.h"
#include <QBrush>
#include <QDebug>
#include <QFontMetricsF>
#include <QList>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QPair>
#include <QPen>
#include <QString>

int PlotLight::m_instanceCount = 1;
PlotLight::PlotLight(QWidget* parent)
    : PlotItemBase(parent)
{
	QString name = QString("Light%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;
    m_title = "Events";
    initPlot();
}

PlotLight::~PlotLight() {}

void PlotLight::onLightConstraintUpdate(
    const QList<std::tuple<int32_t, QString, QString, double, QString>>& constraintList)
{
    m_constraintList = constraintList;
    updatePlotByCurrentData();
}

void PlotLight::updateDataForDataPairsByTime(double secs)
{
    m_dataList.clear();
    int isize = getDataPairs().size();
    for(int i = 0; i < isize; i++)
    {
        auto dataPair = getDataPairs().at(i);
        auto xEntityID = dataPair->getEntityIDX();
        auto xEntityName = dataPair->getEntity_x();
        auto xAttr = dataPair->getAttr_x();
        double value =
            DataManager::getInstance()->getEntityAttrValueByMaxTime(xEntityID, xAttr, secs);

        auto desc = dataPair->getDesc();
        m_dataList.append(std::make_tuple(desc, xEntityID, xEntityName, xAttr, value));
    }
    updatePlotByCurrentData();
}

void PlotLight::processDataByConstraints()
{
    m_drawDataList.clear();
    for(const auto& tuple : m_dataList)
    {
        QString desc = std::get<0>(tuple);
        int32_t id = std::get<1>(tuple);
        QString entityName = std::get<2>(tuple);
        QString attr = std::get<3>(tuple);
        double value = std::get<4>(tuple);
        QString text = desc.isEmpty() ? entityName : desc;
        m_drawDataList.append(qMakePair(text, getColorByDataPairWithCon(id, attr, value)));
    }
}

void PlotLight::initPlot()
{
    m_customPlot = new QCustomPlot();
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_customPlot);
    // top需要考虑标题栏的字体高度，先用60
    pLayout->setContentsMargins(m_leftPadding, 60, m_rightPadding, m_bottomPadding);
    setLayout(pLayout);
    // 将plot里面的坐标轴和中间的绘图矩形全部删除
    m_customPlot->plotLayout()->clear();
    m_customPlot->setBackground(m_outerFillColor);
}

void PlotLight::updatePlotByDrawData()
{
    clearPlotContent();
    calculateRaidus();
    // 需要手动计算灯和文本的坐标，实现网格布局
    // 状态灯左上角坐标
    int32_t lampX = m_innerPadding;
    int32_t lampY = 0;
    // 文本框左上角坐标
    int32_t textX = m_innerPadding + m_circleRadius * 2 + m_verPadding;
    int32_t textY = 0;

    int i = 0;
    for(const auto& pair : m_drawDataList)
    {
        lampY = m_innerPadding + i * (m_circleRadius * 2 + m_horPadding);
        QCPItemEllipse* statusLamp = new QCPItemEllipse(m_customPlot);
        statusLamp->setAntialiased(true);

        statusLamp->setPen(Qt::NoPen);
        statusLamp->setBrush(QBrush(pair.second));
        statusLamp->topLeft->setType(QCPItemPosition::ptAbsolute);
        statusLamp->topLeft->setCoords(lampX, lampY);
        statusLamp->bottomRight->setType(QCPItemPosition::ptAbsolute);
        statusLamp->bottomRight->setCoords(lampX + 2 * m_circleRadius, lampY + 2 * m_circleRadius);
        m_drawItemList.append(statusLamp);

        textY = lampY;
        QCPItemText* statusText = new QCPItemText(m_customPlot);
        statusText->setAntialiased(true);
        //字体大小
        statusText->setFont(QFont(font().family(), 16));
        // 外边框画笔
        statusText->setPen(QPen(Qt::NoPen));
        //字体颜色
        statusText->setColor(m_defaultTextColor);
        statusText->setText(pair.first);
        statusText->setVisible(true);
        statusText->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
        statusText->position->setType(QCPItemPosition::ptAbsolute);
        statusText->position->setCoords(textX, textY);
        m_drawItemList.append(statusText);
        ++i;
    }
    m_customPlot->replot();
}

void PlotLight::clearPlotContent()
{
    for(auto item : m_drawItemList)
    {
        m_customPlot->removeItem(item);
    }
    m_drawItemList.clear();
}

QColor PlotLight::getColorByDataPairWithCon(int32_t id, const QString& attr, double value)
{
    for(const auto& tuple : m_constraintList)
    {
        int32_t conId = std::get<0>(tuple);
        QString conAttr = std::get<1>(tuple);
        // 存在约束条件
        if(id == conId && attr == conAttr)
        {
            QString conCondition = std::get<2>(tuple);
            double threshold = std::get<3>(tuple);
            // 符合约束条件
            if((conCondition == "≥" && value > threshold) ||
               (conCondition == "<" && value < threshold))
            {
                QString colorName = std::get<4>(tuple);
                return QColor(colorName);
            }
        }
    }
    // 不存在约束条件返回默认颜色
    return m_defaultLightColor;
}

void PlotLight::updatePlotByCurrentData()
{
    if(!m_dataList.isEmpty())
    {
        processDataByConstraints();
        updatePlotByDrawData();
    }
}

void PlotLight::calculateRaidus()
{
    auto rect = m_customPlot->rect();
    m_circleRadius = (rect.height() - 2 * m_innerPadding - (m_dataList.size() - 1) * m_horPadding) /
                     m_dataList.size() / 2;
}
