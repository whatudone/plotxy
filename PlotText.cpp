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
    m_textLeftOffset = 0;
    m_textRightOffset = 0;
    m_rowsNum = 1;
    m_colsNum = 1;

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

        auto xDataType = dataPair->getXDataType();
        double value = 0.0;
        if(xDataType == DataPair::Time)
        {
            value = secs;
        }
        else if(xDataType == DataPair::Parameter)
        {
            value =
                DataManagerInstance->getEntityAttrValueByMaxTime(xEntityID, xAttr, secs, m_xRate);
        }
        else
        {
            auto xTargetEntityID = dataPair->getTargetEntityIDX();
            auto xCalType = dataPair->getXCalType();
            value = DataManagerInstance->rangeCalculationLastValue(
                xEntityID, xTargetEntityID, xCalType, secs, m_xRate);
        }
        m_dataList.append(value);
    }
    update();
}

void PlotText::updateGraphByDataPair(DataPair* data, double curSecs)
{
    Q_UNUSED(data)
    Q_UNUSED(curSecs)
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

    if(m_isFillByRow)
    {
        int rowNum = m_rowsNum;
        int colNum = qCeil(double(itemSize) / rowNum);

        int32_t colSize = 2 * colNum;
        int horiGridWidth = drawRect.width() / colSize;
        int verGridWidth = drawRect.height() / rowNum;

        QRect cellRect;
        if(m_isColGridVisible)
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

        if(m_isRowGridVisible)
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
                    int offset = m_textLeftOffset - m_textRightOffset;
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
        int colNum = m_colsNum;
        int rowNum = qCeil(double(itemSize) / colNum);

        int32_t colSize = 2 * colNum;
        int horiGridWidth = drawRect.width() / colSize;
        int verGridWidth = drawRect.height() / rowNum;

        QRect cellRect;
        if(m_isColGridVisible)
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

        if(m_isRowGridVisible)
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

                if(data->isDraw())
                {
                    // 绘制第一列属性
                    QString desc;
                    auto xDataType = data->getXDataType();
                    if(xDataType == DataPair::Time)
                    {
                        desc = "Time";
                    }
                    else
                    {
                        desc = data->getXEntityAttrPair();
                    }
                    int offset = m_textLeftOffset - m_textRightOffset;
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
                    painter.drawText(cellRect, alignFlag | Qt::TextWordWrap, desc);
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
                    if(math::doubleEqual(value, std::numeric_limits<double>::max()) ||
                       math::doubleEqual(value, std::numeric_limits<double>::min()))
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

int PlotText::colsNum() const
{
    return m_colsNum;
}

void PlotText::setColsNum(int colsNum)
{
    m_colsNum = colsNum;
    update();
}

int PlotText::rowsNum() const
{
    return m_rowsNum;
}

void PlotText::setRowsNum(int rowsNum)
{
    m_rowsNum = rowsNum;
    update();
}

bool PlotText::isFillByRow() const
{
    return m_isFillByRow;
}

void PlotText::setIsFillByRow(bool isFillByRow)
{
    m_isFillByRow = isFillByRow;
    update();
}

int PlotText::textRightOffset() const
{
    return m_textRightOffset;
}

void PlotText::setTextRightOffset(int textRightOffset)
{
    m_textRightOffset = textRightOffset;
    update();
}

int PlotText::textLeftOffset() const
{
    return m_textLeftOffset;
}

void PlotText::setTextLeftOffset(int textLeftOffset)
{
    m_textLeftOffset = textLeftOffset;
    update();
}

bool PlotText::isColGridVisible() const
{
    return m_isColGridVisible;
}

void PlotText::setIsColGridVisible(bool isColGridVisible)
{
    m_isColGridVisible = isColGridVisible;
    update();
}

bool PlotText::isRowGridVisible() const
{
    return m_isRowGridVisible;
}

void PlotText::setIsRowGridVisible(bool isRowGridVisible)
{
    m_isRowGridVisible = isRowGridVisible;
    update();
}
