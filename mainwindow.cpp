#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_actionNew_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    ui->textField->clear();
    File = new QFile(QFileDialog::getOpenFileName(this,"Open a File",QDir::homePath()));
    //openFile.open(QFile::ReadWrite | QFile::Text);
    if (!File->open(QFile::ReadOnly | QFile::Text))
        QMessageBox::critical(this,"Error", "File not opened");
    while (!File->atEnd()) {
        QByteArray line = File->readLine();
        ui->textField->setText(QString::fromStdString(ui->textField->toPlainText().toStdString()+line.toStdString()));
    }
}


void MainWindow::on_actionNew_triggered()
{
    ui->textField->clear();
}


void MainWindow::on_actionClose_triggered()
{
    if(File->isOpen()){
        File->close();
        ui->textField->clear();
    }
    else
        QMessageBox::information(this, "Information", "No file is opened");
}

