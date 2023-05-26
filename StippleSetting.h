#pragma once
/*
 * stipple作为图表的一种纹理（brush），仅在线性模式下才生效
*/
#include "ui_StippleSetting.h"
#include <QWidget>

class StippleSetting : public QWidget
{
	Q_OBJECT

public:
    StippleSetting(QWidget* parent = Q_NULLPTR);
	~StippleSetting();

    void setStippleEnable(bool enable);
    void setStipplePattern(Qt::PenStyle style);
    void setStippleCustomPattern(const QString& pattern);
    void setStippleFactor(int factor);
signals:
    void stippleEnableChanged(bool enable);
    void stipplePatternChanged(Qt::PenStyle style);
    void stippleCustomPatternChanged(const QString& pattern);
    void stippleFactorChanged(int factor);

private:
    void initPatternComboBox();
    void initConnections();

private:
	Ui::StippleSetting ui;

private:
    QMap<Qt::PenStyle, QString> m_patternMap;
};
