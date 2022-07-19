#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    bool eventFilter(QObject*,QEvent*);
private slots:
    void on_pushButton_clicked();

    void readData();

    void on_pushButton_right_clicked();

    void on_pushButton_left_clicked();

    void on_pushButton_rInc_clicked();

    void on_pushButton_rDec_clicked();

    void on_pushButton_lInc_clicked();

    void on_pushButton_lDec_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

    QString bufData;
    QString dataStr;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint currentPos;
    QPoint newPos;

protected:
     void keyPressEvent(QKeyEvent* e);
};
#endif // MAINWINDOW_H
