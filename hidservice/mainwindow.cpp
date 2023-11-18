#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../hidkit/hidservice.h"
#include "../common/transmitter.h"
#include "router.hpp"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* permission */
    requestPermission();
    /* register handler */
    HidService::instance()->registerRouter(new Router);
    /* connect */
    connect(ui->sendBtn, &QPushButton::clicked, this, [=](){
        QString text = ui->sendTextEdit->placeholderText();
        if (text.isEmpty()) {
            return;
        }
        HidService::instance()->write(text.toStdString());
    });
    connect(&Transmitter::instance(), &Transmitter::sendText,
            ui->recvTextEdit, &QTextEdit::append, Qt::QueuedConnection);
    HidService::instance()->launch();
}
void MainWindow::requestPermission()
{
#ifdef Q_OS_ANDROID
    QStringList permissions = {
        "android.permission.READ_EXTERNAL_STORAGE",
        "android.permission.WRITE_EXTERNAL_STORAGE",
        "android.permission.CAMERA",
        "android.permission.INTERNET",
        "android.hardware.usb.host",
        "android.permission.USB_PERMISSION"
    };
    for (QString &permission : permissions) {
        QtAndroid::PermissionResult result = QtAndroid::checkPermission(permission);
        if (result == QtAndroid::PermissionResult::Denied) {
            QtAndroid::requestPermissionsSync(QStringList{permission});
        }
    }
#endif
    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    HidService::instance()->stop();
    return QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

