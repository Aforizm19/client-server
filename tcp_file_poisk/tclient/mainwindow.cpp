#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setText("192.168.80.131");
    ui->lineEdit->setInputMask("000.000.000.000");

    m_connection = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit->text() == QString(""))
        {

            ui->textEdit->append("Error...");
        }
    else {
    m_connection = new QTcpSocket(this);

    this->connect(m_connection, SIGNAL(readyRead()), SLOT(readyRead()));
    this->connect(m_connection, SIGNAL(disconnected()), SLOT(onDisconnected()));

    m_connection->connectToHost(ui->lineEdit->text(), ui->spinBox->value());
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
}}

void MainWindow::on_pushButton_2_clicked()
{
    m_connection->disconnectFromHost();
    delete m_connection;
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
}

void save(QString name, QByteArray data)
{
    QFile file(name);

    file.open(QFile::WriteOnly);

    file.write(data);

    file.close();
}

void MainWindow::readyRead()
{
    switch (ui->comboBox->currentIndex())
    {
    case 1:
        ui->textEdit->append(QString("Download File: ") + ui->lineEdit_3->text());
        save(ui->lineEdit_3->text(), m_connection->readAll());
        break;
    case 0:
    case 2:
    case 3:
        ui->textEdit->append(QString::fromUtf8(m_connection->readAll()));
        break;
    }

    ui->textEdit->append("```````````````````````````````````````````````````````````````````````````````````");
    ui->textEdit->append("```````````````````````````````````````````````````````````````````````````````````");
}

void MainWindow::onDisconnected()
{
}

void MainWindow::on_pushButton_3_clicked()
{
    m_connection->write(command().toUtf8());
}

QString open(QString path)
{
    QFile file(path);

    file.open(QFile::ReadOnly);

    QString data = QString::fromAscii(file.readAll().toBase64());
    file.close();

    return data;
}

QString MainWindow::command()
{
    switch (ui->comboBox->currentIndex())
    {
    case 0:
        return QString("0;") + ui->lineEdit_2->text();
        break;
    case 1:
        return QString("1;") + ui->lineEdit_2->text();
        break;
    case 2:
        return QString("2;") + open(ui->lineEdit_2->text()) + QString(";") + ui->lineEdit_3->text();
        break;
    case 3:
        return QString("3;") + ui->lineEdit_2->text() + QString(";") + ui->lineEdit_3->text();
        break;
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    ui->textEdit->clear();
}



void MainWindow::on_lineEdit_2_cursorPositionChanged()
{

}
