#ifndef _ADVANCED_DATA_MANAGER_H_
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
    // ColorRange
    void onBtnMore();
    void onBtnColorMore();
    void onColorRangesEnable(bool enable);
    void onColorRangesDefColorChanged(const QColor& color);
    void onColorRangesModeChanged(DataPair::ColorRangeMode mode);

    void onBtnAddColorRange();
    void onBtnUpdateColorRange();
    void onBtnRemoveColorRange();

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
    void onLineWidthChanged(int32_t width);
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
    void onLabelSettings_xUnit(const QString& xUnit);
    void onLabelSettings_yUnit(const QString& yUnit);

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

    // stipple
    void onStippleEnableChanged(bool enable);
    void onStipplePatternChanged(Qt::PenStyle style);
    void onStippleCustomPatternChanged(const QString& pattern);
    void onStippleFactorChanged(int factor);

    // Event
    void onEventBtnMoreClicked();
    // 刷新当前实体中包含的所有通用数据标签
    void onGenericDataEntityChanged(int32_t row, int32_t col);
    void onAddEvent();
    void onRemoveEvent();

    // connections
    void onConBtnMoreClicked();
    void onAddConnection();
    void onUpdateConnection();
    void onRemoveConnection();
    void onConnectionItemClicked();

private:
    // 根据数据对数据刷新UI
	void refreshUI();
	void refreshGeneral();
	void refreshIcon();
	void refreshExtrapolation();
	void refreshLabelSettings();
	void refreshStipple();
	void refreshEvent();
    void refreshAddedEventList();
	void refreshLabelText();
	void refreshColorRanges();
    void refreshConnections();

    void initConnections();

    void initGeneralConnections();
    void initIconConnections();
    void initLabelSettingsConnections();
    void initLabelTextConnections();
    void initEventConnections();
    void initStippleConnections();
    void initColorRangeConnections();
    void initConConnections();

private:
    Ui::AdvancedDataManager ui;

	PlotItemBase* m_curSelectPlot = nullptr;
	DataPair* m_curSelectDatapair = nullptr;
	SubSettingWidgetContainer* subSettingWidgetContainer;
};

#endif // _ADVANCED_DATA_MANAGER_H_
