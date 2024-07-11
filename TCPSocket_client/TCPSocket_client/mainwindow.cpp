#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("TCPSocket Client");
    TCPSocket = new QTcpSocket();
    TCPSocket->connectToHost(QHostAddress("127.0.0.1"),6767);// this is local host change to the ip that u need to connect
    connect(TCPSocket,SIGNAL(readyRead()),this,SLOT(read_Data_Form_serverTCPSocket()));
//    connect(TCPSocket, &QTcpSocket::readyRead, this, &MainWindow::read_Data_From_serverTCPSocket());

    TCPSocket->open(QIODevice::ReadWrite);
    if(TCPSocket->isOpen()){
        QMessageBox::information(this,"Qt TCP Socket","Connected to the server ");
    }
    else{
        QMessageBox::information(this,"Qt TCP Socket","NOT Connected to the server ");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_onButton_clicked()
{
    if (TCPSocket && TCPSocket->isOpen()) {
        TCPSocket->write("ON");
    }
}

void MainWindow::on_offButton_clicked()
{
    if (TCPSocket && TCPSocket->isOpen()) {
        TCPSocket->write("OFF");
    }
}


void MainWindow::on_sendDataButton_clicked()
{
    if(TCPSocket){
        if(TCPSocket->isOpen()){
            QString WriteData = ui->lineEdit_Write_Data->text();
            TCPSocket->write(WriteData.toUtf8());
        }
        else{
            QMessageBox::information(this,"Qt TCP Socket","error sendata");
        }
    }
    else{
        QMessageBox::information(this,"Qt TCP Socket","error TCPSocket does not exist");
    }

}
void MainWindow::read_Data_Form_serverTCPSocket()
{
    if (TCPSocket && TCPSocket->isOpen()) {
        QByteArray dataFromServer = TCPSocket->readAll();

        // Use QTextStream to read QString from QByteArray

//        QString messageString = dataFromServer.toStdString();
        QString messageString = QString::fromUtf8(dataFromServer);

        // Update UI with received message
//        ui->textEdit_Data->setText(messageString);
        ui->textEdit_Data->append(messageString);
    }
}


void MainWindow::on_pushButton_clicked()
{
    ui->textEdit_Data->setText("");
}

