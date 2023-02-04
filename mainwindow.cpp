#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textField->setDisabled(true);
    ui->actionClose->setDisabled(true);
    ui->actionSave->setDisabled(true);
    ui->actionSave_as->setDisabled(true);
    myFile = new QFile();
}

MainWindow::~MainWindow()
{
    if(myFile->isOpen()){
        myFile->close();
    }
    if(myFile->exists(QDir::tempPath()+"/SimpleNotepadTempFile.txt")){
        myFile->remove(QDir::tempPath()+"/SimpleNotepadTempFile.txt");
    }
    delete myFile;
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    ui->textField->clear();
    if(myFile->isOpen())
        myFile->close();
    myFile->setFileName(QFileDialog::getOpenFileName(this,"Open a File",QDir::homePath()));
    if (!myFile->open(QFile::ReadWrite | QFile::Text)){
        QMessageBox::critical(this,"Error", "File not opened");
        return;
    }
    ui->textField->setEnabled(true);
    ui->actionClose->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    this->setWindowTitle("Notepad "+myFile->fileName());
    QTextStream in(myFile);
    QString line = in.readAll();
    ui->textField->setText(line);
}

void MainWindow::on_actionNew_triggered()
{
    myFile->setFileName(QDir::tempPath()+"/SimpleNotepadTempFile.txt");
    ui->textField->clear();
    if(!myFile->open(QIODevice::ReadWrite | QIODevice::Text)){
        QMessageBox::information(this,"Error","No new file created");
        return;
    }
    this->setWindowTitle("Notepad UntitledFile");
    ui->actionClose->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->textField->setEnabled(true);
}

void MainWindow::on_actionClose_triggered()
{
    ui->textField->setDisabled(true);
    if(myFile->isOpen()){
        myFile->close();
        ui->textField->clear();
        //on_actionNew_triggered();
        this->setWindowTitle("Notepad");
        ui->actionClose->setDisabled(true);
        ui->actionSave->setDisabled(true);
        ui->actionSave_as->setDisabled(true);
    }
    else
        QMessageBox::information(this, "Information", "No file is opened");
}

void MainWindow::on_actionSave_triggered()
{
    QTextStream out(myFile);
    if(myFile->fileName() == (QDir::tempPath()+"/SimpleNotepadTempFile.txt")){
        myFile = new QFile(QFileDialog::getSaveFileName(this, "Select a name and Location", QDir::homePath()));
        myFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Text);
        this->setWindowTitle("Notepad "+myFile->fileName());
        out << ui->textField->toPlainText();
        myFile->flush();
    }
    else {
        myFile->close();
        myFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
        out << ui->textField->toPlainText();
        myFile->flush();
    }
}
