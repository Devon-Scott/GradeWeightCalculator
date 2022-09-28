#include "textenterframe.h"
#include "qapplication.h"

textEnterFrame::textEnterFrame(QWidget *parent) : QFrame(parent)
{
    // Widths held for file saving and loading
    w1 = 20, w2 = 20, w3 = 10;
    courseLabel = new QLabel("Course Name:", this);
    courseLabel->setGeometry(5, 5, 100, 30);

    courseName = new QTextEdit(this);
    courseName->setGeometry(85, 5, 295, 30);
    courseName->setVisible(true);
    courseName->setTabChangesFocus(true);

    headers = new QLabel*[3];
    for (int i = 0; i < 3; i++){
        headers[i] = new QLabel((i == 0 ? "Section:" :
                                 i == 1 ? "Grade:" :
                                 i == 2 ? "Weight:" : ""),
                                this);
        headers[i]->setGeometry((125 * i), 35, 125, 30);
        headers[i]->setAlignment(Qt::AlignCenter);
    }

    list = std::vector<sectionRowFrame*>();
    for (int i = 0; i < 5; i++){
        addRow();
    }

    buttonFont = new QFont("Helvetica", 30, 1, false);

    percentage = "";

    addField = new QPushButton(this);
    addField->setText("+");
    addField->setGeometry(420, 40, 50, 50);
    addField->setFont(*buttonFont);
    addField->setToolTip("Add a field");

    removeField = new QPushButton(this);
    removeField->setText("-");
    removeField->setGeometry(420, 90, 50, 50);
    removeField->setFont(*buttonFont);
    removeField->setToolTip("Remove a field");

    calculate = new QPushButton(this);
    calculate->setText("C");
    calculate->setGeometry(420, 140, 50, 50);
    calculate->setFont(*buttonFont);
    calculate->setToolTip("Calculate Grade");

    QObject::connect(calculate, SIGNAL(clicked()), this, SLOT(getGrade()));
    QObject::connect(removeField, SIGNAL(clicked()), this, SLOT(removeRow()));
    QObject::connect(addField, SIGNAL(clicked()), this, SLOT(addRow()));

}

void textEnterFrame::fillRows(QTextStream& textIn){
    QString test1 = textIn.readLine();
    QString test2 = textIn.readLine();
    // Horrible way of checking a valid file format
    if (test1.toStdString() != "Section             Grade(fraction)     Weight    " ||
        test2.toStdString() != "**************************************************"){
        std::cout << "error" << std::endl;
        return;
    }
    int linesRead = 0;
    QString sample = textIn.readLine();
    while (!sample.startsWith("Based")){
        linesRead++;
        if (linesRead >= (int)this->list.size()){
            this->addRow();
        }
        this->list.at(linesRead - 1)->fillRow(sample);
        sample = textIn.readLine();
    }
    while (!textIn.atEnd()){
        sample = textIn.readLine();
    }
    this->courseName->setText(sample);
}

std::string textEnterFrame::getPercentage(){
    return percentage;
}

bool textEnterFrame::courseNameEmpty(){
    return (courseName->toPlainText().isEmpty());
}

std::string textEnterFrame::getCourseName(){
    return this->courseName->toPlainText().toStdString();
}

void textEnterFrame::removeRow(){
    if (this->list.size() == 0){
        return;
    }
    delete this->list.at(this->list.size() - 1);
    this->list.pop_back();
    this->setFixedHeight(std::max(this->height() - 35, 205));
}

void textEnterFrame::addRow(){
    int index = this->list.size();
    sectionRowFrame *temp = new sectionRowFrame(this);
    temp->setGeometry(5, (index + 2) * 35, 500, 30);
    temp->setVisible(true);
    temp->setMinimumHeight(30);

    this->list.push_back(temp);
    if (index == 0){
        this->courseName->setTabOrder(this->courseName, this->list.at(0));
    }
    if (index > 1){
        QWidget::setTabOrder(this->list.at(index - 2), this->list.at(index - 1));
    }
    if (index >= 5){
        this->setFixedHeight(std::max(this->height() + 35, 205));
    }
}

void textEnterFrame::getGrade(){
    double totalWeight = 0;
    for (sectionRowFrame *row : this->list){
        if (!row->isEmpty()){
            totalWeight += row->getWeightValue();
        }
    }
    percentage = QString::number(totalWeight, 'f', 2).toStdString();
    emit result();
}

void textEnterFrame::clearRows(){
    for (sectionRowFrame *row : this->list){
        row->clearRow();
    }
    this->courseName->clear();
}

// This function will return a formatted string of each row of fields for saving to a text file
std::string textEnterFrame::getGradesText(){
    std::stringstream gradesText;

    //gradesText << std::endl;
    gradesText << std::left << std::setw(w1) << "Section"
                            << std::setw(w2) << "Grade(fraction)"
                            << std::setw(w3) << "Weight" << std::endl
                            << std::string((w1 + w2 + w3), '*') << std::endl;

    for (sectionRowFrame *row : this->list){
        if (!row->isEmpty()){
            gradesText << row->getRowText(w1, w2, w3) << std::endl;
        }
    }
    //gradesText << "End Of Marks" << std::endl;

    return gradesText.str();
}

