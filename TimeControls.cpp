﻿#include "TimeControls.h"
#include "data_manager_data.h"

#include <QDate>
#include <QDebug>
#include <QDoubleValidator>

TimeControls::TimeControls(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_Multiplier = 100.0;
	m_minTime = 0;
	m_maxTime = 0;
	m_refYear = 1970;
	m_stepFactor = 0.1;
	m_bLoopTime = false;

	ui.toolButton_stop->setEnabled(false);

	QDoubleValidator* doubleValid = new QDoubleValidator(0, 100, 3);
	ui.lineEdit_stepFactor->setValidator(doubleValid);
    connect(ui.lineEdit_stepFactor,
            &QLineEdit::editingFinished,
            this,
            &TimeControls::onLineEdit_stepFactor_EditingFinished);

	connect(ui.pushButton_2, &QPushButton::clicked, this, &TimeControls::onClose);
	connect(ui.horizontalSlider, &QSlider::valueChanged, this, &TimeControls::onSliderValueChanged);
	connect(ui.pushButton_beginNow, &QPushButton::clicked, this, &TimeControls::onBeginNowClicked);
    connect(
        ui.pushButton_beginReset, &QPushButton::clicked, this, &TimeControls::onBeginResetClicked);
	connect(ui.pushButton_endNow, &QPushButton::clicked, this, &TimeControls::onEndNowClicked);
	connect(ui.pushButton_endReset, &QPushButton::clicked, this, &TimeControls::onEndResetClicked);

    connect(
        ui.checkBox_loopTime, &QCheckBox::clicked, this, &TimeControls::onCheckBox_LoopTimeClicked);

	connect(ui.toolButton_playForward, &QToolButton::clicked, this, &TimeControls::onActionPlay);
    connect(
        ui.toolButton_playBackwards, &QToolButton::clicked, this, &TimeControls::onActionReverse);
	connect(ui.toolButton_stop, &QToolButton::clicked, this, &TimeControls::onActionStop);
    connect(ui.toolButton_stepForward,
            &QToolButton::clicked,
            this,
            &TimeControls::onActionFrameForward);
    connect(
        ui.toolButton_stepBack, &QToolButton::clicked, this, &TimeControls::onActionFrameReverse);
    connect(
        ui.toolButton_Increase, &QToolButton::clicked, this, &TimeControls::onActionIncreaseStep);
    connect(
        ui.toolButton_Decrease, &QToolButton::clicked, this, &TimeControls::onActionDecreseStep);
	connect(ui.toolButton_TS, &QToolButton::clicked, this, &TimeControls::onActionTimeServer);
	connect(ui.toolButton_TC, &QToolButton::clicked, this, &TimeControls::onActionTimeClient);
    connect(ui.toolButton_RealTime, &QToolButton::clicked, this, &TimeControls::onActionRealTime);

    connect(this, &TimeControls::sgn_setSliderRange, this, &TimeControls::onSetSliderRange);

    ui.checkBox_T0->setVisible(false);
    ui.label_15->setVisible(false);
    ui.lineEdit_TALO->setVisible(false);
    ui.label_16->setVisible(false);
    ui.pushButton->setVisible(false);

    ui.checkBox_loopTime->setVisible(false);
    ui.label_17->setVisible(false);
    ui.spinBox_refTimeD->setVisible(false);
    ui.spinBox_refTimeY->setVisible(false);
    ui.spinBox_refTimeH->setVisible(false);
    ui.label_20->setVisible(false);
    ui.spinBox_refTimeM->setVisible(false);
    ui.label_18->setVisible(false);
    ui.doubleSpinBox_refTimeS->setVisible(false);
    ui.label_19->setVisible(false);
    ui.pushButton_refNow->setVisible(false);
}

TimeControls::~TimeControls() {}

void TimeControls::setBeginTime(double seconds, int refYear)
{
	unsigned int day, hour, minute;
	float second;

    OrdinalTimeFormatter::secondsConvertToTime(seconds, refYear, day, hour, minute, second);
	ui.spinBox_beginTimeD->setValue(day);
    ui.spinBox_beginTimeY->setValue(seconds > 0 ? refYear : refYear - 1);
	ui.spinBox_beginTimeH->setValue(hour);
	ui.spinBox_beginTimeM->setValue(minute);
	ui.doubleSpinBox_beginTimeS->setValue(second);

    ui.spinBox_beginTimeY->setMinimum(seconds > 0 ? refYear : refYear - 1);
}

void TimeControls::setCurTime(double seconds, int refYear)
{
	unsigned int day, hour, minute;
	float second;

    OrdinalTimeFormatter::secondsConvertToTime(seconds, (double)refYear, day, hour, minute, second);
	ui.spinBox_curTimeD->setValue(day);
    ui.spinBox_curTimeY->setValue(seconds > 0 ? refYear : refYear - 1);
	ui.spinBox_curTimeH->setValue(hour);
	ui.spinBox_curTimeM->setValue(minute);
	ui.doubleSpinBox_curTimeS->setValue(second);
}

void TimeControls::setEndTime(double seconds, int refYear)
{
	unsigned int day, hour, minute;
	float second;

    OrdinalTimeFormatter::secondsConvertToTime(seconds, (double)refYear, day, hour, minute, second);
	ui.spinBox_endTimeD->setValue(day);
    ui.spinBox_endTimeY->setValue(seconds > 0 ? refYear : refYear - 1);
	ui.spinBox_endTimeH->setValue(hour);
	ui.spinBox_endTimeM->setValue(minute);
	ui.doubleSpinBox_endTimeS->setValue(second);

    ui.spinBox_endTimeY->setMinimum(seconds > 0 ? refYear : refYear - 1);
}

void TimeControls::setRefTime(int refYear)
{
	//ui.spinBox_refTimeD->setValue(day);
	ui.spinBox_refTimeY->setValue(refYear);
	//ui.spinBox_refTimeH->setValue(hour);
	//ui.spinBox_refTimeM->setValue(minute);
	//ui.doubleSpinBox_refTimeS->setValue(second);
}

double TimeControls::getBeginTime(int refYear)
{
	unsigned int day, hour, minute, year;
	float second;

	day = ui.spinBox_beginTimeD->value();
	year = ui.spinBox_beginTimeY->value();
	hour = ui.spinBox_beginTimeH->value();
	minute = ui.spinBox_beginTimeM->value();
	second = ui.doubleSpinBox_beginTimeS->value();

	QString str = QString("%1 %2 %3:%4:%5").arg(day).arg(year).arg(hour).arg(minute).arg(second);

    double secs = OrdinalTimeFormatter::convertToSeconds(str, refYear);
	return secs;
}

double TimeControls::getCurTime(int refYear)
{
	unsigned int day, hour, minute, year;
	float second;

	day = ui.spinBox_curTimeD->value();
	year = ui.spinBox_curTimeY->value();
	hour = ui.spinBox_curTimeH->value();
	minute = ui.spinBox_curTimeM->value();
	second = ui.doubleSpinBox_curTimeS->value();

	QString str = QString("%1 %2 %3:%4:%5").arg(day).arg(year).arg(hour).arg(minute).arg(second);

    double secs = OrdinalTimeFormatter::convertToSeconds(str, refYear);
	return secs;
}

double TimeControls::getEndTime(int refYear)
{
	unsigned int day, hour, minute, year;
	float second;

	day = ui.spinBox_endTimeD->value();
	year = ui.spinBox_endTimeY->value();
	hour = ui.spinBox_endTimeH->value();
	minute = ui.spinBox_endTimeM->value();
	second = ui.doubleSpinBox_endTimeS->value();

	QString str = QString("%1 %2 %3:%4:%5").arg(day).arg(year).arg(hour).arg(minute).arg(second);

    double secs = OrdinalTimeFormatter::convertToSeconds(str, refYear);
	return secs;
}

void TimeControls::onUpdateData()
{
    auto dataMap = DataManagerDataInstance->getDataMap();
    if(dataMap.isEmpty())
		return;

    DataManagerDataInstance->getMinMaxTime(m_minTime, m_maxTime);
    m_refYear = DataManagerDataInstance->getRefYear();

	setBeginTime(m_minTime, m_refYear);
    setCurTime(m_maxTime, m_refYear);
	setEndTime(m_maxTime, m_refYear);
    setRefTime(m_refYear);

    emit sgn_setSliderRange(
        m_minTime * m_Multiplier, m_maxTime * m_Multiplier, m_stepFactor * m_Multiplier);
    ui.horizontalSlider->setValue(m_maxTime * m_Multiplier);
    emit sgn_sliderValueChanged(m_maxTime * m_Multiplier);
}

void TimeControls::onUpdateRealData()
{
    auto dataMap = DataManagerDataInstance->getDataMap();
    if(dataMap.isEmpty())
        return;

    DataManagerDataInstance->getMinMaxRealTime(m_minTime, m_maxTime);
    m_refYear = QDate::currentDate().year();

    setBeginTime(m_minTime, m_refYear);
    setCurTime(m_maxTime, m_refYear);
    setEndTime(m_maxTime, m_refYear);
    setRefTime(m_refYear);

    emit sgn_setSliderRange(
        m_minTime * m_Multiplier, m_maxTime * m_Multiplier, m_stepFactor * m_Multiplier);
    ui.horizontalSlider->setValue(m_maxTime * m_Multiplier);
    emit sgn_sliderValueChanged(m_maxTime * m_Multiplier);
}

void TimeControls::onClose()
{
	this->close();
}

void TimeControls::onSetSliderRange(int min, int max, int singleStep)
{
	ui.horizontalSlider->setMinimum(min);
	ui.horizontalSlider->setMaximum(max);
	ui.horizontalSlider->setPageStep(singleStep);
}

void TimeControls::onRemoteSliderValueChanged(int value)
{
	ui.horizontalSlider->setValue(value);
    setCurTime(value / m_Multiplier, m_refYear);
}

void TimeControls::onSliderValueChanged(int value)
{
	emit sgn_sliderValueChanged(value);
}

void TimeControls::onBeginNowClicked()
{
	double curSecs = getCurTime(m_refYear);
	setBeginTime(curSecs, m_refYear);

    emit sgn_setSliderRange(
        curSecs * m_Multiplier, getEndTime(m_refYear) * m_Multiplier, m_stepFactor * m_Multiplier);
}

void TimeControls::onBeginResetClicked()
{
	setBeginTime(m_minTime, m_refYear);

    emit sgn_setSliderRange(m_minTime * m_Multiplier,
                            getEndTime(m_refYear) * m_Multiplier,
                            m_stepFactor * m_Multiplier);
}

void TimeControls::onEndNowClicked()
{
	double curSecs = getCurTime(m_refYear);
	setEndTime(curSecs, m_refYear);

    emit sgn_setSliderRange(getBeginTime(m_refYear) * m_Multiplier,
                            curSecs * m_Multiplier,
                            m_stepFactor * m_Multiplier);
}

void TimeControls::onEndResetClicked()
{
	setEndTime(m_maxTime, m_refYear);

    emit sgn_setSliderRange(getBeginTime(m_refYear) * m_Multiplier,
                            m_maxTime * m_Multiplier,
                            m_stepFactor * m_Multiplier);
}

void TimeControls::onEnableActionStop(bool enabled)
{
	ui.toolButton_stop->setEnabled(enabled);
}

void TimeControls::onCheckBox_LoopTimeClicked()
{
	m_bLoopTime = (ui.checkBox_loopTime->checkState() == Qt::Checked) ? true : false;
}

void TimeControls::onActionPlay()
{
	emit sgn_actionPlay();
}

void TimeControls::onActionStop()
{
	emit sgn_actionStop();
}

void TimeControls::onActionReverse()
{
	emit sgn_actionReverse();
}

void TimeControls::onActionFrameReverse()
{
	emit sgn_actionFrameReverse();
}

void TimeControls::onActionFrameForward()
{
	emit sgn_actionFrameForward();
}

void TimeControls::onActionDecreseStep()
{
	emit sgn_actionDecreseStep();
}

void TimeControls::onActionIncreaseStep()
{
	emit sgn_actionIncreaseStep();
}

void TimeControls::onActionTimeServer()
{
	emit sgn_actionTimeServer();
}

void TimeControls::onActionTimeClient()
{
	emit sgn_actionTimeClient();
}

void TimeControls::onActionRealTime()
{
	emit sgn_actionRealTime();
}

void TimeControls::onLineEdit_stepFactor_EditingFinished()
{
	m_stepFactor = ui.lineEdit_stepFactor->text().toDouble();
}
