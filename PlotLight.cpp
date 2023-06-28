#include "PlotLight.h"
#include "DataManager.h"
#include "PlotItemBase.h"
#include "PlotXYDemo.h"
#include "Utils.h"

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
    m_lightType = "Circle";
    m_lightWidth = 40;
    m_lightHeight = 40;
    m_lightTextYPos = 0; //light的文字Y位置
    m_lightOutlineWidth = 0; //light灯的外边框线宽度
    m_lightOutlineColor = Qt::transparent; //light灯的外边框线颜色
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
    clearLineList();
    QRect drawRect(0, 0, m_customPlot->width(), m_customPlot->height());
    int32_t itemSize = m_drawDataHash.size() == 0 ? 1 : m_drawDataHash.size();

    //图表的宽比高
    double ratioWidthAndHeight = double(m_customPlot->width()) / m_customPlot->height();

    QCPItemEllipse* statusEllipseLamp;
    QCPItemRect* statusRectLamp;
    QCPItemText* statusText;
    QString uuid = data->getUuid();
    if(m_drawItemHash.contains(uuid))
    {
        auto draw = m_drawItemHash.value(uuid);
        statusEllipseLamp = draw.ellipseGraph;
        statusRectLamp = draw.rectGraph;
        statusText = draw.text;
    }
    else
    {
        DrawComponents draw;
        statusEllipseLamp = new QCPItemEllipse(m_customPlot);
        draw.ellipseGraph = statusEllipseLamp;
        statusRectLamp = new QCPItemRect(m_customPlot);
        draw.rectGraph = statusRectLamp;
        statusText = new QCPItemText(m_customPlot);
        draw.text = statusText;
        m_drawItemHash.insert(uuid, draw);
    }

    // 填充整体背景颜色
    m_customPlot->setBackground(m_gridFillColor);

    // 绘制内容区表格边框线
    QCPItemRect* m_outlineRect = new QCPItemRect(m_customPlot);
    m_outlineRect->setBrush(Qt::NoBrush);
    m_outlineRect->setPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
    m_outlineRect->topLeft->setType(QCPItemPosition::ptViewportRatio);
    m_outlineRect->bottomRight->setType(QCPItemPosition::ptViewportRatio);
    m_outlineRect->topLeft->setCoords(0, 0);
    // 要稍微往内部画一点，不然右边和底边的线显示不出来
    m_outlineRect->bottomRight->setCoords(0.99, 0.99);
    m_outRectList.append(m_outlineRect);

    if(getIsFillByRow())
    {
        int rowNum = getRowsNum();
        int colNum = qCeil(double(itemSize) / rowNum);

        int32_t colSize = 2 * colNum;
        double horGridWidth = double(drawRect.width()) / colSize;
        double verGridWidth = double(drawRect.height()) / rowNum;
        double perHorRatio = 1.0 / colSize / 2;
        double perVerRatio = 1.0 / rowNum / 2;
        double lampRatioWidth = double(m_lightWidth) / m_customPlot->width();
        double lampRatioHeight = double(m_lightHeight) / m_customPlot->height();

        // 绘制网格线
        if(getIsColGridVisible())
        {
            for(int i = 0; i < colSize; ++i)
            {
                QCPItemLine* m_verLine = new QCPItemLine(m_customPlot);
                m_verLine->start->setType(QCPItemPosition::ptViewportRatio);
                m_verLine->end->setType(QCPItemPosition::ptViewportRatio);
                m_verLine->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
                m_verLine->setVisible(true);
                double vx = (drawRect.x() + horGridWidth * (i + 1)) / m_customPlot->width();
                // 垂直线
                m_verLine->start->setCoords(vx, 0);
                m_verLine->end->setCoords(vx, 0.99);
                m_verLineList.append(m_verLine);
            }
        }
        else
        {
            for(int i = 0; i < m_verLineList.size(); i++)
                m_verLineList.at(i)->setVisible(false);
        }

        if(getIsRowGridVisible())
        {
            for(int i = 0; i < rowNum; ++i)
            {
                QCPItemLine* m_horLine = new QCPItemLine(m_customPlot);
                m_horLine->start->setType(QCPItemPosition::ptViewportRatio);
                m_horLine->end->setType(QCPItemPosition::ptViewportRatio);
                m_horLine->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
                m_horLine->setVisible(true);
                double vy = (drawRect.x() + verGridWidth * (i + 1)) / m_customPlot->height();
                // 水平线
                m_horLine->start->setCoords(0, vy);
                m_horLine->end->setCoords(0.99, vy);
                m_horLineList.append(m_horLine);
            }
        }
        else
        {
            for(int i = 0; i < m_horLineList.size(); i++)
                m_horLineList.at(i)->setVisible(false);
        }

        int index = getDataPairs().indexOf(data) + 1;
        int colIndex = (index % colNum == 0) ? colNum : index % colNum;
        int rowIndex = qCeil(double(index) / colNum);
        if(data->isDraw())
        {
            // 需要手动计算灯和文本的坐标，实现网格布局
            // 先确定灯的中心位置
            double lampCenterX =
                perHorRatio * ((colIndex - 1) * 4 + 1) +
                (double(m_textLeftOffset - m_textRightOffset) / m_customPlot->width());
            double lampCenterY = perVerRatio * ((rowIndex - 1) * 2 + 1);
            double textCenterX =
                perHorRatio * ((colIndex - 1) * 4 + 3) +
                (double(m_textLeftOffset - m_textRightOffset) / m_customPlot->width());
            double textCenterY = perVerRatio * ((rowIndex - 1) * 2 + 1) +
                                 double(m_lightTextYPos) / m_customPlot->height();

            auto drawDataPair = m_drawDataHash.value(uuid);
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
            QString type = getLightType();
            if(type == "Rectangle" || type == "Square")
            {
                statusEllipseLamp->setVisible(false);
                statusRectLamp->setVisible(true);
                statusRectLamp->setBrush(QBrush(lightColor));
                statusRectLamp->topLeft->setType(QCPItemPosition::ptViewportRatio);
                statusRectLamp->bottomRight->setType(QCPItemPosition::ptViewportRatio);
                if(type == "Rectangle")
                {
                    statusRectLamp->topLeft->setCoords(lampCenterX - lampRatioWidth / 2,
                                                       lampCenterY - lampRatioHeight / 2);
                    statusRectLamp->bottomRight->setCoords(lampCenterX + lampRatioWidth / 2,
                                                           lampCenterY + lampRatioHeight / 2);
                }
                else
                {
                    statusRectLamp->topLeft->setCoords(lampCenterX - lampRatioWidth / 2,
                                                       lampCenterY - lampRatioWidth / 2 *
                                                                         ratioWidthAndHeight);
                    statusRectLamp->bottomRight->setCoords(lampCenterX + lampRatioWidth / 2,
                                                           lampCenterY + lampRatioWidth / 2 *
                                                                             ratioWidthAndHeight);
                }
            }
            else if(type == "Ellipse" || type == "Circle")
            {
                statusEllipseLamp->setVisible(true);
                statusRectLamp->setVisible(false);
                statusEllipseLamp->setBrush(QBrush(lightColor));
                statusEllipseLamp->topLeft->setType(QCPItemPosition::ptViewportRatio);
                statusEllipseLamp->bottomRight->setType(QCPItemPosition::ptViewportRatio);
                if(type == "Ellipse")
                {
                    statusEllipseLamp->topLeft->setCoords(lampCenterX - lampRatioWidth / 2,
                                                          lampCenterY - lampRatioHeight / 2);
                    statusEllipseLamp->bottomRight->setCoords(lampCenterX + lampRatioWidth / 2,
                                                              lampCenterY + lampRatioHeight / 2);
                }
                else
                {
                    statusEllipseLamp->topLeft->setCoords(lampCenterX - lampRatioWidth / 2,
                                                          lampCenterY - lampRatioWidth / 2 *
                                                                            ratioWidthAndHeight);
                    statusEllipseLamp->bottomRight->setCoords(
                        lampCenterX + lampRatioWidth / 2,
                        lampCenterY + lampRatioWidth / 2 * ratioWidthAndHeight);
                }
            }
            statusEllipseLamp->setAntialiased(true);
            statusRectLamp->setAntialiased(true);

            QPen outlinePen;
            outlinePen.setColor(getLightOutlineColor());
            outlinePen.setWidth(getLightOutlineWidth());
            statusEllipseLamp->setPen(outlinePen);
            statusRectLamp->setPen(outlinePen);

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
            statusText->setPositionAlignment(Qt::AlignCenter);
            statusText->position->setType(QCPItemPosition::ptViewportRatio);
            statusText->position->setCoords(textCenterX, textCenterY);
        }
        else
        {
            statusEllipseLamp->setVisible(false);
            statusRectLamp->setVisible(false);
            statusText->setVisible(false);
        }
    }
    // 按列填充
    else
    {
        int colNum = getColsNum();
        int rowNum = qCeil(double(itemSize) / colNum);

        int32_t colSize = 2 * colNum;
        double horGridWidth = double(drawRect.width()) / colSize;
        double verGridWidth = double(drawRect.height()) / rowNum;
        // 便于计算，每个为框的一半
        double perHorRatio = 1.0 / colSize / 2;
        double perVerRatio = 1.0 / rowNum / 2;
        double lampRatioWidth = double(m_lightWidth) / m_customPlot->width();
        double lampRatioHeight = double(m_lightHeight) / m_customPlot->height();

        // 绘制网格线
        if(getIsColGridVisible())
        {
            for(int i = 0; i < colSize; ++i)
            {
                QCPItemLine* m_verLine = new QCPItemLine(m_customPlot);
                m_verLine->start->setType(QCPItemPosition::ptViewportRatio);
                m_verLine->end->setType(QCPItemPosition::ptViewportRatio);
                m_verLine->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
                m_verLine->setVisible(true);
                double vx = (drawRect.x() + horGridWidth * (i + 1)) / m_customPlot->width();
                // 垂直线
                m_verLine->start->setCoords(vx, 0);
                m_verLine->end->setCoords(vx, 0.99);
                m_verLineList.append(m_verLine);
            }
        }
        else
        {
            for(int i = 0; i < m_verLineList.size(); i++)
                m_verLineList.at(i)->setVisible(false);
        }

        if(getIsRowGridVisible())
        {
            for(int i = 0; i < rowNum; ++i)
            {
                QCPItemLine* m_horLine = new QCPItemLine(m_customPlot);
                m_horLine->start->setType(QCPItemPosition::ptViewportRatio);
                m_horLine->end->setType(QCPItemPosition::ptViewportRatio);
                m_horLine->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
                m_horLine->setVisible(true);
                double vy = (drawRect.x() + verGridWidth * (i + 1)) / m_customPlot->height();
                // 水平线
                m_horLine->start->setCoords(0, vy);
                m_horLine->end->setCoords(0.99, vy);
                m_horLineList.append(m_horLine);
            }
        }
        else
        {
            for(int i = 0; i < m_horLineList.size(); i++)
                m_horLineList.at(i)->setVisible(false);
        }

        int index = getDataPairs().indexOf(data) + 1;
        int rowIndex = (index % rowNum == 0) ? rowNum : index % rowNum;
        int colIndex = qCeil(double(index) / rowNum);
        if(data->isDraw())
        {
            // 需要手动计算灯和文本的坐标，实现网格布局
            double lampCenterX =
                perHorRatio * ((colIndex - 1) * 4 + 1) +
                (double(m_textLeftOffset - m_textRightOffset) / m_customPlot->width());
            double lampCenterY = perVerRatio * ((rowIndex - 1) * 2 + 1);
            double textCenterX =
                perHorRatio * ((colIndex - 1) * 4 + 3) +
                (double(m_textLeftOffset - m_textRightOffset) / m_customPlot->width());
            double textCenterY = perVerRatio * ((rowIndex - 1) * 2 + 1) +
                                 double(m_lightTextYPos) / m_customPlot->height();

            auto drawDataPair = m_drawDataHash.value(uuid);
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
            QString type = getLightType();
            if(type == "Rectangle" || type == "Square")
            {
                statusEllipseLamp->setVisible(false);
                statusRectLamp->setVisible(true);
                statusRectLamp->setBrush(QBrush(lightColor));
                statusRectLamp->topLeft->setType(QCPItemPosition::ptViewportRatio);
                statusRectLamp->bottomRight->setType(QCPItemPosition::ptViewportRatio);
                if(type == "Rectangle")
                {
                    statusRectLamp->topLeft->setCoords(lampCenterX - lampRatioWidth / 2,
                                                       lampCenterY - lampRatioHeight / 2);
                    statusRectLamp->bottomRight->setCoords(lampCenterX + lampRatioWidth / 2,
                                                           lampCenterY + lampRatioHeight / 2);
                }
                else
                {
                    statusRectLamp->topLeft->setCoords(lampCenterX - lampRatioWidth / 2,
                                                       lampCenterY - lampRatioWidth / 2 *
                                                                         ratioWidthAndHeight);
                    statusRectLamp->bottomRight->setCoords(lampCenterX + lampRatioWidth / 2,
                                                           lampCenterY + lampRatioWidth / 2 *
                                                                             ratioWidthAndHeight);
                }
            }
            else if(type == "Ellipse" || type == "Circle")
            {
                statusEllipseLamp->setVisible(true);
                statusRectLamp->setVisible(false);
                statusEllipseLamp->setBrush(QBrush(lightColor));
                statusEllipseLamp->topLeft->setType(QCPItemPosition::ptViewportRatio);
                statusEllipseLamp->bottomRight->setType(QCPItemPosition::ptViewportRatio);
                if(type == "Ellipse")
                {
                    statusEllipseLamp->topLeft->setCoords(lampCenterX - lampRatioWidth / 2,
                                                          lampCenterY - lampRatioHeight / 2);
                    statusEllipseLamp->bottomRight->setCoords(lampCenterX + lampRatioWidth / 2,
                                                              lampCenterY + lampRatioHeight / 2);
                }
                else
                {
                    statusEllipseLamp->topLeft->setCoords(lampCenterX - lampRatioWidth / 2,
                                                          lampCenterY - lampRatioWidth / 2 *
                                                                            ratioWidthAndHeight);
                    statusEllipseLamp->bottomRight->setCoords(
                        lampCenterX + lampRatioWidth / 2,
                        lampCenterY + lampRatioWidth / 2 * ratioWidthAndHeight);
                }
            }
            statusEllipseLamp->setAntialiased(true);
            statusRectLamp->setAntialiased(true);

            QPen outlinePen;
            outlinePen.setColor(getLightOutlineColor());
            outlinePen.setWidth(getLightOutlineWidth());
            statusEllipseLamp->setPen(outlinePen);
            statusRectLamp->setPen(outlinePen);

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
            statusText->setPositionAlignment(Qt::AlignCenter);
            statusText->position->setType(QCPItemPosition::ptViewportRatio);
            statusText->position->setCoords(textCenterX, textCenterY);
        }
        else
        {
            statusEllipseLamp->setVisible(false);
            statusRectLamp->setVisible(false);
            statusText->setVisible(false);
        }
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
        if(m_drawItemHash[uuid].text)
        {
            delete m_drawItemHash[uuid].text;
            m_drawItemHash[uuid].text = nullptr;
        }
        if(m_drawItemHash[uuid].ellipseGraph)
        {
            delete m_drawItemHash[uuid].ellipseGraph;
            m_drawItemHash[uuid].ellipseGraph = nullptr;
        }
        if(m_drawItemHash[uuid].rectGraph)
        {
            delete m_drawItemHash[uuid].rectGraph;
            m_drawItemHash[uuid].rectGraph = nullptr;
        }
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
    m_customPlot->setBackground(m_gridFillColor);
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
                return color_transfer::QColorFromRGBAStr(colorName);
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

void PlotLight::clearLineList()
{
    for(int i = 0; i < m_horLineList.size(); i++)
    {
        m_customPlot->removeItem(m_horLineList.at(i));
    }

    for(int i = 0; i < m_verLineList.size(); i++)
    {
        m_customPlot->removeItem(m_verLineList.at(i));
    }

    for(int i = 0; i < m_outRectList.size(); i++)
    {
        m_customPlot->removeItem(m_outRectList.at(i));
    }

    m_horLineList.clear();
    m_verLineList.clear();
    m_outRectList.clear();
}

QColor PlotLight::getLightOutlineColor() const
{
    return m_lightOutlineColor;
}

void PlotLight::setLightOutlineColor(const QColor& lightOutlineColor)
{
    m_lightOutlineColor = lightOutlineColor;
    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}

int PlotLight::getLightOutlineWidth() const
{
    return m_lightOutlineWidth;
}

void PlotLight::setLightOutlineWidth(int lightOutlineWidth)
{
    m_lightOutlineWidth = lightOutlineWidth;
    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}

int PlotLight::getLightTextYPos() const
{
    return m_lightTextYPos;
}

void PlotLight::setLightTextYPos(int lightTextYPos)
{
    m_lightTextYPos = lightTextYPos;
    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}

int PlotLight::getLightHeight() const
{
    return m_lightHeight;
}

void PlotLight::setLightHeight(int lightHeight)
{
    m_lightHeight = lightHeight;
    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}

int PlotLight::getLightWidth() const
{
    return m_lightWidth;
}

void PlotLight::setLightWidth(int lightWidth)
{
    m_lightWidth = lightWidth;
    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}

QString PlotLight::getLightType() const
{
    return m_lightType;
}

void PlotLight::setLightType(const QString& lightType)
{
    m_lightType = lightType;
    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}
