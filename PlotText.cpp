#include "PlotText.h"
#include "DataManager.h"

#include "Utils.h"
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
    m_title = "Text";

	QString name = QString("Text%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

    m_widget = new QWidget;
    setupLayout();
}

PlotText::~PlotText() {}

void PlotText::updateDataForDataPairsByTime(double secs)
{
    m_entityList.clear();
    m_attrList.clear();
    m_dataHash.clear();
    QVector<int32_t> entityIDList;
    int isize = getDataPairs().size();
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
        double value =
            DataManager::getInstance()->getEntityAttrValueByMaxTime(xEntityID, xAttr, secs);
        m_dataHash.insert(qMakePair(xEntityName, xAttr), value);
    }
    update();
}

void PlotText::updateGraphByDataPair(DataPair* data)
{

    // 暂时无法单独更新每个DataPair数据，只能全局刷新
    update();
}

void PlotText::customPainting(QPainter& painter)
{
    if(m_entityList.isEmpty() || m_attrList.isEmpty())
    {
        return;
    }

    // 图表绘制区域

    drawCellData(painter);
}

void PlotText::drawCellData(QPainter& painter)
{
    QRect drawRect(0, 0, m_widget->width(), m_widget->height());
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
        font.setPixelSize(getTickLabelFontSize());
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
        painter.setPen(pen);
        QFont font = getTickLabelFont();
        font.setPixelSize(getTickLabelFontSize());
        painter.setFont(font);

        cellRect.setRect(hx1, hy1, horiGridWidth, verGridWidth);
        painter.drawText(cellRect, Qt::AlignCenter | Qt::TextWordWrap, m_attrList.at(j));
        painter.restore();
    }
    // 绘制X和Y轴的value

    auto dataPairList = getDataPairs();
    for(DataPair* data : dataPairList)
    {
        if(data->isDraw())
        {

            pen.setColor(data->getLabelColor());
            pen.setWidth(3);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            QFont font = data->getLabelFont();
            font.setPointSize(data->getLabelFontSize());
            painter.setFont(font);

            auto xEntityName = data->getEntity_x();
            auto xAttr = data->getAttr_x();
            int32_t entityIndex = m_entityList.indexOf(xEntityName);
            int32_t attrIndex = m_attrList.indexOf(xAttr);
            int x = drawRect.x() + horiGridWidth * (entityIndex + 1);
            int y = drawRect.y() + verGridWidth * (attrIndex + 1);
            cellRect.setRect(x, y, horiGridWidth, verGridWidth);
            double value = m_dataHash.value(qMakePair(xEntityName, xAttr));
            QString text;
            if(math::doubleEqual(value, std::numeric_limits<double>::max()))
            {
                text = "无数据";
            }
            else
            {
                text = QString::number(value, 'f', data->getLabelPrecision_x());
            }
            auto alignFlag = data->processLabelTextPosition();
            painter.drawText(cellRect, alignFlag | Qt::TextWrapAnywhere, text);
        }
    }
}
