/* * @filename: PlotItemBase.h
* @brief: PlotBase Plot基类
* @author: zhameng
* @create time: 2022-09-21
*  */

#ifndef  _PLOT_ITEM_BASE_H_
#define  _PLOT_ITEM_BASE_H_

#include <QtWidgets/QMainWindow>
#include "ui_PlotItemBase.h"


class PlotItemBase : public QWidget
{
    Q_OBJECT

public:
    explicit PlotItemBase(QWidget* parent = 0);
    ~PlotItemBase();

public:
    void init();                        //初始化函数，连接信号槽

    void setPosition(int x,int y);      //设置包围盒左上角位置
    void setWidth(int width);           //设置宽度
    void setHeight(int height);         //设置包围盒高度

    QPoint currPosition();
    int currWidth();
    int currHeight();

    void setName(const QString&);
    void setTabName(const QString&);
    
    //void setVisible(bool);            //始作俑者，万恶之首！跟踪了一天无法显示的bug

    QString currName();
    QString currTabName();

protected:
    //virtual void paintEvent(QPaintEvent* event);

private:
    QPoint  m_position;
    int m_width;
    int m_height;
    QString m_plotItemName;
    QString m_tabName;
    bool    m_bVisible;

    Ui::PlotItemBase ui;
};

#endif // ! 