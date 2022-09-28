#include "sectionrowframe.h"
#include <iomanip>
#include <iostream>

sectionRowFrame::sectionRowFrame(QFrame *parent) : QFrame(parent){
    fields = new QTextEdit*[FIELDS];
    for (int i = 0; i < 3; i++){
        fields[i] = new QTextEdit(this);
        fields[i]->setGeometry((125 * i), 0, 125, 30);
        fields[i]->setTabChangesFocus(true);
        if (i > 0){
           QWidget::setTabOrder(fields[i - 1], fields[i]);
        }
    }
}

std::string sectionRowFrame::getSectionName(){
    return fields[0]->toPlainText().toStdString();
}

bool sectionRowFrame::isEmpty(){
    for (int i = 0; i < FIELDS; i++){
        if (fields[i]->toPlainText().toStdString() != ""){
            return false;
        }
    }
    return true;
}

std::string sectionRowFrame::getSectionMark(){
    std::string result = "(";
    result += this->fields[1]->toPlainText().toStdString() + ")";
    return result;
}

void sectionRowFrame::clearRow(){
    for (int i = 0; i < FIELDS; i++){
        this->fields[i]->clear();
    }
}

void sectionRowFrame::fillRow(QString &sample){
    // Split the QString into 3 parts
    QString field0, field1, field2;
    int i = 0, length = sample.length();
    while (!(sample[i] == ' ' && sample[i + 1] == ' ')){
        field0 += sample[i];
        i++;
    }
    while (sample[i] != '('){i++;}
    i++;

    while (sample[i] != ')'){
        field1 += sample[i];
        i++;
    }

    while (!sample[i].isDigit()){i++;}

    while (i < length){
        field2 += sample[i];
        i++;
    }
    fields[0]->setText(field0);
    fields[1]->setText(field1);
    fields[2]->setText(field2);
}

std::string sectionRowFrame::getRowText(int w1, int w2, int w3){
    std::stringstream rowText;
    double percentage = 0;
    try {
        percentage  = this->getGradeValue() * 100;
        std::stringstream percentageMark;

        percentageMark << std::setprecision(4) << percentage;
        percentageMark << this->getSectionMark();

        rowText << std::left
                << std::setw(w1)
                << this->getSectionName()
                << std::setw(w2)
                << percentageMark.str()
                << std::setw(w3)
                << fields[2]->toPlainText().toStdString();
    } catch (const char* s){
    }

    return rowText.str();
}

// returns a double value between 0 and 1 indicating a percentage value for the grade
double sectionRowFrame::getGradeValue(){
    if (this->fields[1]->toPlainText().toStdString() == ""){
        throw ("no value here");
    } else {
        double* result = new double[2];
        std::vector<std::string> tokens;

        std::stringstream check(this->fields[1]->toPlainText().toStdString());
        std::string temp;

        while (getline(check, temp, '/')){
            tokens.push_back(temp);
        }
        if (tokens.size() == 1){
            tokens.push_back("100");
        }
        for (int i = 0; i < 2; i++){
            result[i] = std::stod(tokens.at(i));
        }
        double gradeValue = (result[0] / result[1]);
        delete[] result;
        return gradeValue;
    }
}

// invokes sectionRowFrame::getGradeValue() and multiplies it by the weight of the section
// to provide a final value for the section
double sectionRowFrame::getWeightValue(){
    double weight = std::stod(fields[2]->toPlainText().toStdString());
    double result = this->getGradeValue() * weight;
    return result;
}

