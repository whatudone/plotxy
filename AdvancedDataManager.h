﻿#ifndef _ADVANCED_DATA_MANAGER_H_
#define _ADVANCED_DATA_MANAGER_H_

#include "DataPair.h"
#include "PlotItemBase.h"
#include "SubSettingWidgetContainer.h"
#include "ui_AdvancedDataManager.h"
#include <QColor>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QWidget>

class AdvancedDataManager : public QWidget
{
    Q_OBJECT

public:
    explicit AdvancedDataManager(QWidget* parent = nullptr);
    ~AdvancedDataManager();

signals:
	void sgnAddPlotPair();
	void sgnFilterSort(QString);

public slots:
    void onBtnMore();
    void onBtnColorMore();
    void onBtnAddColorRange();
    void onBtnUpdateColorRange();

	void onEventBtnMoreClicked();
    void onUpdatePlotPair();
    // 选中不同数据对
	void onTableWidget_plotpairItemSelectionChanged();
	void onLineEditChanged();
	void onPushButton_addClicked();
	void onPushButton_copyClicked();
	void onPushButton_autofitClicked();
	void onPushButton_deleteClicked();
	void onPushButton_closeClicked();

	//General
	void onGeneral_draw(bool);
	void onGeneral_color(QColor);
	void onGeneral_matchColor(bool);
    void onLineModeChanged(bool lineMode);
    void onLineWidthChanged(int32_t lineWidth);
	//Label Settings
	void onLabelSettings_draw(bool);
	void onLabelSettings_color(QColor);
	void onLabelSettings_background(QColor);
    void onLabelSettingsSecColor(const QColor& color);
	void onLabelSettings_font(QFont);
	void onLabelSettings_fontSize(int);
	void onLabelSettings_position(int);
	void onLabelSettings_precision_x(int);
	void onLabelSettings_precision_y(int);
	void onLabelSettings_xUnit(QString);
	void onLabelSettings_yUnit(QString);

	//Label Text
	void onLabelText_format(int);
	void onLabelText_prefix(bool);
	void onLabelText_object(bool);
	void onLabelText_attr(bool);
	void onLabelText_data(bool);
	void onLabelText_unit(bool);
	void onLabelText_custom(QString);

	//Icon Setting
	void onIconSetting_draw(bool);
	void onIconSetting_name(QString);
	void onIconSetting_rotation(int);
	void onIconSetting_flipHorz(bool);
	void onIconSetting_flipVert(bool);
	void onIconSetting_width(int);
	void onIconSetting_height(int);
	void onIconSetting_color(QColor);

private:
    // 根据数据对数据刷新UI
	void refreshUI();
	void refreshGeneral();
	void refreshIcon();
	void refreshExtrapolation();
	void refreshLabelSettings();
	void refreshStipple();
	void refreshEvent();
	void refreshLabelText();
	void refreshColorRanges();

    void initConnections();
    void initGeneralConnections();
    void initIconConnections();
    void initLabelSettingsConnections();
    void initLabelTextConnections();
    void initColorRangeConnections();
    void initEventConnections();

private:
    Ui::AdvancedDataManager ui;

	PlotItemBase* m_curSelectPlot = nullptr;
	DataPair* m_curSelectDatapair = nullptr;
	SubSettingWidgetContainer* subSettingWidgetContainer;
};

#endif // _ADVANCED_DATA_MANAGER_H_
