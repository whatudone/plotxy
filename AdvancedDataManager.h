#ifndef _ADVANCED_DATA_MANAGER_H_
#define _ADVANCED_DATA_MANAGER_H_

#include <QWidget>
#include <QColor>
#include <QColorDialog>
#include <QVBoxLayout>
#include "ui_AdvancedDataManager.h"
#include "PlotItemBase.h"
#include "DataPair.h"

class AdvancedDataManager : public QWidget
{
    Q_OBJECT

public:
    explicit AdvancedDataManager(QWidget* parent = 0);
    ~AdvancedDataManager();

    QMap<QString, QMap<int, QColor>> m_map;

signals:
    void updateColorThresholdMap(QMap<QString, QMap<int, QColor>>);
	void sgnAddPlotPair();

	//General
	void sgnGeneral_draw(bool);
	void sgnGeneral_color(QColor);

	//Label Settings
	void sgnLabelSettings_draw(bool);
	void sgnLabelSettings_color(QColor);
	void sgnLabelSettings_background(QColor);
	void sgnLabelSettings_transparent(bool);
	void sgnLabelSettings_font(QFont);
	void sgnLabelSettings_fontSize(int);
	void sgnLabelSettings_position(int);
	void sgnLabelSettings_precision_x(int);
	void sgnLabelSettings_precision_y(int);
	void sgnLabelSettings_xUnit(QString);
	void sgnLabelSettings_yUnit(QString);

	//Label Text
	void sgnLabelText_format(int);
	void sgnLabelText_prefix(bool);
	void sgnLabelText_object(bool);
	void sgnLabelText_attr(bool);
	void sgnLabelText_data(bool);
	void sgnLabelText_unit(bool);
	void sgnLabelText_custom(QString);

	//Icon Setting
	void sgnIconSetting_draw(bool);
	void sgnIconSetting_name(QString);
	void sgnIconSetting_rotation(int);
	void sgnIconSetting_flipHorz(bool);
	void sgnIconSetting_flipVert(bool);
	void sgnIconSetting_width(int);
	void sgnIconSetting_height(int);
	void sgnIconSetting_color(QColor);

public slots:
    void onBtnMore();
    void onBtnColorMore();
    void onBtnAdd();
	void onEventBtnMoreClicked();
	void onUpdatePlotPair();
	void onTableWidget_plotpairItemSelectionChanged();
	void onPushButton_addClicked();
	void onPushButton_copyClicked();
	void onPushButton_autofitClicked();
	void onPushButton_deleteClicked();

	//General
	void onGeneral_draw(bool);
	void onGeneral_color(QColor);

	//Label Settings
	void onLabelSettings_draw(bool);
	void onLabelSettings_color(QColor);
	void onLabelSettings_background(QColor);
	void onLabelSettings_transparent(bool);
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
	void refreshUI();
	void refreshAdvancedDataManagerUI();
	void refreshGeneral();
	void refreshIcon();
	void refreshExtrapolation();
	void refreshLabelSettings();
	void refreshStipple();
	void refreshEvent();
	void refreshLabelText();
	void refreshColorRanges();

private:
    Ui::AdvancedDataManager ui;
	QMap<QString, QList<PlotItemBase*>> m_plotManager; //tabName
	PlotItemBase* m_curSelectPlot = nullptr;
	DataPair* m_curSelectDatapair = nullptr;
};

#endif // _ADVANCED_DATA_MANAGER_H_