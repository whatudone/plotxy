#ifndef PLOT_MANAGER_H
#define PLOT_MANAGER_H

#include "PlotItemBase.h"
#include "constdef.h"
#include "ui_PlotManager.h"
#include <QtWidgets/QMainWindow>

class PlotItemBase;

class PlotManager : public QWidget
{
    Q_OBJECT

public:
    explicit PlotManager(QWidget* parent = Q_NULLPTR);
    ~PlotManager();

    void addPlot(const QString&, PlotItemBase*); //添加Plot
    void setTabWidgetRect(const QRect& rect);
    void clearCurSelectedPlot();

private:
    void init(); //初始化函数，连接信号槽
	void initTreeWidgetSettings();
	void initGeneralUI();
	void initAxisGridUI();
    void initAxisGridScrollUI();
    void initLinkAxesUI();
	void initPlotDataUI();
	void initTextEditUI();
	void initAttitudeUI();
    void initGOGUI();
    void initBarUI();
    void initDialUI();
	void initTextLightUI();
    void initScatterLimitUI();
    void initScatterMarkersUI();
    void initAScopeUI();
    void initRTIUI();
    void initDopplerUI();
    void initTrackUI();
    void initEditableMap();
    void initUnitData();

    void showScatterEditableItem(PlotType type);

	void refreshTreeWidgetSettingEnabled(PlotItemBase* plot);
	void refreshGeneralUI(PlotItemBase* plot);
	void refreshAxisGridUI(PlotItemBase* plot);
    void refreshAxisGridScrollUI(PlotItemBase* plot);
    void refreshLinkAxesUI(PlotItemBase* plot);
	void refreshPlotDataUI(PlotItemBase* plot);
	void refreshLightTextUI(PlotItemBase* plot);
    void refreshGOGUI(PlotItemBase* plot);
    void refreshAScopeUI(PlotItemBase* plot);
    void refreshRTIUI(PlotItemBase* plot);
    void refreshDopplerUI(PlotItemBase* plot);
    void refreshTrackUI(PlotItemBase* plot);

	void refreshTextEditUI(PlotItemBase* plot);
	void refreshAttitudeUI(PlotItemBase* plot);
    void refreshBarUI(PlotItemBase* plot);
    void refreshDialUI(PlotItemBase* plot);
    void refreshScatterLimitUI(PlotItemBase* plot);
    void refreshScatterMarkersUI(PlotItemBase* plot);

	void enableItem_Scatter();
	void enableItem_AScope();
	void enableItem_RTI();
	void enableItem_Text_Light();
	void enableItem_Bar();
	void enableItem_Dial();
	void enableItem_Attitude();
	void enableItem_Polar();
	void enableItem_Track();
	void enableItem_Doppler();

    void exchangeItem(QTableWidget* tableWidget, int selectedRow, int targetRow);
    void refreshRTIColorRange();
    void refreshDopplerColorRange();

public slots:
    void onTWSclicked(QTreeWidgetItem* item, int i);
    void onTWSPclicked(QTreeWidgetItem* item, int i);
    void onSelectedPlot(PlotItemBase* pBasePlot); //用于默认点击的Item

    void onUpdatePlotManager();
	void onBtnCloseClicked();

    void onBarHorizonChanged(int state);
	void spinboxBetweenChanged();
	void spinboxLeftChanged();
	void spinboxRightChanged();

	//General
	void onRadioPixelClicked();
	void onRadioPercentClicked();

	void onPlotRectEditFinished();
	void onLineEditPlotNameEditingFinished();
	void onCheckBox_drawStateChanged();
	void onPushButton_outerFillColorClicked();
	void onPushButton_outlineColorClicked();

	//Axis and Grid
    void oncheckBoxScrollXStateChanged();
    void oncheckBoxScrollYStateChanged();
    void onFollowXChanged();
    void onFollowYChanged();
    void onLeadXChanged();
    void onLeadYChanged();
    void onEnableDataPairChanged(QTableWidgetItem* item);

	void onLineEdit_limitXEditingFinished();
	void onLineEdit_limitYEditingFinished();
	void onPushButton_flipXValuesClicked();
	void onPushButton_flipYValuesClicked();
	void onLineEdit_horzGridsEditingFinished();
	void onLineEdit_vertGridsEditingFinished();
	void onSetAxisColorWidth();
	void onSetGridColorWidth();
	void onPushButton_gridFillClicked();
    void onCheckBox_6StateChanged(int state);
	void onPushButton_10Clicked();
	void onfontComboBox_3CurrentFontChanged(const QFont& font);
	void onComboBox_AxisGrid_FontSizeCurrentTextChanged(const QString& text);
	void onComboBox_2CurrentIndexChanged(int index);
    void onComboBox_3CurrentIndexChanged(int index);
    void onLineEdit_PrecisionXEditingFinished();
    void onLineEdit_PrecisionYEditingFinished();
    void onComboBox_XUnitChanged(const QString& newUnit);
    void onComboBox_YUnitChanged(const QString& newUnit);
    void onCheckBox_enableXCyclicalStateChanged(int state);
    void onCheckBox_enableYCyclicalStateChanged(int state);
    void onLineEdit_XCyclicalLowEditingFinished();
    void onLineEdit_XCyclicalUpperEditingFinished();
    void onLineEdit_YCyclicalLowEditingFinished();
    void onLineEdit_YCyclicalUpperEditingFinished();

    // Link Axis
    void onLinkAxesCheckedChanged(bool checked);
    void onRemoveLinkSet();
    void onResetLinkSet();
    void onAddLinkedSet();
    void onRemoveLinkedSet();
    void onUpdateLinkedSet();
    void onRemoveAllLinkedSet();
    //GOG
    void onPushButton_24Clicked();
    void onPushButton_25Clicked();
    void onPushButton_26Clicked();
    void onPushButton_27Clicked();
    void onListWidget_2Clicked();

    void addGOGTableItem(const QString& fileName);
    void onUpdateGOGCustomSetting();

	//Light/Text
	void onPushButton_71Clicked();
    void onPushButton_TextFillColorClicked();
    void onPushButtonTextOutlineColorClicked();
    void onSpinbox_10Changed();
    void onSpinbox_TextOutlineWidthChanged();
	void onPushButton_66Clicked();
	void onPushButton_67Clicked();
	void onPushButton_69Clicked();
    void onTableWidget_textLightDataSortItemSelectionChanged();
    void onCheckBox_RowGridVisible(int state);
    void onCheckBox_ColGridVisible(int state);
    void onSpinBox_TextLeftPadValueChanged(int value);
    void onSpinBox_TextRightPadValueChanged(int value);
    void onRadioBox_FillByRowClicked();
    void onRadioBox_FillByColClicked();
    void onSpinBox_NumRowsChanged(int value);
    void onSpinBox_NumColsChanged(int value);
    void onComboBox_LightShapeChanged(const QString& type);
    void onSpinBox_LightWidthChanged(int value);
    void onSpinBox_LightHeightChanged(int value);
    void onSpinBox_LightTextYPosChanged(int value);
    void onSpinBox_LightOutlineWidthChanged(int value);
    void onPushButton_LightOutlineColorClicked();

	//Plot Data
	void onTableWidget_plotDataItemSelectionChanged();
	void onLineEdit_24EditingFinished();
	void onPushButton_21Clicked();
	void onCheckBox_10StateChanged();
	void onCheckBox_11StateChanged();
	void onPushButton_15Clicked();
	void onPushButton_16Clicked();
	void onAddNewClicked();
	void onPushButton_18Clicked();
	void onPushButton_19Clicked();
    void onAdvancedClicked();
	void onToolButtonClicked();
	void onComboBox_6CurrentIndexChanged(int index);

	//Text Edit
	void onCheckBox_12StateChanged();
	void onCheckBox_13StateChanged();
	void onCheckBox_14StateChanged();
	void onLineEdit_26EditingFinished();
	void onPushButton_22Clicked();
	void onPushButton_23Clicked();
    void onfontComboBox_2CurrentFontChanged(const QFont& font);
    void onSpinBox_FontSizeChanged(int value);
    void onOffsetValueChanged();
    void onListWidgetItemChanged();
    void textSettingChanged();

	//Attitude
	void onPushButton_80Clicked();
	void onPushButton_81Clicked();
	void onSpinBox_29ValueChanged(int);
	void onSpinBox_30ValueChanged(int);
	void onSpinBox_31ValueChanged(int);

    //Dial
    void onSpinBox_21ValueChanged(int);
    void onSpinBox_22ValueChanged(int);
    void onSpinBox_23ValueChanged(int);
    void onSpinBox_24ValueChanged(int);
    void onSpinBox_25ValueChanged(int);
    void onSpinBox_26ValueChanged(int);
    void onSpinBox_27ValueChanged(int);
    void onSpinBox_28ValueChanged(int);
    void onPushButton_74Clicked();
    void onPushButton_75Clicked();
    void onPushButton_76Clicked();
    void onPushButton_77Clicked();
    void onComboBox_dialStyleCurrentTextChanged(const QString& text);
    void onCheckBox_34StateChanged(int state);
    void onCheckBox_35StateChanged(int state);
    void onCheckBox_36StateChanged(int state);
    void onCheckBox_37StateChanged(int state);
    // scatter
    void onAddScatterLimit();
    void onRemoveScatterLimit();
    void onUpdateScatterLimit();
    // 选中不同的Limit
    void onCurrentLimitChanged(QTreeWidgetItem* item, int column);
    // Plot Markers
    void onAddScatterMarkers();
    void onModifyScatterMarkers();
    void onDeleteScatterMarkers();
    void onCurrentMarkerChanged();

    // AScope
    void onCheckBox_DrawGateStateChanged(int state);
    void onSpinBox_GateHeightValueChanged(int value);
    void onPushButton_GateColorClicked();
    void onCheckBox_AutofitXStateChanged(int state);
    void onCheckBox_AutofitYStateChanged(int state);

    // RTI
    void onLineEdit_TimeSpanEditFinished();
    void onComboBox_LabelDesityTextChanged(const QString& text);
    void onComboBox_ColorIndexChanged(int index);
    void onSlider_colorValueChanged(int value);
    void onPushButton_NewColorClicked();
    void onPushButton_DeleteColorClicked();

    // Doppler
    void onCheckBox_ShowToolTipStateChanged(int state);
    void onPushButton_AScopeAxisColorClicked();
    void onPushButton_AScopeAxisLineColorClicked();
    void onSpinBox_AScopeAxisLineWidthChanged(int value);
    void onSlider_DopplerColorValueChanged(int value);
    void onComboBox_DopplerColorIndexChanged(int index);
    void onPushButton_DopplerNewColorClicked();
    void onPushButton_DopplerDeleteColorClicked();

    // Track
    void onUnavaBarColorChanged();
    void onInvalidBarColorChanged();

signals:
    void sigAddPlotPair();
	void sigAdvancedDataManager();
	void sigGetTabRect();
	void sigSetPlotVisible(bool);

private:
    Ui::PlotManager ui;

	QTreeWidgetItem* m_itemGeneral;
	QTreeWidgetItem* m_itemAxis;
    QTreeWidgetItem* m_itemPlotData;
	QTreeWidgetItem* m_itemText;
	QTreeWidgetItem* m_itemGOG;
	QTreeWidgetItem* m_itemLinkedAxis;
	QTreeWidgetItem* m_itemScatterPlot;
	QTreeWidgetItem* m_itemLimits;
	QTreeWidgetItem* m_itemPlotMarkers;
	QTreeWidgetItem* m_itemTimeLine;
	QTreeWidgetItem* m_itemHandsOff;
	QTreeWidgetItem* m_itemAScope;
	QTreeWidgetItem* m_itemRTI;
	QTreeWidgetItem* m_itemTextLight;
	QTreeWidgetItem* m_itemBar;
	QTreeWidgetItem* m_itemDial;
	QTreeWidgetItem* m_itemAttitude;
	QTreeWidgetItem* m_itemTrackStatus;
	QTreeWidgetItem* m_itemRangeDoppler;

	QRect m_tabWidgetRect;
    // 本界面当前选中图表，不影响主界面选中图表
    PlotItemBase* m_curSelectPlot = nullptr;
    // 不同图表的Text可编辑属性不同
    QMap<PlotType, QList<QString>> m_itemTextEditableMap;
    QHash<QString, QStringList> m_unitsMap;
    QMap<QString, QHash<QString, double>>
        m_rateMap; // QString：文件中的原始单位  QString：当前选中的单位  double：原始单位转换成当前单位需要除以的比例。例如 <m<mm,0.001>>
    QImage m_RTIColorSliderImage;
    QImage m_DopplerColorSliderImage;
};

#endif // !
