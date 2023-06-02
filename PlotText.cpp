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
    m_dataList.clear();
    int isize = getDataPairs().size();
    for(int i = 0; i < isize; i++)
    {
        auto dataPair = getDataPairs().at(i);
        auto xEntityID = dataPair->getEntityIDX();
        auto xAttr = dataPair->getAttr_x();

        double value =
            DataManager::getInstance()->getEntityAttrValueByMaxTime(xEntityID, xAttr, secs);
        m_dataList.append(value);
    }
    update();
}

void PlotText::updateGraphByDataPair(DataPair* data)
{
    Q_UNUSED(data)
    // 暂时无法单独更新每个DataPair数据，只能全局刷新
    update();
}

void PlotText::customPainting(QPainter& painter)
{
    if(m_dataList.isEmpty())
    {
        return;
    }

    // 图表绘制区域
    drawCellData(painter);
}

void PlotText::drawCellData(QPainter& painter)
{
    QRect drawRect(0, 0, m_widget->width(), m_widget->height());
    int32_t itemSize = m_dataList.size();
    int32_t colSize = 2;
    // 没有插值只有两列，插值会出现三列，后续完善
    int horiGridWidth = drawRect.width() / colSize;
    int verGridWidth = drawRect.height() / (itemSize);
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

    // 绘制M*2的网格线
    pen.setColor(getGridColor());
    pen.setWidth(getGridWidth());
    pen.setStyle(getGridStyle());
    painter.setPen(pen);

    QRect cellRect;
    for(int i = 0; i < colSize; ++i)
    {
        int vx1 = drawRect.x() + horiGridWidth * (i + 1);
        int vy1 = drawRect.y();
        int vy2 = drawRect.bottom();
        // 垂直线
        painter.drawLine(vx1, vy1, vx1, vy2);
    }

    for(int j = 0; j < itemSize; ++j)
    {
        int hy1 = drawRect.y() + verGridWidth * (j + 1);
        int hx1 = drawRect.left();
        int hx2 = drawRect.right();
        // 水平线
        painter.drawLine(hx1, hy1, hx2, hy1);
    }

    auto dataPairList = getDataPairs();
    for(int j = 0; j < dataPairList.size(); ++j)
    {
        auto data = dataPairList.at(j);
        auto xAttr = data->getAttr_x();
        // 顺便画Attr Label，减少重复循环
        int hy1 = drawRect.y() + verGridWidth * j;
        int hx1 = drawRect.left();
        painter.save();
        pen.setColor(getTickLabelColor());
        painter.setPen(pen);
        QFont font = getTickLabelFont();
        font.setPixelSize(getTickLabelFontSize());
        painter.setFont(font);

        cellRect.setRect(hx1, hy1, horiGridWidth, verGridWidth);
        painter.drawText(cellRect, Qt::AlignCenter | Qt::TextWordWrap, xAttr);
        painter.restore();

        if(data->isDraw())
        {
            // 绘制X和Y轴的value
            pen.setColor(data->getLabelColor());
            pen.setWidth(3);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            QFont font = data->getLabelFont();
            font.setPointSize(data->getLabelFontSize());
            painter.setFont(font);

            int x = drawRect.x() + horiGridWidth;
            int y = drawRect.y() + verGridWidth * j;
            cellRect.setRect(x, y, horiGridWidth, verGridWidth);
            double value = m_dataList.at(j);
            QString text;
            if(math::doubleEqual(value, std::numeric_limits<double>::max()))
            {
                text = "无数据";
            }
            else
            {
                text = QString::number(value, 'f', data->getLabelPrecision_x());
                text += " " + data->getUnit_x();
            }
            auto alignFlag = data->processLabelTextPosition();
            painter.drawText(cellRect, alignFlag | Qt::TextWrapAnywhere, text);
        }
    }
}
