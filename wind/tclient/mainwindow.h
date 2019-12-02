#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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

    void readyRead();
    void onDisconnected();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_2_cursorPositionChanged();

private:
    Ui::MainWindow *ui;
    QTcpSocket* m_connection;

    QString command();
};

#endif // MAINWINDOW_H
