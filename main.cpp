#include "mainwindow.h"
#include "simpleserver.h"
#include <QApplication>
#include <QFileDialog>
#include <QWebEngineSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    SimpleServer server;

    {
        auto root = QFileDialog::getExistingDirectory(
                    nullptr, "Select root directory",
                    a.applicationDirPath(), QFileDialog::ShowDirsOnly);

        if (root.isEmpty()) {
            return 0;
        }

        server.setDocumentRoot(root);
        server.listen(QHostAddress::LocalHost);
        w.setHost("localhost");
        w.setPort(server.serverPort());
        w.load();
    }

    QWebEngineSettings::globalSettings()->setAttribute(
                QWebEngineSettings::FullScreenSupportEnabled, true);

    w.show();

    return a.exec();
}
