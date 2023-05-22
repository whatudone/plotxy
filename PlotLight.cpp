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
    setupLayout();
}

PlotLight::~PlotLight() {}

void PlotLight::onLightConstraintUpdate(
    const QList<std::tuple<int32_t, QString, QString, double, QString>>& constraintList)
{
    m_constraintList = constraintList;
    if(!m_dataHash.isEmpty())
    {
        processDataByConstraints();
    }
    int isize = getDataPairs().size();
    for(int i = 0; i < isize; i++)
    {
        auto dataPair = getDataPairs().at(i);
        updateGraphByDataPair(dataPair);
    }
}

void PlotLight::updateDataForDataPairsByTime(double secs)
{
    m_dataHash.clear();
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
        m_dataHash.insert(dataPair->getUuid(),
                          std::make_tuple(desc, xEntityID, xEntityName, xAttr, value));
    }
    if(!m_dataHash.isEmpty())
    {
        processDataByConstraints();
        clearPlotContent();
        calculateRaidus();
    }
    for(int i = 0; i < isize; i++)
    {
        auto dataPair = getDataPairs().at(i);
        updateGraphByDataPair(dataPair);
    }

    m_customPlot->replot();
}

void PlotLight::updateGraphByDataPair(DataPair* data)
{
    QCPItemEllipse* statusLamp;
    QCPItemText* statusText;
    QString uuid = data->getUuid();
    if(m_drawItemHash.contains(uuid))
    {
        auto draw = m_drawItemHash.value(uuid);
        statusLamp = draw.graph;
        statusText = draw.text;
    }
    else
    {
        DrawComponents draw;
        statusLamp = new QCPItemEllipse(m_customPlot);
        draw.graph = statusLamp;
        statusText = new QCPItemText(m_customPlot);
        draw.text = statusText;
        m_drawItemHash.insert(uuid, draw);
    }
    if(data->isDraw())
    {
        // 需要手动计算灯和文本的坐标，实现网格布局
        // 状态灯左上角坐标
        int32_t lampX = m_innerPadding;
        int32_t lampY = 0;
        // 文本框左上角坐标
        int32_t textX = m_innerPadding + m_circleRadius * 2 + m_verPadding;
        int32_t textY = 0;

        int index = getDataPairs().indexOf(data);
        auto drawDataPair = m_drawDataHash.value(uuid);
        lampY = m_innerPadding + index * (m_circleRadius * 2 + m_horPadding);
        statusLamp->setVisible(true);
        statusLamp->setAntialiased(true);

        statusLamp->setPen(Qt::NoPen);
        QColor lightColor;
        if(drawDataPair.second.isValid())
        {
            lightColor = drawDataPair.second;
        }
        else
        {
            // 约束条件没有返回有效颜色，采用默认数据颜色
            lightColor = data->dataColor();
        }
        statusLamp->setBrush(QBrush(lightColor));
        statusLamp->topLeft->setType(QCPItemPosition::ptAbsolute);
        statusLamp->topLeft->setCoords(lampX, lampY);
        statusLamp->bottomRight->setType(QCPItemPosition::ptAbsolute);
        statusLamp->bottomRight->setCoords(lampX + 2 * m_circleRadius, lampY + 2 * m_circleRadius);

        textY = lampY;

        statusText->setAntialiased(true);
        //字体大小
        statusText->setFont(QFont(font().family(), 16));
        // 外边框画笔
        statusText->setPen(QPen(Qt::NoPen));
        //字体颜色
        statusText->setColor(data->getLabelColor());
        statusText->setFont(data->getLabelFont());
        statusText->setText(drawDataPair.first);
        statusText->setVisible(true);
        statusText->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
        statusText->position->setType(QCPItemPosition::ptAbsolute);
        statusText->position->setCoords(textX, textY);
    }
    else
    {
        statusLamp->setVisible(false);
        statusText->setVisible(false);
    }
}

void PlotLight::delPlotPairData(const QString& uuid)
{
    if(m_drawDataHash.contains(uuid))
    {
        m_drawDataHash.remove(uuid);
    }
    if(m_drawItemHash.contains(uuid))
    {
        m_drawItemHash.remove(uuid);
    }
    PlotItemBase::delPlotPairData(uuid);
}

void PlotLight::processDataByConstraints()
{
    m_drawDataHash.clear();
    auto keys = m_dataHash.keys();
    for(const auto& uuid : keys)
    {
        auto tuple = m_dataHash.value(uuid);
        QString desc = std::get<0>(tuple);
        int32_t id = std::get<1>(tuple);
        QString entityName = std::get<2>(tuple);
        QString attr = std::get<3>(tuple);
        double value = std::get<4>(tuple);
        QString text = desc.isEmpty() ? entityName : desc;
        m_drawDataHash.insert(uuid, qMakePair(text, getColorByDataPairWithCon(id, attr, value)));
    }
}

void PlotLight::initPlot()
{
    m_customPlot = new QCustomPlot();
    // 将plot里面的坐标轴和中间的绘图矩形全部删除
    m_customPlot->plotLayout()->clear();
    m_customPlot->setBackground(m_outerFillColor);
}

void PlotLight::clearPlotContent()
{

    m_customPlot->clearItems();
    m_drawItemHash.clear();
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
    return QColor();
}

void PlotLight::calculateRaidus()
{
    auto rect = m_customPlot->rect();
    m_circleRadius = (rect.height() - 2 * m_innerPadding - (m_dataHash.size() - 1) * m_horPadding) /
                     m_dataHash.size() / 2;
}
