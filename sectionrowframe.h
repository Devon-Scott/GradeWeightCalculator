#ifndef SECTIONROWFRAME_H
#define SECTIONROWFRAME_H

#include <QFrame>
#include <QTextEdit>
#include <sstream>

class sectionRowFrame : public QFrame
{
    Q_OBJECT
public:
    sectionRowFrame(QFrame *parent = nullptr);
    std::string getSectionMark(), getRowText(int, int, int), getSectionName();
    double getGradeValue(), getWeightValue(), getWeight();
    bool isEmpty();
    void clearRow();
    void fillRow(QString&);
signals:
    void markWeight(QString, QString);
protected:

private:
    const int FIELDS = 3;
    QTextEdit **fields;
};

#endif // SECTIONROWFRAME_H
