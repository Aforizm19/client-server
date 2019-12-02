#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_newConnection();
    void on_disconnected();
    void on_readyRead();

private:
    Ui::MainWindow *ui;
    QTcpServer *m_server;
    QList<QTcpSocket*> m_clients;
public:
    QStringList findFile(QString root, QString file);
};

#endif // MAINWINDOW_H
