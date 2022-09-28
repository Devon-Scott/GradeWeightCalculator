#include "mainwindow.h"
#include "qapplication.h"

/* to do list:
 * change entry list into scrollable area (just the entries, not the whole thing)
 * - Make sub-window widget in textEnterFrame that holds the vector
 * - Connect the relevant signals and slots to the sub-window class
 * - Make textEnterFrame->scrollArea->setWidget(sub-window)
 * implement return key to trigger calculation
 * Error handliing (Valid input checking)
 * Mathematical stack based calculator in sectionRowFrame::Grade
 * */

/* challenges encountered:
 * Problem: When the addRow button was clicked, the vector would add a new row,
 *          but it wouldn't show up on screen
 * Solution: After making a forum post asking about widget visibility, I
 *           realized I needed to set the visibility of the new row to true, even
 *           though the rows added in the constructor were visible by default
 * Problem: The scroll area would either squish or stack sectionRowFrames when they were
 *          added to the vector, rather than extending past the viewport and enabling scrollbars
 * Solution: 1. Switched scroll area to main window, used textEnterFrame as its widget,
 *              and resized textEnterFrame on every addRow or removeRow signal. This is inelegant though
 *              and I know I can do better
 * Problem: Adding a reset button, and causing it to trigger QTextField::clear(), the function was called,
 *          but the text fields would not clear
 * Solution: Syntax error where I was not de-referencing the pointer to sectionRowFrame. Changed row
 *           to *row, and row.clearRow() to row->clearRow
 * Problem: Weird text formatting when saving to file, certain lines would be indented with extra characters
 * Solution: Switched to QTextStream from QDataStream and formatting issues were resolved
 * */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    int frameHeight = 0;
    int instructionHeight = 120;
    int fieldHeight = 250;
    windowWidth = 520;
    int windowHeight = 550;

    this->setFixedSize(windowWidth, windowHeight);
    instructions = new QLabel("This application helps you calculate your final grade for a course\n\
Section: input the name of the section of your course (e.g. 'Assignments', 'Midterm', etc.)\n\
Grade: input the amount of marks you recieved (or expect to recieve), followed by a slash,\n\
\tand the total marks available (e.g. 29/35). If no second number is given, \n\
\tit is assumed to be 100 (like entering a percentage without % sign)\n\
Weight: input the weight of the grade as shown on your course outline (e.g. Midterm 1\n\
\tis worth 20%, so input 20)", this);
    instructions->setGeometry(5, frameHeight, windowWidth, instructionHeight);
    instructions->setAlignment(Qt::AlignVCenter);
    frameHeight += instructionHeight;

    fieldList = new textEnterFrame(this);
    fieldList->setGeometry(5, frameHeight, windowWidth - 30, fieldHeight - 5);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(fieldList);
    scrollArea->setGeometry(5, frameHeight, windowWidth - 10, fieldHeight);
    frameHeight += fieldHeight;

    output = new QLabel("", this);
    output->setGeometry(5, frameHeight, windowWidth - 10, 500 - frameHeight - 5);
    output->setFrameStyle(1);

    reset = new QPushButton(this);
    reset->setGeometry((windowWidth - 100), 500, 90, 40);
    reset->setText("Reset");
    reset->setToolTip("Clear all of the text fields");

    exit = new QPushButton(this);
    exit->setGeometry((windowWidth - 200), 500, 90, 40);
    exit->setText("Quit");
    exit->setToolTip("Exit the application");

    save = new QPushButton(this);
    save->setGeometry((this->windowWidth - 300), 500, 90, 40);
    save->setText("Save");
    save->setToolTip("Save your grades to a text file");
    save->setEnabled(false);

    load = new QPushButton(this);
    load->setGeometry((this->windowWidth - 400), 500, 90, 40);
    load->setText("Load");
    load->setToolTip("Load a Marks file for editing");

    QObject::connect(fieldList, SIGNAL(result()), this, SLOT(setOutput()));
    QObject::connect(reset, SIGNAL(clicked()), fieldList, SLOT(clearRows()));
    QObject::connect(exit, SIGNAL(clicked()), QApplication::instance(), SLOT(exit()));
    QObject::connect(save, SIGNAL(clicked()), this, SLOT(saveToFile()));
    QObject::connect(load, SIGNAL(clicked()), this, SLOT(loadFromFile()));
}

std::string MainWindow::getFinalStatement(){
    std::string result = "Based on the weights of these grades, you\n"
                         "have achieved a mark of " +
                         this->fieldList->getPercentage() + " percent";
    if (!this->fieldList->courseNameEmpty()){
        result += " in\n" + this->fieldList->getCourseName();
    } else {
        result += '.';
    }
    return result;
}

void MainWindow::setOutput(){
    std::string result = this->getFinalStatement();
    this->output->setText(result.c_str());
    this->output->setAlignment(Qt::AlignCenter);
    makeSaveButton();
}

void MainWindow::makeSaveButton(){
    this->save->setEnabled(true);
}

void MainWindow::saveToFile(){
    // The following code in this function was taken from
    // https://doc.qt.io/qt-6/qtwidgets-tutorials-addressbook-part6-example.html
    // The only modification was defining a new std::string object to pass into the data stream
    // and switching to QTextStream to fix formatting issues
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Class Marks"), "",
            tr("Marks (*.txt);;All Files (*)"));
    if (fileName.isEmpty()){
        return;
    } else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        QTextStream textOut(&file);
        std::string grades = this->fieldList->getGradesText();
        //textOut << "this is a test";
        textOut << grades.c_str();
        textOut << this->getFinalStatement().c_str();
    }
}

void MainWindow::loadFromFile(){

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Marks File"), "",
        tr("Marks (*.txt);;All Files (*)"));
    if (fileName.isEmpty())
            return;
        else {

            QFile file(fileName);

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }

            QTextStream textIn(&file);

            this->fieldList->fillRows(textIn);
            /*
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_4_5);
            contacts.clear();   // clear existing contacts
            in >> contacts;
            */
    }
}

MainWindow::~MainWindow()
{
}

