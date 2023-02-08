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
        on_actionClose_triggered();
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
    ui->textField->clear();
    this->setWindowTitle("Notepad");
    ui->actionClose->setDisabled(true);
    ui->actionSave->setDisabled(true);
    ui->actionSave_as->setDisabled(true);
    if(myFile->isOpen()){
        myFile->close();
    }
    else
        QMessageBox::warning(this, "Warning", "No file selected\nNo file is opened try again...");
}

void MainWindow::on_actionSave_triggered()
{
    if(myFile->fileName() == (QDir::tempPath()+"/SimpleNotepadTempFile.txt")){
        myFile = new QFile(QFileDialog::getSaveFileName(this, "Select a name and Location", QDir::homePath()));
        QTextStream out(myFile);
        myFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
        this->setWindowTitle("Notepad "+myFile->fileName());
        out << ui->textField->toPlainText();
        myFile->flush();
    }
    else {
        myFile->close();
        myFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
        QTextStream out(myFile);
        out << ui->textField->toPlainText();
        myFile->flush();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox aboutDev(QMessageBox::NoIcon,"About Developer","This app is developed by Nazakat Umrani (21SW49)\nFor More information visit my Github Account\nGithub: github.com/nazakatumrani",QMessageBox::Close);
    QPixmap devPic(":/resources/icons/devPic.png");
    aboutDev.setIconPixmap(devPic.scaled(100,100,Qt::KeepAspectRatio));
    aboutDev.exec();
}
