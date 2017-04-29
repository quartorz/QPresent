#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebEngineFullScreenRequest>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->webView->page(), &QWebEnginePage::fullScreenRequested,
            [&](QWebEngineFullScreenRequest request) {
        request.accept();
        if (request.toggleOn()) {
            maximized = isMaximized();
            this->showFullScreen();
        } else {
            if (maximized){
                showMaximized();
            } else {
                showNormal();
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_F11) {
        if (isFullScreen()) {
            if (maximized){
                showMaximized();
            } else {
                showNormal();
            }
        } else {
            maximized = isMaximized();
            showFullScreen();
        }
    }
}

void MainWindow::load()
{
    ui->webView->load(QString("http://%1:%2/").arg(host).arg(port));
}

void MainWindow::setHost(const QString &host)
{
    this->host = host;
}

void MainWindow::setPort(quint16 port)
{
    this->port = port;
}
