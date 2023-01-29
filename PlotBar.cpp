#include "PlotBar.h"
#include <QDebug>
#include <QPainter>
#include "DataManager.h"
int PlotBar::m_instanceCount = 1;
PlotBar::PlotBar(QWidget *parent)
    : PlotItemBase(parent)
{
    m_bHorizontal = true;
	m_titleShow = true;

    m_leftPadding = 50;
    m_rightPadding = 50;
	m_topPadding = 0;
	m_bottomPadding = 50;
    m_interPadding = 20;

    m_currTimeIndex = 0;

    m_horiGridNum = 4;
    m_verGridNum = 5;

    QString name = QString("Bar%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_defaultColor = Qt::gray;
    m_gridColor = Qt::darkGray;
	m_axisColor = Qt::white;
	m_titleColor = Qt::white;
	m_title = "Bar";
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(16.0);
}

PlotBar::~PlotBar()
{

}

void PlotBar::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>> targetMap)
{
    m_thresholdColorMap = targetMap;
}

void PlotBar::onGetCurrentSecond(double secs)
{
    if (getDataPair().isEmpty())
        return;

    m_seconds = secs;
}

void PlotBar::getDataInfo(double secs)
{
	if (getDataPair().isEmpty())
		return;

    int isize = getDataPair().size();

    for (int i = 0; i < isize; i++) {
        QString xColumn = getDataPair().at(i)->getDataPair().first;
        QString yColumn = getDataPair().at(i)->getDataPair().first;
        updateData(i, xColumn, yColumn, secs);
    }
}

void PlotBar::setPlotItemExtentionDirec(bool horizontal)
{
    m_bHorizontal = horizontal;
}

void PlotBar::setLeftPadding(int leftPadding)
{
    m_leftPadding = leftPadding;
}

void PlotBar::setRightPadding(int rightPadding)
{
    m_rightPadding = rightPadding;
}

void PlotBar::setInterPadding(int interPadding)
{
    m_interPadding = interPadding;
}

void PlotBar::drawRect(int itemIndex, bool bHorizontal, int itemLength, int leftBoundary, int rightBoundary, QColor color)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(color);
    QBrush brush;
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);

    painter.setPen(pen);
    painter.setBrush(brush);

    int height = this->height();
    int width = this->width();

    QRect rect;
    if (bHorizontal) {
        rect.setRect(leftBoundary, height - (m_leftPadding + itemIndex * (itemLength + m_interPadding) + itemLength), rightBoundary - leftBoundary, itemLength);
    } else {
        rect.setRect(m_leftPadding + itemIndex * (itemLength + m_interPadding), height - rightBoundary, itemLength, rightBoundary - leftBoundary);
    }
    painter.drawRect(rect);
}

void PlotBar::paintEvent(QPaintEvent *event)
{
	int width = this->width();
	int height = this->height();
    
    QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QPen pen;
	//���Ʊ���
	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h = fm.size(Qt::TextSingleLine, m_title).height();
	double as = fm.ascent();
	if (m_titleShow)
	{
		painter.setFont(m_titleFont);
		painter.setPen(m_titleColor);
		painter.drawText(QPoint((width + m_leftPadding - m_rightPadding - w) / 2, as + m_topPadding), m_title);
	}

	//����x���y��
    pen.setColor(m_axisColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(QPoint(m_leftPadding, height - m_bottomPadding), QPoint(width - m_rightPadding, height - m_bottomPadding));   //x��
    painter.drawLine(QPoint(m_leftPadding, height - m_bottomPadding), QPoint(m_leftPadding, m_topPadding + h));    //y��

    //��������
    pen.setColor(m_gridColor);
    painter.setPen(pen);

    QBrush brush;   //��ˢ����伸��ͼ�εĵ�ɫ�壬����ɫ����������
    brush.setColor(m_gridFillColor);
    brush.setStyle(Qt::SolidPattern);
    //painter.setBrush(brush);

    int horiGridWidth = 0;
    if (m_horiGridNum) {    //itemˮƽ������չ
        horiGridWidth = (width - m_leftPadding - m_rightPadding) / m_horiGridNum;
    }

    for (int i = 0; i < m_horiGridNum; i++) {
        QRect gridRect;
        gridRect.setRect(i * horiGridWidth + m_leftPadding, m_topPadding + h, horiGridWidth, height - m_topPadding - m_bottomPadding - h);
        painter.drawRect(gridRect);
    }

    int verGridWidth = 0;
    if (m_verGridNum) {     //itemˮƽ������չ
        verGridWidth = (height - m_topPadding - m_bottomPadding - h) / m_verGridNum;
    }

    for (int i = 0; i < m_verGridNum; i++) {
        QRect gridRect;
        gridRect.setRect(m_leftPadding, i * verGridWidth + m_topPadding + h, width - m_leftPadding - m_rightPadding, verGridWidth);
        painter.drawRect(gridRect);
    }
    getDataInfo(m_seconds);

    return PlotItemBase::paintEvent(event);
}

void PlotBar::updateData(int itemIndex, QString x, QString y, double secs)
{
    QStringList xlist = x.split("+");
//    QStringList ylist = y.split("+");
    QList<double> xSecList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs);
//    QList<double> ySecList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs);
    int cnt = getDataPair().size();

    if (xSecList.isEmpty())
        return;

    //���ȼ���ÿ��item�Ŀ��/�߶�
	QFontMetricsF fm(m_titleFont);
	double h = fm.size(Qt::TextSingleLine, m_title).height();
    int perItemLength = 0;
    int width = this->width() - m_leftPadding - m_rightPadding;
    int height = this->height() - m_topPadding - m_bottomPadding - h;
	
    if (m_bHorizontal) 
	{    
		//itemˮƽ������չ
		m_horiGridNum = 5;
		m_verGridNum = 0;
        perItemLength = (height - (cnt - 1) * m_interPadding) / cnt;
    } 
	else 
	{   
		//item��ֱ������չ
		m_verGridNum = 5;
		m_horiGridNum = 0;
        perItemLength = (width - (cnt - 1) * m_interPadding) / cnt;
    }

    //*��ȡ��ǰAttrֵ
    double currValue = xSecList.last();

    QString currKey = xlist.at(0) + '_' + xlist.at(1);
    if (!m_thresholdColorMap.contains(currKey) || m_thresholdColorMap.value(currKey).keys().isEmpty()) {
        //û��������ֵ��������ֿ����ƶ�����Σ���Ĭ����ɫ����һ������
        drawRect(itemIndex, m_bHorizontal, perItemLength, m_leftPadding, currValue, m_defaultColor);
        update();
        return;
    }

    //������ɫ��ֵ���ֿ�����
    auto colorMap = m_thresholdColorMap.value(currKey);
    QList<int> thresholdList = colorMap.keys();
    if (currValue < thresholdList.first()) {
        drawRect(itemIndex, m_bHorizontal, perItemLength, m_leftPadding, currValue, m_defaultColor);
    }

    //��Ĭ����ɫ���Ƶ�һ������
    drawRect(itemIndex, m_bHorizontal, perItemLength, m_leftPadding, thresholdList.first(), m_defaultColor);

    if (thresholdList.size() == 1) {
        drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.first(), currValue, colorMap.value(thresholdList.first()));
    }

    for (int i = 0; i < thresholdList.size() - 1; i++) {
        QColor currColor = colorMap.value(thresholdList.at(i));

        if (currValue < thresholdList.at(i + 1)) {
            drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.at(i), currValue, colorMap.value(thresholdList.at(i)));
            break;
        } else {
            drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.at(i), thresholdList.at(i + 1), colorMap.value(thresholdList.at(i)));
        }
    }

    if (currValue > thresholdList.last()) {
        drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.last(), currValue, colorMap.value(thresholdList.last()));
    }
    update();
}
