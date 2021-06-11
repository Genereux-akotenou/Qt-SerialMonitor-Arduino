#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qextserialport.h"
#include "qextserialenumerator.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //configuration graphique ui
    ui->setupUi(this);
    ui->btnEffacer->setIcon(QIcon(":ArduinoPortSerie/assets/delete.png" ));
    ui->btnHorloge->setIcon(QIcon(":ArduinoPortSerie/assets/horloge.jpg"));
    ui->btnDefiler->setIcon(QIcon(":ArduinoPortSerie/assets/defilement_automatique.png"));
    ui->btnConnexion->setCursor(Qt::PointingHandCursor);

    //configuration graphique non-ui
    this->setWindowTitle("EMULATEUR PORT SERIE ARDUINO-QT");
    this->setFixedSize(590, 375);

    //configuration set-up
    QextSerialEnumerator mesEmmulateurs;
    QList<QextPortInfo> mesPorts = mesEmmulateurs.getPorts();
    for(QextPortInfo unPort : mesPorts){
        ui->comboPort->addItem(unPort.physName);
    }
    ui->comboPort->setCurrentIndex(mesPorts.size()-1);
    QList<QString> listVitesse = {"300", "1200", "2400", "4800", "9600", "14400", "19200", "38400", "57600", "115200"};
    for(QString vitesse : listVitesse){
        ui->comboVitesse->addItem(vitesse);
    }
    ui->comboVitesse->setCurrentText("9600");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnConnexion_clicked()
{
    if(ui->btnConnexion->isChecked() ){
        port = new QextSerialPort();
        port->setPortName(ui->comboPort->currentText());
        port->setBaudRate(getBaudRateFromString(ui->comboVitesse->currentText()));

        port->setParity(PAR_NONE);
        port->setStopBits(STOP_1);
        port->setDataBits(DATA_8);
        port->setFlowControl(FLOW_OFF);
        port->open(QextSerialPort::ReadWrite);

        ui->btnConnexion->setText("Deconnexion");
        ui->btnConnexion->setStyleSheet(QString::fromUtf8("background-color: rgb(204, 0, 0); color: white; font-weight: bold;"));

        connect(port, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(ui->boxEmission, SIGNAL(textChanged()), this, SLOT(sendData()));
    }
    else{
        port->close();
        delete port;
        ui->btnConnexion->setStyleSheet(QString::fromUtf8("background-color: rgb(78, 154, 6); color: white; font-weight: bold;"));
        ui->btnConnexion->setText("Se connecter");
        //ui->boxEmission->setText("");
        //ui->boxReception->setPlainText("");
    }
}

BaudRateType MainWindow::getBaudRateFromString(QString baudRate) {
    int vitesse = baudRate.toInt();
    switch(vitesse) {
        case(300):
            return BAUD300;
        case(1200):
            return BAUD1200;
        case(2400):
            return BAUD2400;
        case(4800):
            return BAUD4800;
        case(9600):
            return BAUD9600;
        case(19200):
            return BAUD19200;
        case(38400):
            return BAUD38400;
        case(57600):
            return BAUD57600;
        case(115200):
            return BAUD115200;
        default:
            return BAUD9600;
    }
}

void MainWindow::sendData(){
    QByteArray data = ui->boxEmission->toPlainText().right(1).toLatin1() ;
    if(port != NULL && port->isOpen()){
        port->write(data);
    }
}

void MainWindow::readData() {
    //mettre a jourla position du curseurcers le bas
    if(ui->btnDefiler->isChecked()){
        QTextCursor curseur = ui->boxReception->textCursor();
        curseur.movePosition(QTextCursor::End);
        ui->boxReception->setTextCursor(curseur);
        ui->btnDefiler->setStyleSheet(QString::fromUtf8("border: 2px solid rgb(78, 154, 6); background-color: white;"));//background-color: rgb(78, 154, 6);
    }
    //executer l'horodatage
    if(ui->btnHorloge->isChecked()){
        /*
        * Have to display current time a the begin of each line
        * when user checked time buttom
        */
        /*
        ui->boxReception->insertPlainText("DATE\n");
        QByteArray array = port->readAll();
        ui->boxReception->insertPlainText(array);
        */
    }
    else {
        QByteArray array = port->readAll();
        ui->boxReception->insertPlainText(array);
    }
}

void MainWindow::on_btnDefiler_clicked()
{
    if(ui->btnDefiler->isChecked()){
        ui->btnDefiler->setStyleSheet(QString::fromUtf8("border: 2px solid rgb(78, 154, 6); background-color: white;"));//background-color: rgb(78, 154, 6);
    }
    else{
        ui->btnDefiler->setStyleSheet(QString::fromUtf8("border: 1px solid rgb(136, 138, 133); background-color: white;"));
    }
}

void MainWindow::on_btnHorloge_clicked()
{
    if(ui->btnHorloge->isChecked()){
        ui->btnHorloge->setStyleSheet(QString::fromUtf8("border: 2px solid rgb(78, 154, 6); background-color: white;"));//background-color: rgb(78, 154, 6);
    }
    else{
        ui->btnHorloge->setStyleSheet(QString::fromUtf8("border: 1px solid rgb(136, 138, 133); background-color: white;"));
    }
}

void MainWindow::on_btnEffacer_clicked()
{
    ui->boxReception->setPlainText("");
}

void MainWindow::on_btnEffacer_pressed()
{
    ui->btnEffacer->setStyleSheet(QString::fromUtf8("border: 2px solid rgb(78, 154, 6); background-color: white;"));//background-color: rgb(78, 154, 6);
}

void MainWindow::on_btnEffacer_released()
{
    ui->btnEffacer->setStyleSheet(QString::fromUtf8("border: 1px solid rgb(136, 138, 133); background-color: white;"));
}
