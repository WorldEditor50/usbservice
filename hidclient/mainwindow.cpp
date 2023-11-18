#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hidclient.h"
#include "router.hpp"
#include "../common/transmitter.h"
#include "../common/message_def.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    unsigned short vid = 0x0001;
    unsigned short pid = 0x0002;
    /* register router */
    HidClient::instance()->registerRouter(new Router);
    /* connect */
    connect(&Transmitter::instance(), &Transmitter::sendText,
            ui->recvTextEdit, &QTextEdit::append, Qt::QueuedConnection);

    connect(ui->openBtn, &QPushButton::clicked, this, [=](){
        HidClient::instance()->start(vid, pid);
    });
    connect(ui->closeBtn, &QPushButton::clicked, this, [=](){
        HidClient::instance()->stop();
    });
    connect(ui->connectBtn, &QPushButton::clicked, this, [=](){
        HidClient::instance()->connect();
    });
    connect(ui->disconnectBtn, &QPushButton::clicked, this, [=](){
        HidClient::instance()->disconnect();
    });

    connect(ui->sendBtn, &QPushButton::clicked, this, [=](){
        QString text = ui->sendTextEdit->placeholderText();
        if (text.isEmpty()) {
            return;
        }
        HidClient::instance()->sendText(text.toStdString());
        return;
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    HidClient::instance()->stop();
    return QMainWindow::closeEvent(event);
}
