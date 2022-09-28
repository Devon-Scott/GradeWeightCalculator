#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include "textenterframe.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    textEnterFrame *fieldList;
    QLabel *instructions, *output;
    QScrollArea *scrollArea;
    QPushButton *reset, *exit, *save, *load;
    std::string getGradesText(), getFinalStatement();
    void makeSaveButton();
    int windowWidth;

private slots:
    void setOutput();
    void saveToFile();
    void loadFromFile();

};
#endif // MAINWINDOW_H
