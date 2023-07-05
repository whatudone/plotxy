#ifndef PLOTDIAL_H
#define PLOTDIAL_H

#include "BaseData.h"
#include "PlotItemBase.h"
#include "constdef.h"

#include <QMap>
#include <QString>
#include <QTimer>

class PlotDial : public PlotItemBase
{
    Q_OBJECT

public:
    PlotDial(QWidget* parent = Q_NULLPTR);
    ~PlotDial() override;

    static int m_instanceCount; //实体个数
    PlotType plotType() const override
    {
        return Type_PlotDial;
    }

    void setTickRadiusRate(int rate);
    void setColorRate(int rate);
    void setTextRate(int rate);
    void setDialRate(int rate);
    void setDialCapRate(int rate);
    void setStartAngle(int angle);
    void setEndAngle(int angle);
    void setCapColor(QColor color);
    void setDrawFirstTick(bool draw);
    void setDrawLastTick(bool draw);
    void setDrawFirstTextLabel(bool draw);
    void setDrawLastTextLabel(bool draw);
    void setColorInfoList(const QList<DialColorInfo>& colorInfoList);
    void setDialStyle(const QString& style);

    int getTickRadiusRate();
    int getColorRate();
    int getTextRate();
    int getDialRate();
    int getDialCapRate();
    int getStartAngle();
    int getEndAngle();
    const QColor getCapColor();
    bool getDrawFirstTick();
    bool getDrawLastTick();
    bool getDrawFirstTextLabel();
    bool getDrawLastTextLabel();
    const QList<DialColorInfo> getColorInfoList();
    const QString getDialStyle();

private:
    void updateDataForDataPairsByTime(double secs) override;
    void customPainting(QPainter& painter) override;
    void updateGraphByDataPair(DataPair* data, double curSecs) override;
    void updateCenterPoint();

private:
    QColor m_dialColor;
    QColor m_capColor;

    QPoint m_centerPoint;
    int m_circleRadius;

    int m_radiusRate;
    int m_colorRangeRate;
    int m_textRate;
    int m_dialRate;
    int m_dialCapRate;
    int m_startAngle;
    int m_endAngle;

    bool m_drawFirstTick = true;
    bool m_drawLastTick = true;
    bool m_drawFirstTextLabel = true;
    bool m_drawLastTextLabel = true;
    QString m_dialStyle;

    QMap<QString, double> m_valueMap;
    QList<DialColorInfo> m_colorInfoList;
};

#endif // PLOTDIAL_H
