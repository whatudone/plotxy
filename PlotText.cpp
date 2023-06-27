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
    // 图表绘制区域
    drawCellData(painter);
}

void PlotText::drawCellData(QPainter& painter)
{
    QRect drawRect(0, 0, m_widget->width(), m_widget->height());
    int32_t itemSize = m_dataList.size() == 0 ? 1 : m_dataList.size();

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

    if(getIsFillByRow())
    {
        int rowNum = getRowsNum();
        int colNum = qCeil(double(itemSize) / rowNum);

        int32_t colSize = 2 * colNum;
        int horiGridWidth = drawRect.width() / colSize;
        int verGridWidth = drawRect.height() / rowNum;

        QRect cellRect;
        if(getIsColGridVisible())
        {
            for(int i = 0; i < colSize; ++i)
            {
                int vx1 = drawRect.x() + horiGridWidth * (i + 1);
                int vy1 = drawRect.y();
                int vy2 = drawRect.bottom();
                // 垂直线
                painter.drawLine(vx1, vy1, vx1, vy2);
            }
        }

        if(getIsRowGridVisible())
        {
            for(int j = 0; j < rowNum; ++j)
            {
                int hy1 = drawRect.y() + verGridWidth * (j + 1);
                int hx1 = drawRect.left();
                int hx2 = drawRect.right();
                // 水平线
                painter.drawLine(hx1, hy1, hx2, hy1);
            }
        }

        auto dataPairList = getDataPairs();
        for(int rowIndex = 0; rowIndex < rowNum; rowIndex++)
        {
            for(int colIndex = 0; colIndex < colNum; colIndex++)
            {
                int curIndex = colIndex + colNum * rowIndex;
                if(curIndex >= dataPairList.size())
                    break;

                auto data = dataPairList.at(curIndex);
                auto xAttr = data->getAttr_x();
                auto entity = data->getEntity_x();

                if(data->isDraw())
                {
                    // 顺便画Attr Label，减少重复循环
                    int offset = getTextLeftOffset() - getTextRightOffset();
                    int hx1 = drawRect.left() + horiGridWidth * colIndex * 2 + offset;
                    int hy1 = drawRect.y() + verGridWidth * rowIndex;
                    painter.save();
                    pen.setColor(getxTickLabelColor());
                    painter.setPen(pen);
                    QFont tickFont = getxTickLabelFont();
                    tickFont.setPixelSize(getxTickLabelFontSize());
                    painter.setFont(tickFont);

                    cellRect.setRect(hx1, hy1, horiGridWidth, verGridWidth);
                    auto alignFlag = data->getLabelTextAlign();
                    painter.drawText(cellRect, alignFlag | Qt::TextWordWrap, entity + " " + xAttr);
                    painter.restore();

                    // 绘制X和Y轴的value
                    if(data->matchColor())
                    {
                        pen.setColor(data->dataColor());
                    }
                    else
                    {
                        pen.setColor(data->getLabelColor());
                    }
                    pen.setWidth(3);
                    pen.setStyle(Qt::SolidLine);
                    painter.setPen(pen);
                    QFont font = data->getLabelFont();
                    font.setPointSize(data->getLabelFontSize());
                    painter.setFont(font);

                    int x = drawRect.x() + horiGridWidth * (colIndex * 2 + 1) + offset;
                    int y = drawRect.y() + verGridWidth * rowIndex;
                    cellRect.setRect(x, y, horiGridWidth, verGridWidth);
                    double value = m_dataList.at(curIndex);
                    QString text;
                    if(math::doubleEqual(value, std::numeric_limits<double>::max()))
                    {
                        text = "---";
                    }
                    else
                    {
                        text = QString::number(value, 'f', data->getLabelPrecision_x());
                        text += " " + data->getUnit_x();
                    }
                    painter.drawText(cellRect, alignFlag | Qt::TextWordWrap, text);
                }
            }
        }
    }
    else
    {
        // fill by col
        int colNum = getColsNum();
        int rowNum = qCeil(double(itemSize) / colNum);

        int32_t colSize = 2 * colNum;
        int horiGridWidth = drawRect.width() / colSize;
        int verGridWidth = drawRect.height() / rowNum;

        QRect cellRect;
        if(getIsColGridVisible())
        {
            for(int i = 0; i < colSize; ++i)
            {
                int vx1 = drawRect.x() + horiGridWidth * (i + 1);
                int vy1 = drawRect.y();
                int vy2 = drawRect.bottom();
                // 垂直线
                painter.drawLine(vx1, vy1, vx1, vy2);
            }
        }

        if(getIsRowGridVisible())
        {
            for(int j = 0; j < rowNum; ++j)
            {
                int hy1 = drawRect.y() + verGridWidth * (j + 1);
                int hx1 = drawRect.left();
                int hx2 = drawRect.right();
                // 水平线
                painter.drawLine(hx1, hy1, hx2, hy1);
            }
        }

        auto dataPairList = getDataPairs();
        for(int colIndex = 0; colIndex < colNum; colIndex++)
        {
            for(int rowIndex = 0; rowIndex < rowNum; rowIndex++)
            {
                int curIndex = rowIndex + rowNum * colIndex;
                if(curIndex >= dataPairList.size())
                    break;

                auto data = dataPairList.at(curIndex);
                auto xAttr = data->getAttr_x();
                auto entity = data->getEntity_x();

                if(data->isDraw())
                {
                    // 顺便画Attr Label，减少重复循环
                    int offset = getTextLeftOffset() - getTextRightOffset();
                    int hx1 = drawRect.left() + horiGridWidth * colIndex * 2 + offset;
                    int hy1 = drawRect.y() + verGridWidth * rowIndex;
                    painter.save();
                    pen.setColor(getxTickLabelColor());
                    painter.setPen(pen);
                    QFont tickFont = getxTickLabelFont();
                    tickFont.setPixelSize(getxTickLabelFontSize());
                    painter.setFont(tickFont);

                    cellRect.setRect(hx1, hy1, horiGridWidth, verGridWidth);
                    auto alignFlag = data->getLabelTextAlign();
                    painter.drawText(cellRect, alignFlag | Qt::TextWordWrap, entity + " " + xAttr);
                    painter.restore();

                    // 绘制X和Y轴的value
                    if(data->matchColor())
                    {
                        pen.setColor(data->dataColor());
                    }
                    else
                    {
                        pen.setColor(data->getLabelColor());
                    }
                    pen.setWidth(3);
                    pen.setStyle(Qt::SolidLine);
                    painter.setPen(pen);
                    QFont font = data->getLabelFont();
                    font.setPointSize(data->getLabelFontSize());
                    painter.setFont(font);

                    int x = drawRect.x() + horiGridWidth * (colIndex * 2 + 1) + offset;
                    int y = drawRect.y() + verGridWidth * rowIndex;
                    cellRect.setRect(x, y, horiGridWidth, verGridWidth);
                    double value = m_dataList.at(curIndex);
                    QString text;
                    if(math::doubleEqual(value, std::numeric_limits<double>::max()))
                    {
                        text = "---";
                    }
                    else
                    {
                        text = QString::number(value, 'f', data->getLabelPrecision_x());
                        text += " " + data->getUnit_x();
                    }
                    painter.drawText(cellRect, alignFlag | Qt::TextWordWrap, text);
                }
            }
        }
    }
}
