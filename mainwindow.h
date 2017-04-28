#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *);

    void load();
    void setHost(const QString &host);
    void setPort(quint16 port);

private:
    Ui::MainWindow *ui;
    bool maximized;
    QString host;
    quint16 port;

};

#endif // MAINWINDOW_H
