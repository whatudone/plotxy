#include "PlotText.h"
#include "AddPlotPair.h"
#include "DataManager.h"
#include "PlotItemBase.h"
#include "PlotManager.h"
#include "colorbutton.h"
#include "constdef.h"
#include <QDebug>
#include <QFont>
#include <QList>
#include <QPainter>
#include <QPen>
#include <QStringList>

int PlotText::m_instanceCount = 1;
PlotText::PlotText(QWidget* parent)
    : PlotItemBase(parent)
{
	m_bHorizontal = true;
    m_title = "Text";

	m_interPadding = 20;

	m_gridWidth = 1;
	m_gridColor = Qt::white;

	m_axisColor = Qt::white;
	m_axisWidth = 1;

	QString name = QString("Text%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;
}

PlotText::~PlotText() {}

void PlotText::updateDataForDataPairsByTime(double secs)
{
    int isize = getDataPairs().size();
    m_dataVec.clear();
    m_entityList.clear();
    QVector<int32_t> entityIDList;
    m_attrList.clear();
    for(int i = 0; i < isize; i++)
    {
        auto dataPair = getDataPairs().at(i);
        auto xEntityID = dataPair->getEntityIDX();
        auto xEntityName = dataPair->getEntity_x();
        auto xAttr = dataPair->getAttr_x();

        if(!entityIDList.contains(xEntityID))
        {
            entityIDList.append(xEntityID);
            m_entityList.append(xEntityName);
        }
        if(!m_attrList.contains(xAttr))
        {
            m_attrList.append(xAttr);
        }
    }
    // 两层循环获取所有entity-attr对应的属性值
    for(int i = 0; i < entityIDList.size(); ++i)
    {
        QVector<double> attrValueList;
        for(int j = 0; j < m_attrList.size(); ++j)
        {
            double value = DataManager::getInstance()->getEntityAttrValueByMaxTime(
                entityIDList.at(i), m_attrList.at(j), secs);
            attrValueList.append(value);
        }
        m_dataVec.append(attrValueList);
    }
}

void PlotText::customPainting(QPainter& painter)
{
    if(m_entityList.isEmpty() || m_attrList.isEmpty())
    {
        return;
    }

    QFont titleFont;
    titleFont.setPointSize(getTitleFontSize());
    QFontMetricsF fm(titleFont);
    double titleAs = fm.ascent();
    double titleHeight = fm.height();
    // 图表绘制区域
    QRect plotRect(m_leftPadding,
                   m_topPadding + titleHeight + titleAs,
                   width() - m_leftPadding - m_rightPadding,
                   height() - m_topPadding - m_bottomPadding - titleHeight - titleAs);
    drawCellData(painter, plotRect);
}

void PlotText::drawCellData(QPainter& painter, const QRect& drawRect)
{
    int entitySize = m_entityList.size();
    int attrSize = m_attrList.size();
    int horiGridWidth = drawRect.width() / (entitySize + 1);
    int verGridWidth = drawRect.height() / (attrSize + 1);
    // 填充整体背景颜色
    QBrush brush;
    brush.setColor(getGridFillColor());
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.fillRect(drawRect, brush);
    // 绘制外层矩形边框
    QPen pen;
    pen.setColor(getAxisColor());
    pen.setWidth(getAxisWidth());
    painter.setPen(pen);
    painter.drawRect(drawRect);

    // 绘制M*N的网格线
    pen.setColor(getGridColor());
    pen.setWidth(getGridWidth());
    pen.setStyle(getGridStyle());
    painter.setPen(pen);

    QRect cellRect;
    for(int i = 0; i < entitySize; ++i)
    {
        int vx1 = drawRect.x() + horiGridWidth * (i + 1);
        int vy1 = drawRect.y();
        int vy2 = drawRect.bottom();
        // 垂直线
        painter.drawLine(vx1, vy1, vx1, vy2);
        painter.save();
        // 顺便画Entity Label，减少重复循环
        pen.setColor(getTickLabelColor());
        QFont font = getTickLabelFont();
        font.setPointSize(getTickLabelFontSize());
        painter.setFont(font);
        painter.setPen(pen);
        cellRect.setRect(vx1, vy1, horiGridWidth, verGridWidth);
        painter.drawText(cellRect, Qt::AlignCenter | Qt::TextWordWrap, m_entityList.at(i));
        painter.restore();
    }

    for(int j = 0; j < attrSize; ++j)
    {
        int hy1 = drawRect.y() + verGridWidth * (j + 1);
        int hx1 = drawRect.left();
        int hx2 = drawRect.right();
        // 水平线
        painter.drawLine(hx1, hy1, hx2, hy1);
        painter.save();
        // 顺便画Attr Label，减少重复循环
        pen.setColor(getTickLabelColor());
        QFont font = getTickLabelFont();
        font.setPointSize(getTickLabelFontSize());
        painter.setFont(font);
        painter.setPen(pen);

        cellRect.setRect(hx1, hy1, horiGridWidth, verGridWidth);
        painter.drawText(cellRect, Qt::AlignCenter | Qt::TextWordWrap, m_attrList.at(j));
        painter.restore();
    }
    // 绘制X和Y轴的value
    m_cellValueColor = QColor(Qt::white);
    pen.setColor(m_cellValueColor);
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    for(int i = 0; i < entitySize; ++i)
    {
        int x = drawRect.x() + horiGridWidth * (i + 1);
        for(int j = 0; j < attrSize; ++j)
        {
            int y = drawRect.y() + verGridWidth * (j + 1);
            cellRect.setRect(x, y, horiGridWidth, verGridWidth);
            double value = m_dataVec.at(i).at(j);
            painter.drawText(
                cellRect, Qt::AlignCenter | Qt::TextWrapAnywhere, QString::number(value, 'f', 6));
        }
    }
}
