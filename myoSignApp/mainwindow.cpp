#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

std::string gestureName;
int testNumber;

bool singleWordRadio;
bool sentenceOptionRadio;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    singleWordRadio = true;
    sentenceOptionRadio = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString str = ui->gesturename->text();
    testNumber = ui->testNum->text().toInt();
    if(testNumber == 0 || testNumber == NULL){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Must enter the number of times to test!");
        messageBox.setFixedSize(500,200);
    }
    if(str.size() == 0){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Must enter a gesture name!");
        messageBox.setFixedSize(500,200);
    }

    //QLabel strLabel = &str.QString;
    ui->outputLabel->setText(str);

}

void MainWindow::on_singleWordOption_pressed()
{
    singleWordRadio = true;
    sentenceOptionRadio = false;
}

void MainWindow::on_sentenceOption_pressed()
{
    singleWordRadio = false;
    sentenceOptionRadio = true;
}
