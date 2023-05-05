#pragma once

#include "TimeClass.h"
#include "ui_TimeControls.h"
#include <QWidget>

class TimeControls : public QWidget
{
	Q_OBJECT

public:
    explicit TimeControls(QWidget* parent = Q_NULLPTR);
	~TimeControls();
    double getMultiplizer()
    {
        return m_Multiplier;
    }
    double getStepFactor()
    {
        return m_stepFactor;
    }
    bool getLoopTime()
    {
        return m_bLoopTime;
    }
    int getRefYear()
    {
        return m_refYear;
    }

private:
	Ui::TimeControls ui;
    double m_Multiplier;
    double m_minTime, m_maxTime;
    int m_refYear;
    double m_stepFactor;
    bool m_bLoopTime;

private:
	void setBeginTime(double seconds, int refYear);
	void setCurTime(double seconds, int refYear);
	void setEndTime(double seconds, int refYear);
    void setRefTime(int);

	double getBeginTime(int refYear);
	double getCurTime(int refYear);
	double getEndTime(int refYear);

public slots:
	void onAlwaysOnTop();
	void onUpdateData();
    void onClose();
    void onSetSliderRange(int min, int max, int singleStep);
	void onSliderValueChanged(int);
	void onRemoteSliderValueChanged(int);

	void onBeginNowClicked();
	void onBeginResetClicked();
	void onEndNowClicked();
	void onEndResetClicked();
	void onEnableActionStop(bool);

	void onCheckBox_LoopTimeClicked();

	void onActionPlay();
	void onActionStop();
	void onActionReverse();
	void onActionFrameReverse();
	void onActionFrameForward();
	void onActionDecreseStep();
	void onActionIncreaseStep();
	void onActionTimeServer();
	void onActionTimeClient();
	void onActionRealTime();

	void onLineEdit_stepFactor_EditingFinished();

signals:
    void sgn_setSliderRange(int min, int max, int singleStep);
	void sgn_sliderValueChanged(int);
	void sgn_actionPlay();
	void sgn_actionStop();
	void sgn_actionReverse();
	void sgn_actionFrameReverse();
	void sgn_actionFrameForward();
	void sgn_actionDecreseStep();
	void sgn_actionIncreaseStep();
	void sgn_actionTimeServer();
	void sgn_actionTimeClient();
	void sgn_actionRealTime();
};
