/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *led_control_gr;
    QPushButton *onButton;
    QPushButton *offButton;
    QLabel *lblLedStatus;
    QLabel *lblStatus;
    QGroupBox *groupBox;
    QPushButton *sendDataButton;
    QLineEdit *lineEdit_Write_Data;
    QTextEdit *textEdit_Data;
    QPushButton *pushButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(599, 551);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        led_control_gr = new QGroupBox(centralwidget);
        led_control_gr->setObjectName("led_control_gr");
        led_control_gr->setGeometry(QRect(380, 0, 211, 161));
        onButton = new QPushButton(led_control_gr);
        onButton->setObjectName("onButton");
        onButton->setGeometry(QRect(10, 50, 93, 29));
        offButton = new QPushButton(led_control_gr);
        offButton->setObjectName("offButton");
        offButton->setGeometry(QRect(10, 90, 93, 29));
        lblLedStatus = new QLabel(led_control_gr);
        lblLedStatus->setObjectName("lblLedStatus");
        lblLedStatus->setGeometry(QRect(130, 20, 31, 20));
        lblStatus = new QLabel(led_control_gr);
        lblStatus->setObjectName("lblStatus");
        lblStatus->setGeometry(QRect(130, 50, 61, 61));
        QFont font;
        font.setPointSize(14);
        lblStatus->setFont(font);
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 0, 341, 501));
        sendDataButton = new QPushButton(groupBox);
        sendDataButton->setObjectName("sendDataButton");
        sendDataButton->setGeometry(QRect(240, 30, 93, 29));
        lineEdit_Write_Data = new QLineEdit(groupBox);
        lineEdit_Write_Data->setObjectName("lineEdit_Write_Data");
        lineEdit_Write_Data->setGeometry(QRect(10, 30, 221, 25));
        textEdit_Data = new QTextEdit(groupBox);
        textEdit_Data->setObjectName("textEdit_Data");
        textEdit_Data->setGeometry(QRect(10, 70, 321, 371));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(120, 460, 93, 29));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        led_control_gr->setTitle(QCoreApplication::translate("MainWindow", "LED STATUS", nullptr));
        onButton->setText(QCoreApplication::translate("MainWindow", "ON", nullptr));
        offButton->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
        lblLedStatus->setText(QCoreApplication::translate("MainWindow", "Led", nullptr));
        lblStatus->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "SOCKET", nullptr));
        sendDataButton->setText(QCoreApplication::translate("MainWindow", "Send data ", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "clear data", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
