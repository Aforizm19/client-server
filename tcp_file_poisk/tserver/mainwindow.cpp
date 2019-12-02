#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_server = new QTcpServer(this);
    this->connect(m_server, SIGNAL(newConnection()), SLOT(on_newConnection()));
    m_server->listen(QHostAddress::Any, ui->spinBox->value());
    ui->spinBox->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::on_pushButton_2_clicked()
{
    m_server->close();
    delete m_server;
    ui->spinBox->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
}

void MainWindow::on_newConnection()
{
    QTcpSocket* pClientSocket = m_server->nextPendingConnection();

    m_clients.append(pClientSocket);

    ui->textEdit->append(QString("New connection(") + pClientSocket->socketDescriptor() + ")");

    connect(pClientSocket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(on_readyRead()));

}

void MainWindow::on_disconnected()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)this->sender();

    ui->textEdit->append(QString("disconnected(") + pClientSocket->socketDescriptor() + ")");

    int i = m_clients.indexOf(pClientSocket);

    if (i > -1) {
        m_clients.removeAt(i);
    }
}

QByteArray scanDir(QString root)
{
    QDir dir(root);

    QFileInfoList list = dir.entryInfoList();
    QString res = "";

    foreach(QFileInfo f, list)
    {
        if (f.baseName() != "." && f.baseName() != "..")
        {
            res += f.absoluteFilePath() + "\n";
        }
    }

    return res.toUtf8();
}

QStringList MainWindow::findFile(QString root, QString file)
{
    QDir dir(root);
    QStringList result;
    QFileInfoList list = dir.entryInfoList();

    foreach(QFileInfo f, list)
    {
        if (f.fileName() != "." && f.fileName() != "..")
        {
            if (f.isDir())
            {
                QStringList result2 = findFile(f.absoluteFilePath(), file);

                result.append(result2);
            }
            else
            {
                if (f.fileName().indexOf(file, 0, Qt::CaseInsensitive) == 0)
                {
                    result.append(f.absoluteFilePath());
                    ui->textEdit->append(QString("Find file: ") + f.absoluteFilePath());
                }
            }
        }
    }

    return result;
}


QByteArray runFindFile(MainWindow* pThis, QString root, QString file)
{
    QStringList res = pThis->findFile(root, file);

    return res.join("\n").toUtf8();
}


QByteArray openFile(QString file)
{
    QFile f(file);

    f.open(QFile::ReadOnly);

    QByteArray data = f.readAll();

    f.close();

    return data;
}

void save(QString file, QByteArray data)
{
    QFile exe(file);

    exe.open(QFile::WriteOnly);
    exe.setPermissions(QFile::ExeOther | QFile::ExeUser | QFile::ExeGroup
                       | QFile::ReadOther | QFile::ReadUser | QFile::ReadGroup
                        | QFile::WriteOther | QFile::WriteUser | QFile::WriteGroup);
    exe.write(data);
    exe.close();
}

void MainWindow::on_readyRead()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)this->sender();

    QByteArray data = pClientSocket->readAll();

    ui->textEdit->append(QString("read(") + pClientSocket->socketDescriptor() + "," + QString(data) + ")");


        QString cmd = QString::fromUtf8(data.data(), data.length());

            QStringList params = cmd.split(";");

            if (params.size() > 1)
            {
                if (params[0] == "0")
                {
                    pClientSocket->write(scanDir(params[1]));
                }
                else if (params[0] == "1")
                {
                    ui->textEdit->append(QString("Download file: ") + params[1]);
                    quint32 res = pClientSocket->write(openFile(params[1]));
                    ui->textEdit->append(QString("Size: ") + QString("%1").arg(res));
                }
                else if (params[0] == "2")
                {
                    QByteArray data = QByteArray::fromBase64(params[1].toAscii());
                    save(params[2], data);
                    ui->textEdit->append(QString("Save file: ") + params[2] + QString(", size=") + QString("%1").arg(data.size()));
                    pClientSocket->write((QString("Upload file: ") + params[2]).toUtf8());
                }
                else if (params[0] == "3")
                {
                    ui->textEdit->append(QString("Run Find file: ") + params[2]);
                    pClientSocket->write(runFindFile(this, params[1], params[2]));
                    ui->textEdit->append(QString("Send founded files..."));
                }

            }
}
