#ifndef SCROLLWINDOW_H
#define SCROLLWINDOW_H

#include "qboxlayout.h"
#include <QWidget>

class scrollWindow : public QWidget
{
    Q_OBJECT
public:
    explicit scrollWindow(QWidget *parent = nullptr);
    QLayout *layout;
    void setLayout(QLayout*);
signals:

};

#endif // SCROLLWINDOW_H
