#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QScreen>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");
static int speed = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort;

    dataStr = "";

    move(QGuiApplication::screens().at(0)->geometry().center() - frameGeometry().center());
    this->setWindowFlags(Qt::FramelessWindowHint);

    QString description;
    QString manufacturer;
    QString serialNumber;
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();

    qDebug() << "Kullanılabilir Port Sayısı: " << serialPortInfos.count();

    for (const QSerialPortInfo &serialPortInfo : serialPortInfos)
    {
     QStringList list;
     description = serialPortInfo.description();
     manufacturer = serialPortInfo.manufacturer();
     serialNumber = serialPortInfo.serialNumber();
     list << serialPortInfo.portName()
       << (!description.isEmpty() ? description : blankString)
       << (!manufacturer.isEmpty() ? manufacturer : blankString)
       << (!serialNumber.isEmpty() ? serialNumber : blankString)
       << serialPortInfo.systemLocation()
       << (serialPortInfo.vendorIdentifier() ? QString::number(serialPortInfo.vendorIdentifier(), 16) : blankString)
       << (serialPortInfo.productIdentifier() ? QString::number(serialPortInfo.productIdentifier(), 16) : blankString);
     ui->comboBox_serialPort->addItem(list.first(), list);
    }
    ui->comboBox_serialPort->addItem(tr("custom"));

    ui->comboBox_baudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->comboBox_baudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->comboBox_baudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->comboBox_baudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->comboBox_baudRate->addItem(tr("Custom"));

    ui->comboBox_dataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->comboBox_dataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->comboBox_dataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->comboBox_dataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->comboBox_dataBits->setCurrentIndex(3);

    ui->comboBox_parity->addItem(tr("None"), QSerialPort::NoParity);
    ui->comboBox_parity->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->comboBox_parity->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->comboBox_parity->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->comboBox_parity->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->comboBox_stopBit->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->comboBox_stopBit->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    qDebug() << ui->pushButton->text();
     if (ui->pushButton->text() == tr ("BAĞLAN"))
     {

      serial->setPortName(ui->comboBox_serialPort->currentText());
      serial->setBaudRate(ui->comboBox_baudRate->currentText().toInt());
      serial->setDataBits(QSerialPort::Data8);
      serial->setParity(QSerialPort::NoParity);
      serial->setStopBits(QSerialPort::OneStop);
      serial->setFlowControl(QSerialPort::NoFlowControl);
      if (serial->open(QIODevice::ReadWrite))
      {
       ui->comboBox_baudRate->setEnabled(false);
       ui->comboBox_dataBits->setEnabled(false);
       ui->comboBox_parity->setEnabled(false);
       ui->comboBox_serialPort->setEnabled(false);
       ui->comboBox_stopBit->setEnabled(false);
       ui->pushButton->setText(tr ("DURDUR"));

       connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
      }
     }
     else
     {

      serial->close();
      ui->comboBox_baudRate->setEnabled(true);
      ui->comboBox_dataBits->setEnabled(true);
      ui->comboBox_parity->setEnabled(true);
      ui->comboBox_serialPort->setEnabled(true);
      ui->comboBox_stopBit->setEnabled(true);
      ui->pushButton->setText(tr ("BAĞLAN"));

     }
}

void MainWindow::readData(){


    QStringList bufSplit = dataStr.split("\r\n");
    qDebug() << bufData;

    if(bufSplit.length() < 3){
        bufData = serial->readAll();
        dataStr += QString::fromStdString(bufData.toStdString());
    }

    else{
        dataStr = "";
        ui->label_angle->setText(bufSplit[2]);
        ui->label_val2->setText(bufSplit[0]);
        ui->label_speed->setText(bufSplit[1]);
    }
    //qDebug()<<bufSplit;

    /*
    QStringList bufSplit = bufData.split("\r\n");
    QString gelenVeri;
    QByteArray dataRcv;
    if(bufSplit.length() < 3){
        dataRcv = serial->readAll();
        buf = serial->readAll();
        bufData += QString::fromStdString(buf.toStdString());

    }
    else{
       for(QString s : bufSplit){

            qDebug()<<s;
            ui->label_test->setText(s);
            ui->textEdit->append(s);
       }

        bufData = "";
        //ui->textEdit->clear();
    }
    */
}


void MainWindow::keyPressEvent(QKeyEvent *e){

    switch (e->key()) {
    case Qt::Key_W:
        //serial->write("d");
        ui->label->setText("+10");
        serial->write("W");
        break;
    case Qt::Key_S:
        //serial->write("u");
        ui->label->setText("-10");
        serial->write("S");
        break;
    case Qt::Key_A:
        //serial->write("u");
        ui->label->setText("+25");
        serial->write("A");
        break;

    case Qt::Key_D:
        //serial->write("u");
        ui->label->setText("-25");
        serial->write("D");
        break;

    default:
        break;
    }

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
 {
     if (event->type() == QEvent::KeyPress) {
         //and here put your own logic!!
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
         qDebug(" %d", keyEvent->key());
         return true;
     } else {
         // standard event processing
         return QObject::eventFilter(obj, event);
     }
 }

void MainWindow::mousePressEvent(QMouseEvent *event){
    currentPos = QPoint(event->globalPos());
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
 newPos = QPoint( QPoint(event->globalPos() ) - currentPos);
 move(x() + newPos.x() , y() + newPos.y());
 currentPos = QPoint(event->globalPos());
}


void MainWindow::on_pushButton_right_clicked()
{
    serial->write("R");
}

void MainWindow::on_pushButton_left_clicked()
{
    serial->write("L");
}


/* Rigth Increase Decrease Operation */
void MainWindow::on_pushButton_rInc_clicked()
{
    serial->write("I");
}

void MainWindow::on_pushButton_rDec_clicked()
{
    serial->write("D");
}

/* Left Increase Decrease Operation  */

void MainWindow::on_pushButton_lInc_clicked()
{
    serial->write("i");
}

void MainWindow::on_pushButton_lDec_clicked()
{
    serial->write("d");
}
