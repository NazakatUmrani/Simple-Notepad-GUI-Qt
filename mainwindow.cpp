#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    if(myFile->isOpen())
        myFile->close();
    myFile->setFileName(QFileDialog::getOpenFileName(this,"Open a File",QDir::homePath()));
    if (!myFile->open(QFile::ReadWrite | QFile::Text)){
        on_actionClose_triggered();
        return;
    }
    setActions(true);
    this->setWindowTitle("Notepad "+myFile->fileName());
    QTextStream in(myFile);
    QString line = in.readAll();
    ui->textField->setText(line);
}

void MainWindow::on_actionNew_triggered()
{
    myFile->setFileName(QDir::tempPath()+"/SimpleNotepadTempFile.txt");
    if(!myFile->open(QIODevice::ReadWrite | QIODevice::Text)){
        QMessageBox::information(this,"Error","No new file created");
        return;
    }
    this->setWindowTitle("Notepad UntitledFile");
    setActions(true);
}

void MainWindow::on_actionClose_triggered()
{
    setActions(false);
    this->setWindowTitle("Notepad");
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

void MainWindow::setActions(bool x){
    ui->textField->clear();
    ui->textField->setEnabled(x);
    ui->textField->setFocus();
    ui->actionClose->setEnabled(x);
    ui->actionSave->setEnabled(x);
    ui->actionSave_as->setEnabled(x);
    ui->actionCopy->setEnabled(x);
    ui->actionCut->setEnabled(x);
    ui->actionPaste->setEnabled(x);
    ui->actionUndo->setEnabled(x);
    ui->actionRedo->setEnabled(x);
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textField->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textField->cut();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textField->paste();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textField->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textField->redo();
}

