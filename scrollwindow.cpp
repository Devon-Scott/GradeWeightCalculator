#include "scrollwindow.h"

scrollWindow::scrollWindow(QWidget *parent)
    : QWidget{parent}
{

}

void scrollWindow::setLayout(QLayout* l){
    this->layout = l;
}
