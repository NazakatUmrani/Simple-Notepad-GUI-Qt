#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_actionNew_triggered();
    ui->textField->setDisabled(true);
}

MainWindow::~MainWindow()
{
    on_actionClose_triggered();
    delete myFile;
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    ui->textField->setEnabled(true);
    ui->textField->clear();
    myFile->close();
    myFile = new QFile(QFileDialog::getOpenFileName(this,"Open a File",QDir::homePath()));
    if (!myFile->open(QFile::ReadWrite | QFile::Text)){
        QMessageBox::critical(this,"Error", "File not opened");
        return;
    }
    this->setWindowTitle("Notepad "+myFile->fileName());
    QTextStream in(myFile);
    QString line = in.readAll();
    ui->textField->setText(line);
}

void MainWindow::on_actionNew_triggered()
{
    ui->textField->setEnabled(true);
    myFile = new QFile;
    ui->textField->clear();
    myFile->setFileName(QDir::homePath()+QString::fromStdString("SimpleNotepadTempFile.txt"));
    myFile->open(QIODevice::ReadWrite | QIODevice::Text);
    this->setWindowTitle("Notepad UntitledFile");
}

void MainWindow::on_actionClose_triggered()
{
    ui->textField->setDisabled(true);
    if(myFile->isOpen()){
        myFile->close();
        ui->textField->clear();
        on_actionNew_triggered();
        this->setWindowTitle("Notepad");
    }
    else
        QMessageBox::information(this, "Information", "No file is opened");
}

void MainWindow::on_actionSave_triggered()
{
    QTextStream out(myFile);
    if(myFile->fileName() == (QDir::homePath()+"SimpleNotepadTempFile.txt")){
        myFile = new QFile(QFileDialog::getOpenFileName(this, "Select a name and Location", QDir::homePath()));
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
