#include "AdvancedDataManager.h"
#include "SubSettingWidgetContainer.h"

#include <QColorDialog>
#include <QColor>

AdvancedDataManager::AdvancedDataManager(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("AdvancedDataManager"));
    this->resize(1440,960);

	QVBoxLayout* vBoxLayout = new QVBoxLayout;
	SubSettingWidgetContainer* subSettingWidgetContainer = new SubSettingWidgetContainer();
	vBoxLayout->addWidget(subSettingWidgetContainer);

	ui.widget_drawer->setLayout(vBoxLayout);
	ui.stackedWidget_aDMrpart->setCurrentIndex(2);

    connect(ui.pushButton_choosecolor,&QPushButton::clicked,this,&AdvancedDataManager::onBtnColorChooseClicked);
	
	connect(ui.pushButton_color, SIGNAL(clicked()), this, SLOT(onBtnColorClicked()));
	connect(ui.pushButton_add_2, SIGNAL(clicked()), this, SLOT(onBtnAdd()));

	connect(subSettingWidgetContainer->m_general, SIGNAL(sigBtnGenneralMoreclicked()), this, SLOT(onBtnMore()));
	connect(subSettingWidgetContainer->m_colorRanges, SIGNAL(sigBtnColorRangesMoreclicked()), this, SLOT(onBtnColorMore()));

	connect(ui.treeWidget_xy,SIGNAL(currentItemChanged(QTreeWidgetItem * , QTreeWidgetItem * )),
			this,SLOT(onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
}

AdvancedDataManager::~AdvancedDataManager()
{

}

void AdvancedDataManager::onBtnColorChooseClicked()
{
	QColor selectedcolor;
	selectedcolor = QColorDialog::getColor(Qt::white, this);
	if(!selectedcolor.isValid())
	{ }
	else
	{
		ui.pushButton_choosecolor->setStyleSheet("background-color: " + selectedcolor.name() + ";");
	}
}

void AdvancedDataManager::onBtnColorClicked()
{
	QColor selectedcolor;
	selectedcolor = QColorDialog::getColor(Qt::yellow, this);
	if (!selectedcolor.isValid())
	{
	}
	else
	{
		ui.pushButton_color->setStyleSheet("background-color: " + selectedcolor.name() + ";");
	}
}

void AdvancedDataManager::onAdvancedDataManagerAddPair(QString entityType, QString entityAttr)
{
	QTreeWidgetItem* currItem = new QTreeWidgetItem();
	currItem->setText(0, entityType);
	currItem->setText(1, entityAttr);
	ui.treeWidget_xy->addTopLevelItem(currItem);
}

void AdvancedDataManager::onCurrentItemChanged(QTreeWidgetItem* currItem, QTreeWidgetItem* preItem)
{
	//清除老的，更新新的
	ui.treeWidget_union->clear();

	QString x_y;
	QString x, y;
	x = currItem->text(0);
	y = currItem->text(1);
	x_y = x + "_" + y;

	for (auto it = m_map[x_y].begin();it != m_map[x_y].end();it++)
	{
		QTreeWidgetItem* addUnion = new QTreeWidgetItem;
		addUnion->setText(1, QString::number(it.key()));

		ui.treeWidget_union->addTopLevelItem(addUnion);
	}

}

void AdvancedDataManager::onBtnAdd()
{
	QString x_y;
	QString x, y;
	QTreeWidgetItem* item;
	item = ui.treeWidget_xy->currentItem();
	x = item->text(0);
	y = item->text(1);
	x_y = x + "_" + y;


	QString key = ui.lineEdit_value->text();
	int intKey = key.toInt();
	QPalette pal = ui.pushButton_color->palette();
	QBrush brush = pal.base();
	QColor value = brush.color();

	m_map[x_y].insert(intKey, value);

	QTreeWidgetItem* addUnion = new QTreeWidgetItem;
	addUnion->setText(0, ui.lineEdit_name->text());
	addUnion->setText(1, ui.lineEdit_value->text());
	addUnion->setBackground(2, brush);

	ui.treeWidget_union->addTopLevelItem(addUnion);

	emit updateColorThresholdMap(m_map);
}


void AdvancedDataManager::onBtnMore()
{
	ui.stackedWidget_aDMrpart->setCurrentIndex(0);
}

void AdvancedDataManager::onBtnColorMore()
{
	ui.stackedWidget_aDMrpart->setCurrentIndex(1);
}