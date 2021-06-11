#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qextserialport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:
        //var creer via ui
        Ui::MainWindow *ui;

        //set-up non ui
        QextSerialPort* port;
        BaudRateType getBaudRateFromString(QString baudRate);

    private slots:
            void on_btnConnexion_clicked();
            void sendData();
            void readData();
            void on_btnDefiler_clicked();
            void on_btnHorloge_clicked();
            void on_btnEffacer_clicked();
            void on_btnEffacer_pressed();
            void on_btnEffacer_released();

public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

};
#endif
