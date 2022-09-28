#ifndef TEXTENTERFRAME_H
#define TEXTENTERFRAME_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QBoxLayout>

#include "sectionrowframe.h"
#include "scrollwindow.h"

#include <vector>
#include <string>
#include <iostream>
#include <bits/stdc++.h>
class textEnterFrame : public QFrame
{
    Q_OBJECT
public:
    textEnterFrame(QWidget *parent = nullptr);
    std::string getGradesText();
    std::string getCourseName();
    std::string getPercentage();
    bool courseNameEmpty();
    void fillRows(QTextStream&);
protected:
    QRect* geometry;
    scrollWindow* scrollWindow;
    QScrollArea* scrollArea;
private:
    QPushButton *addField, *removeField, *calculate;
    QTextEdit *courseName;
    std::vector<sectionRowFrame*> list;
    sectionRowFrame *temp;
    QLabel **headers, *courseLabel;
    QVBoxLayout *layout;
    QFont *buttonFont;
    std::string percentage;
    int sectionHeight, w1, w2, w3;
private slots:
    void getGrade();
    void removeRow();
    void addRow();
    void clearRows();
signals:
    void result();

};

#endif // TEXTENTERFRAME_H
